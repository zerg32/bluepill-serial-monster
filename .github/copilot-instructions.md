# Copilot Instructions for bluepill-serial-monster

## Project Overview
- **bluepill-serial-monster** is firmware for STM32 Blue Pill and Maple Mini boards, turning them into a 3-port USB-to-Serial adapter.
- Implements a USB 2.0 full-speed composite device with 3 USB CDC interfaces, each mapped to a physical UART.
- No external dependencies except CMSIS; supports DMA RX/TX for high-speed UART.
- Board-specific pin configurations: Blue Pill uses PC13 LED, Maple Mini uses PB2 LED.

## Architecture & Key Files
- **main.c**: Entry point, initializes system and USB CDC interfaces.
- **usb_cdc.c/h**: USB CDC device implementation, manages communication between USB and UARTs.
- **cdc_shell.c/h**: Implements a configuration shell accessible via UART1 for runtime device configuration.
- **device_config.c/h**: Handles persistent device configuration (flash storage, reset, save).
- **gpio.c/h**: GPIO signal management for UART control lines (RTS, CTS, DSR, DTR, DCD, RI, TXA).
- **circ_buf.h**: Circular buffer for UART DMA RX/TX.
- **usb_core.c/h, usb_descriptors.c/h, usb_std.h**: USB stack and descriptors.
- **status_led.c/h**: Status LED control for device state indication.
- **system_clock.c/h, system_interrupts.c/h**: MCU clock and interrupt setup.

## Developer Workflows
- **Build**: Run `make` in the project root. Requires GNU make and arm-none-eabi toolchain in PATH.
- **Flash**: Run `make flash` (uses st-link). For DFU bootloaders, set `FIRMWARE_ORIGIN` variable: `make clean && make FIRMWARE_ORIGIN=0xXXXXXXXX`.
- **Clean**: `make clean` (object/deps), `make distclean` (object/deps/firmware).
- **Configuration Shell**: Connect PB5 to GND and open UART1 with a terminal emulator to access the shell. Use `help` for commands, `config save` to persist changes.

## Project-Specific Patterns & Conventions
- **UART signal mapping**: See README for pinout; 5V-tolerant pins are bolded.
- **Simplified configuration**: Only RX/TX pins used - all control signals (RTS/CTS/DSR/DTR/DCD/RI/TXA) removed to avoid pin conflicts.
- **Windows USB CDC bug**: RTS changes require DTR update (see README for workaround).
- **DMA buffer size**: 1024 bytes for RX/TX.
- **Configuration**: Changes are instant but only persist after `config save`.
- **Board differences**: Pin configurations in `device_config.c` - Blue Pill (PC13 LED, open-drain), Maple Mini (PB2 LED, push-pull).
- **Maple Mini LED conflict resolved**: TXA pins removed, so no conflict with onboard LED (PB1) - firmware uses PB2 for status LED.
- **Maple Mini USB disconnect**: PB9 controls USB disconnect circuit (PB9 low = disconnect, high = connect). Reserved pin, not available for UART use.
- **No external libraries except CMSIS**; all code is bare-metal C for STM32.

## Integration Points
- **USB CDC**: Composite device with 3 CDC interfaces; works with Linux, macOS, Windows (INF driver for WinXP/7/8 in `inf/`).
- **DFU/STM32duino**: Prebuilt binaries for STM32duino bootloader; see README for details.

## Example: UART Parameter Command
```
uart 1 tx output od rts output od active high
```

## References
- See `README.md` for pinout, build, flashing, and configuration details.
- INF driver files for Windows in `inf/` directory.

---

If any section is unclear or missing, please provide feedback to improve these instructions.
