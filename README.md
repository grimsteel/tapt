# tapt

> just a button

`tapt` is a BLE-powered IoT button. It uses an nRF52810 and can be powered for long periods of time on a single CR2032 battery.

It can be connected to Home Assistant and hooked up to automations from there.

## Usage

> Note: do NOT connect the battery when the board is powered off of the debug header.

1. Customize the MAC address and advertisment ID in the firmware if needed.
1. Install the Home Assistant integration (wip)
1. Flash the nRF over SWD, using the SWD header on the board. The pins are labeled on the bottom.

   Firmware is included in the `./firmware` directory.
2. Connect the battery. I soldered on a CR2032 holder to the terminals on the bottom of the PCB.
3. Profit. The button should be autodiscovered in Home Assistant, where it will expose a button entity.

I made this because I wanted a large button for controlling random automations.

## PCB

## Assembly

Apart from the PCB, tapt just has two components: the base and the button. 

![cad render](./case/render.png)

The PCB is screwed into the base, and the button sits inside the base. When pressed down, it presses the limit switch.

[Onshape link](https://cad.onshape.com/documents/617a57c855f6b668c2d4cd24/w/013e725e8d7df515c4261645/e/6602b3eddf1a670923f88f78)

## Bill of Materials

[BOM CSV](./bom.csv)

If the price is 0, I had it already when I was making this.

All components are x2 because 2 PCBA

| Name | Quantity | Link | Price |
| - | - | - | - |
| tapt PCB | 5 | JLCPCB | $2.00 |
| PCBA + Extended components fee | 2 | JLCPCB | |
| Base + button (3d-printed) | 1 | 3d-printed | $0.00 |
| Limit switch | 1 x2 | Aliexpress | $0.00 |
| CR2032 holder | 1 x2 | Aliexpress | $0.00 |
| M3 Heatset inserts | 2 x2 | Aliexpress | $0.00 |
| M3 Screws | 2 x2 | Aliexpress | $0.00 |

