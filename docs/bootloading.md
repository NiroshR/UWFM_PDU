# Bootloading the Arduino Blue Pill

The Arduino Blue Pill does not ship with a bootloader installed, this means 
we cannot easily connect and upload builds to it using the attached USB type-B
connector. To allow us to use a USB connection instead of flashing builds using
the ST-Link V2, this doc will setup a bootloader which allows us to connect to 
the Arduino using just the Arduino IDE.

## Getting Started

- **THE FIRST THING YOU SHOULD DO** is verify with a team member if there is a
  bootloader already installed on the Arduino Blue Pill. There is **no need** 
  to flash the bootloader more than once

- If the bootloader is already installed skip to 
  [Arduino IDE Setup](#arduino-ide-setup)

## Materials

### Arduino Blue Pill
<img src="images/blue-pill.JPEG" width="512">

** The pins on the right hand side are manually bent upwards to prevent 
interference between the Arduino Blue Pill and the relay on the PDU.

### ST-Link V2 
<img src="images/st-link-v2.JPEG" width="512">

You will also need four jumper wires to connect to the Blue Pill.

## Bootloader Setup

To start we will need to get the following:

- [ST-Link V2 driver](https://www.st.com/en/development-tools/stsw-link009.html)
- [STM32CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html)
- Download the bootloader binary on [GitHub](https://github.com/rogerclarkmelbourne/STM32duino-bootloader/blob/master/binaries/generic_boot20_pc13.bin) or in the 
[resources](resources) folder

You will be asked to provide your name and email to download the content from
the ST website. Once you've downloaded and installed all the sofware and 
drivers, we will need to move the `BOOT0` select from `0` to `1`. This simply
involved pulling out the top yellow selector, and moving it to the right hand
pins so they bridge the middle and right pins.

<img src="images/board-features.jpg" width="512">

## STM32CubeProgrammer 

1. Open STM32CubeProgrammer, on the right hand side select `ST-LINK` from the 
   blue drop down then select `Connect` 
2. Select the `Erasing & Programming` tab on the left-hand side of the program
3. Select the file path to the bootloader binary and ensure the starting 
   address is `0x08000000`
4. Program the Blue Pill by clicking `Start Programming`
   ([screenshot](images/stm32cubeprogrammer.png))

If all goes well, you should see a dialog box saying 
`"File download complete"`. The bootloader is now on the Blue Pill. Now that
we're done, **MOVE THE `BOOT0` bridge back to the `0` position**. It will not
show up properly when plugged into a USB port on your PC otherwise.

## Arduino IDE Setup

Once you have **moved the `BOOT0` bridge back to the `0` position**, plug in 
the Blue Pill using a type-B USB cable into your PC.


1. Grab the latest stable release of the 
   [Arduino IDE](https://www.arduino.cc/en/software) and install it on your
   machine. **DO NOT install the Microsoft Store version**, instead download
   the executable file directly off the website.
   
2. Under `File >> Preferences` click on the button to the right of the
   `Additional Boards Manager URLs:` textbox and add the following on a new 
   line: `http://dan.drown.org/stm32duino/package_STM32duino_index.json`
   ([screenshot](images/ide-json-preferences.png))

3. Under `Tools >> Board >> Board Manager` choose the `Contributed` type from the 
   drop down and then search for `"stm32f1"`. Here you will see a package from
   stm32duino. We have only tested this procedure for `v2021.5.31`.
   ([screenshot](images/install-packages.png))

## Blue Pill Drivers

We will now need to install Blue Pill drivers and COM drivers to communicate 
over USB to the Arduino.

1. Download the ZIP file of the Arduino_STM32 GitHub
   [repo](https://github.com/rogerclarkmelbourne/Arduino_STM32)

2. Extract the files, rename the extracted folder to `Arduino_STM32`

3. Move the folder to `C:\Users\<User_Account>\Documents\Arduino\hardware`
   if the `hardware` folder does not exist, create it. The final directory
   structure should look as follows:

```
Arduino
├── hardware
│   └── Arduino_STM32
│     ├── LICENSE
│     ├── README.md
│     ├── STM32F1
│     ├── STM32F4
│     ├── drivers
│     └── tools
└── libraries
    └── readme.txt
```

4. In the `<Arduino Path>\hardware\Arduino_STM32\drivers\win` directory, 
   run the `install_drivers.bat` and `install_STM_COM_drivers.bat` files by
   executing them in an admin Command Prompt manually or by right clicking the
   file and selecting `Run as administrator`. Install them one by one. Now 
   **restart your desktop**.

5. Reconnect the Blue Pill using your USB cable. If its already attached,
   disconnect and reconnect. If a popup comes up saying to run `wdi-simple`
   allow it to run.

6. Restart your Arduino IDE.

## Configuring Arudino Blue Pill in Arduino IDE

1. Open your Arduino IDE, open the `Tools` dropdown from the menu bar.

2. Select the appropriate board by selecting
   `Board: <"Something"> >> STM32F1 Boards (Arduino_STM32 >> Generic STM32F103C series`
   ([screenshot](images/board-selection.png))

3. Verify the following settings in the same `Tools` menu:

```
Board: Generic STM32F103C series
Variant: STM32F103C8 (20k RAM, 64k Flash)
Upload Method: STM32duino bootloader
CPU Speed(MHz): 72Mhz (Normal)
Optimize: Smallest (default)
Port: COM<N> (Maple Mini)
```

## Running a Sample Program

1. Open the built-in `Blink` example from 
   `File >> Examples >> 01.Basics >> Blink`.

2. Click the verify button - the check mark icon on the top left of the IDE

3. Click the upload button - the right hand arrow on the top left of the IDE.
   A [screenshot](images/successful-output.png) of what a successful output 
   should look like is attached.

## References
- [Programming STM32 Board Blue Pill using native USB port](https://www.youtube.com/watch?v=e0CP2IpIBg4&ab_channel=IOTES)