#include "pructrl.h"
#include <iostream>

int main()
{
    std::cout << "PRU ARM-side control stub\n";

    PruCtrl ctrl;
    ctrl.setEnable(true);
    ctrl.setHalfPeriodCycles(500000);
    ctrl.setGpioMask(0x1);

    ctrl.printConfig();

    std::cout << "\nNext step:\n";
    std::cout << "  Connect this config to real PRU shared memory on the BBB.\n";

    return 0;
}