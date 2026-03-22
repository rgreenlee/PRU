#ifndef PRUCTRL_H
#define PRUCTRL_H

#include <cstdint>

struct PruConfig
{
    std::uint32_t enable;
    std::uint32_t halfPeriodCycles;
    std::uint32_t gpioMask;
    std::uint32_t reserved;
};

class PruCtrl
{
public:
    PruCtrl();

    void setEnable(bool on);
    void setHalfPeriodCycles(std::uint32_t cycles);
    void setGpioMask(std::uint32_t mask);

    void printConfig() const;

private:
    PruConfig m_config;
};

#endif