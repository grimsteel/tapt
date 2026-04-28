# Connecting to the board

Flashing, logging, and debug is done over SWD.

SWD connections over OpenOCD have two components:
1. The interface: I'm using a Flipper Zero for my SWD-USB adapter, with DAP-link. This means the interface is `cmsis-dap`
2. The target. This is an nRF52.

To connect, run OpenOCD with these specified: (west uses its own openocd, so stop this when flashing)

```sh
openocd -f interface/cmsis-dap.cfg -c "transport select swd" -f target/nrf52.cfg
```

* GDB can be accessed on localhost:3333
* The OpenOCD console can be accessed on telnet localhost:4444

Before any flashing or debugging can be done, run `nrf52_recover` in the OpenOCD console. (**Important!**)

## Building and Flashing

Building is done with `west`.

Because this is a freestanding project, make sure `ZEPHYR_BASE` is set to your nRF install location. I put mine in `~/.local/share/nrf-sdk/v3.2.2/zephyr/`.

Then, start the toolchain environment: `nrfutil sdk-manager toolchain launch --ncs-version v3.2.2  /bin/bash`

1. `west build`
2. `west flash -r openocd` If you're using a different OpenOCD interface, configure this in boards/tapt/board.cmake

## Debugging

Start OpenOCD as mentioned above.

Then, use the specific version of GDB for the chip (make sure you're in the tolchain environment):

```sh
$ arm-zephyr-eabi-gdb build/firmware/zephyr/zephyr.elf
(gdb) target extended-remote localhost:3333
```

## Logs

Logging is done over RTT to avoid the need for a UART peripheral.

First, configure RTT in the OpenOCD console (telnet localhost:4444):

```
> rtt setup 0x20000410 0x30 "SEGGER RTT"
> rtt server start 9001 0
Listening on port 9001 for rtt connections
> rtt start
rtt: Searching for control block 'SEGGER RTT'
rtt: Control block found at 0x20000410
```

The memory address for `rtt setup` should be that one, but if it doesn't find the control block, grep for `_SEGGER_RTT` in `./build/firmware/zephyr/zephyr.map`.

Logs can then be viewed on `telnet localhost:9001`
