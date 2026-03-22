#include "pructrl.h"
#include <iostream>

PruCtrl::PruCtrl()
{
    m_config.enable = 0;
    m_config.halfPeriodCycles = 1000000;
    m_config.gpioMask = 0x1;
    m_config.reserved = 0;
}

void PruCtrl::setEnable(bool on)
{
    m_config.enable = on ? 1U : 0U;
}

void PruCtrl::setHalfPeriodCycles(std::uint32_t cycles)
{
    m_config.halfPeriodCycles = cycles;
}

void PruCtrl::setGpioMask(std::uint32_t mask)
{
    m_config.gpioMask = mask;
}

void PruCtrl::printConfig() const
{
    std::cout << "PRU Config:\n";
    std::cout << "  enable            = " << m_config.enable << "\n";
    std::cout << "  halfPeriodCycles  = " << m_config.halfPeriodCycles << "\n";
    std::cout << "  gpioMask          = 0x" << std::hex << m_config.gpioMask << std::dec << "\n";
}