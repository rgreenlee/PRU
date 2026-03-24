#ifndef PRUCONTROLLER_H
#define PRUCONTROLLER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <cstdint>

extern "C" {
#include "../firmware/pru_shared.h"
}

class PruController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(int frequencyHz READ frequencyHz WRITE setFrequencyHz NOTIFY frequencyHzChanged)
    Q_PROPERTY(int dutyPercent READ dutyPercent WRITE setDutyPercent NOTIFY dutyPercentChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY statusTextChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
    explicit PruController(QObject *parent = nullptr);
    ~PruController();

    bool enabled() const;
    int frequencyHz() const;
    int dutyPercent() const;
    QString statusText() const;
    bool running() const;

    void setEnabled(bool value);
    void setFrequencyHz(int value);
    void setDutyPercent(int value);

    Q_INVOKABLE void applySettings();
    Q_INVOKABLE void startFirmware();
    Q_INVOKABLE void stopFirmware();
    Q_INVOKABLE void refreshState();

signals:
    void enabledChanged();
    void frequencyHzChanged();
    void dutyPercentChanged();
    void statusTextChanged();
    void runningChanged();

private:
    bool mapSharedRam();
    void unmapSharedRam();
    bool writeSysfs(const QString &path, const QString &value);
    QString readSysfs(const QString &path) const;
    void setStatus(const QString &text);
    void updateSharedStruct();

private:
    bool m_enabled;
    int m_frequencyHz;
    int m_dutyPercent;
    bool m_running;
    QString m_statusText;

    int m_memFd;
    void *m_mapBase;
    pru_shared_t *m_shared;
};

#endif