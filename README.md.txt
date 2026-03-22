# PRU Learning Project

This project is for learning development on the TI PRU, mainly on the BeagleBone Black.

## Initial goal

Version 1 will implement:

- deterministic PRU square-wave generation
- simple shared configuration structure
- ARM-side control stub
- documentation and file structure for later expansion

## Planned progression

1. PRU square-wave output
2. PRU PWM with adjustable duty cycle
3. ARM-to-PRU control
4. encoder / pulse capture
5. fixed-rate sample generation or capture
6. ARM-side FFT processing
7. Qt display later

## Project structure

- `firmware/` - PRU firmware source
- `arm_app/` - Linux-side control application
- `docs/` - architecture and workflow docs
- `scripts/` - deployment notes and helper ideas
