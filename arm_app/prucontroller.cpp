#include "prucontroller.h"
#include <QFileDevice>
#include <cstring>
#include <errno.h>

#include <QDebug>
#include <QtGlobal>
#include <cmath>

#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

namespace {
constexpr off_t PRU_SHARED_RAM_PHYS = 0x4A310000;
constexpr size_t PRU_SHARED_RAM_SIZE = 0x1000;
constexpr double PRU_CLOCK_HZ = 200000000.0;

const char *REMOTEPROC_STATE = "/sys/class/remoteproc/remoteproc1/state";
const char *REMOTEPROC_FW    = "/sys/class/remoteproc/remoteproc1/firmware";
const char *FIRMWARE_NAME    = "pru_pwm_control.out";
}

PruController::PruController(QObject *parent)
    : QObject(parent),
      m_enabled(false),
      m_frequencyHz(1000),
      m_dutyPercent(50),
      m_running(false),
      m_statusText("Idle"),
      m_memFd(-1),
      m_mapBase(nullptr),
      m_shared(nullptr)
{
    refreshState();
    setStatus("Ready");
}

PruController::~PruController()
{
    unmapSharedRam();
}

bool PruController::enabled() const
{
    return m_enabled;
}

int PruController::frequencyHz() const
{
    return m_frequencyHz;
}

int PruController::dutyPercent() const
{
    return m_dutyPercent;
}

QString PruController::statusText() const
{
    return m_statusText;
}

bool PruController::running() const
{
    return m_running;
}

void PruController::setEnabled(bool value)
{
    if (m_enabled == value) {
        return;
    }
    m_enabled = value;
    emit enabledChanged();
    updateSharedStruct();
}

void PruController::setFrequencyHz(int value)
{
    value = qBound(1, value, 50000);
    if (m_frequencyHz == value) {
        return;
    }
    m_frequencyHz = value;
    emit frequencyHzChanged();
    updateSharedStruct();
}

void PruController::setDutyPercent(int value)
{
    value = qBound(1, value, 99);
    if (m_dutyPercent == value) {
        return;
    }
    m_dutyPercent = value;
    emit dutyPercentChanged();
    updateSharedStruct();
}
void PruController::applySettings()
{
    if (!m_shared) {
        if (!mapSharedRam()) {
            setStatus("Failed to map shared RAM. Need elevated permissions.");
            return;
        }

        if (m_shared->magic != PRU_SHARED_MAGIC) {
            m_shared->magic      = PRU_SHARED_MAGIC;
            m_shared->enable     = 0;
            m_shared->mode       = PRU_MODE_PWM;
            m_shared->out_mask   = (1u << 15);
            m_shared->high_count = 10000;
            m_shared->low_count  = 10000;
            m_shared->reserved0  = 0;
            m_shared->reserved1  = 0;
        }
    }

    updateSharedStruct();
    setStatus(QString("Applied: %1 Hz, %2% duty")
              .arg(m_frequencyHz)
              .arg(m_dutyPercent));
}

void PruController::startFirmware()
{
    if (!writeSysfs(REMOTEPROC_STATE, "stop")) {
        setStatus("Failed to stop PRU before reload. Need elevated permissions?");
        return;
    }

    if (!writeSysfs(REMOTEPROC_FW, FIRMWARE_NAME)) {
        setStatus("Failed to write firmware name. Need elevated permissions?");
        return;
    }

    if (!writeSysfs(REMOTEPROC_STATE, "start")) {
        setStatus("Failed to start PRU firmware. Need elevated permissions?");
        return;
    }

    if (!mapSharedRam()) {
        setStatus("Firmware started, but failed to map shared RAM.");
        refreshState();
        return;
    }

    if (m_shared->magic != PRU_SHARED_MAGIC) {
        m_shared->magic      = PRU_SHARED_MAGIC;
        m_shared->enable     = 0;
        m_shared->mode       = PRU_MODE_PWM;
        m_shared->out_mask   = (1u << 15);
        m_shared->high_count = 10000;
        m_shared->low_count  = 10000;
        m_shared->reserved0  = 0;
        m_shared->reserved1  = 0;
    }

    updateSharedStruct();
    refreshState();
    setStatus("PRU firmware started");
}

void PruController::stopFirmware()
{
    unmapSharedRam();

    if (writeSysfs(REMOTEPROC_STATE, "stop")) {
        m_running = false;
        emit runningChanged();
        setStatus("PRU firmware stopped");
    } else {
        setStatus("Failed to stop PRU firmware");
    }
}

void PruController::refreshState()
{
    const QString state = readSysfs(REMOTEPROC_STATE).trimmed();
    const bool isRunning = (state == "running");

    if (m_running != isRunning) {
        m_running = isRunning;
        emit runningChanged();
    }

    if (!state.isEmpty()) {
        setStatus(QString("PRU state: %1").arg(state));
    }
}

bool PruController::mapSharedRam()
{
    if (m_shared) {
        return true;
    }

    m_memFd = ::open("/dev/mem", O_RDWR | O_SYNC);
    if (m_memFd < 0) {
        return false;
    }

    m_mapBase = ::mmap(nullptr,
                       PRU_SHARED_RAM_SIZE,
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED,
                       m_memFd,
                       PRU_SHARED_RAM_PHYS);

    if (m_mapBase == MAP_FAILED) {
        m_mapBase = nullptr;
        ::close(m_memFd);
        m_memFd = -1;
        return false;
    }

    m_shared = static_cast<pru_shared_t *>(m_mapBase);
    return true;
}

void PruController::unmapSharedRam()
{
    if (m_mapBase) {
        ::munmap(m_mapBase, PRU_SHARED_RAM_SIZE);
        m_mapBase = nullptr;
    }

    if (m_memFd >= 0) {
        ::close(m_memFd);
        m_memFd = -1;
    }

    m_shared = nullptr;
}

bool PruController::writeSysfs(const QString &path, const QString &value)
{
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "writeSysfs failed:" << path << f.errorString();
        return false;
    }

    const QByteArray bytes = value.toUtf8();
    const bool ok = (f.write(bytes) == bytes.size());
    if (!ok) {
        qWarning() << "writeSysfs write failed:" << path << f.errorString();
    }

    f.close();
    return ok;
}

QString PruController::readSysfs(const QString &path) const
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        qWarning() << "readSysfs failed:" << path << f.errorString();
        return QString();
    }

    const QString value = QString::fromUtf8(f.readAll());
    f.close();
    return value;
}

void PruController::setStatus(const QString &text)
{
    if (m_statusText == text) {
        return;
    }
    m_statusText = text;
    emit statusTextChanged();
}

void PruController::updateSharedStruct()
{
    if (!m_shared) {
        qDebug() << "updateSharedStruct: m_shared is null";
        return;
    }

    constexpr double DELAY_LOOP_CYCLES = 2.0;
    const double periodCounts =
        (PRU_CLOCK_HZ / static_cast<double>(m_frequencyHz)) / DELAY_LOOP_CYCLES;

    std::uint32_t totalCount =
        static_cast<std::uint32_t>(std::llround(periodCounts));
    if (totalCount < 2u) {
        totalCount = 2u;
    }

    std::uint32_t highCount = static_cast<std::uint32_t>(
        std::llround(static_cast<double>(totalCount) *
                     (static_cast<double>(m_dutyPercent) / 100.0)));
    if (highCount < 1u) {
        highCount = 1u;
    }
    if (highCount >= totalCount) {
        highCount = totalCount - 1u;
    }

    std::uint32_t lowCount = totalCount - highCount;
    if (lowCount < 1u) {
        lowCount = 1u;
    }

    m_shared->magic      = PRU_SHARED_MAGIC;
    m_shared->enable     = m_enabled ? 1u : 0u;
    m_shared->mode       = PRU_MODE_PWM;
    m_shared->out_mask   = (1u << 15);
    m_shared->high_count = highCount;
    m_shared->low_count  = lowCount;

    qDebug() << "WRITE:"
             << "enable=" << m_shared->enable
             << "freq=" << m_frequencyHz
             << "duty=" << m_dutyPercent
             << "total=" << totalCount
             << "high=" << m_shared->high_count
             << "low=" << m_shared->low_count;
}