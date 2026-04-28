# Copyright (c) 2021 Nordic Semiconductor ASA
# SPDX-License-Identifier: Apache-2.0

board_runner_args(jlink "--device=nrf52" "--speed=4000")
board_runner_args(pyocd "--target=nrf52810" "--frequency=4000000")

set(OPENOCD_NRF5_SUBFAMILY "nrf52")
set(OPENOCD_NRF5_INTERFACE "cmsis-dap")

include(${ZEPHYR_BASE}/boards/common/nrfjprog.board.cmake)
include(${ZEPHYR_BASE}/boards/common/jlink.board.cmake)
include(${ZEPHYR_BASE}/boards/common/pyocd.board.cmake)
include(${ZEPHYR_BASE}/boards/common/openocd-nrf5.board.cmake)

#board_set_sim_runner_ifnset(example)
#board_set_flasher_ifnset(openocd)
board_finalize_runner_args(example)
