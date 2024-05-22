# vette

A diagnostic device to watch ECM parameters and errors from 1992 Chevy Corvette (LT1 engine) via ALDL interface.
Built with ch32v003 chip with sh1106 screen.

You will need [ch32v00fun](https://github.com/cnlohr/ch32v003fun/) and [fun_sh1106](https://github.com/hexaedron/fun_sh1106) to build it. I prefer using official WCH toolchain. If you prefer using vanilla risc-v GCC, then remove `#define WCH_FAST_INTERRUPT_ENABLED` from `cube_defs.h`.

![photo](/doc/photo.jpg)