# Electronics build

This is the first version of the dash electronics.

## Background

The Tenere's OEM dash is meant to mimic a rally-style setup, but the UI is lacking, both in terms of display and operation with the single select button. Important (to me) information is buried, hard to access or reset, or simply not available at all.

The overall intent of this is to gather relevant information, display it in a more readable fashion, and interact with it.

## Overview

There are three primary components to this:
 - Controllers and sensors to collect and process data
 - Display to actually show the data
 - Rotary encoders to control the whole system
 
Separately, there is a bar controller with a few switches that functions as a keyboard. This interfaces with the tablet, not the discrete electronics.
 
In its current state, the only sensor being used is the GPS in order to provide heading and trip meters. The USB hub allows the tablet to use the external GPS in lieu of its own, internal GPS. The NEO-M9N GPS is more accurate and has a higher refresh rate than the tablet's GPS. The bar controller also goes through the hub to the tablet.

Future plans include:
 - CAN integration to display bike data
 - Data logging

## Components

These are essentially off-the-shelf components consisting mostly of [Adafruit](https://www.adafruit.com/) and [SparkFun](https://www.sparkfun.com/) products and are interconnected via I2C with the STEMMA/Qwiic interface. This is a breakdown of all the individual components grouped by their actual physical location:

### Microcontrollers / Sensors

This is the box on the side of the tower that houses the majority of the electronics.

- [Electronics Enclosure Assembly](enclosure_electronics)
  - [Enclosure base](enclosure_electronics/enclosure_electronics_base) - Includes mounting posts for: 
    - [RP2040 CAN Feather](https://www.adafruit.com/product/5724)
    - [CoolGear CG-3590X4AX1C-PD-PCBA USB hub](https://www.coolgear.com/product/4-port-usb-3-2-gen-1-micro-powered-hub-pcba-w-gl3590-chipset-esd-surge-protection)
    - [IMU](https://www.adafruit.com/product/4502)
  - [Enclosure lid](enclosure_electronics/enclosure_electronics_lid) - Includes mounting posts for: 
    - [NEO-M9N GPS](https://www.sparkfun.com/products/17285)
  - [Enclosure gasket](enclosure_electronics/enclosure_electronics_gasket)

### Display / Antenna

The displays are mounted to the GPS bar clamp along with the GPS antenna.

- [Display Enclosure Assembly](enclosure_display)
  - [Enclosure base](enclosure_display/enclosure_display_base) - Includes mounting posts for: 
    - [Seven segment backpack and displays](https://www.adafruit.com/product/878)
    - [NeoPixel Stick 8](https://www.adafruit.com/product/1426)
  - [Enclosure lid](enclosure_display/enclosure_display_lid)
  - [Enclosure gasket](enclosure_display/enclosure_display_gasket)
  - [Enclosure lens](enclosure_display/enclosure_display_lens)
- [Enclosure mounting bracket](enclosure_display/bracket_display)
- [Antenna mounting bracket](enclosure_display/bracket_antenna)
  - [ANN-MB-00 GPS Antenna](https://www.mouser.com/ProductDetail/u-blox/ANN-MB-00?qs=CiayqK2gdcLD35zjsrMkTw%3D%3D&srsltid=AfmBOopE4NwvK-NwnYeN03ZKSZuy5o31hP3ZSBpxcxob4BpL1pjlFRlus)

### Rotary Encoders

- [Encoder Enclosure Assembly](enclosure_encoder)
  - [Enclosure base](enclosure_encoder/enclosure_encoder_base) - Includes mounting posts for: 
    - [I2C Active Terminator](https://www.adafruit.com/product/4756)
  - [Enclosure lid](enclosure_encoder/enclosure_encoder_lid) - Includes cutouts to mount:
    - [NeoRotary 4](https://www.adafruit.com/product/5752)
  - [Enclosure gasket](enclosure_encoder/enclosure_encoder_gasket)

### Bar Controller

- [Bar Controller Assembly](bar_controller)
  - [Bar controller base](bar_controller/bar_controller_base)
  - [Bar controller lid](bar_controller/bar_controller_lid) - Includes space to mount:
    - [KB2040](https://www.adafruit.com/product/5302)
  - [Bar controller gasket](bar_controller/bar_controller_gasket)

The boards are connected via an I2C bus to the Arduino. The GPS is also connected to the USB hub both to power everything and to pass data to the tablet more accurate location data to the tablet.

## Is this necessary?

About as necessary as any other mod people do to their bikes.

## Background

When I initially began spitballing a tower, I wanted to completely exclude the OEM dash. While this is 98% feasible, the odometer and trip meter are calculated and stored in the OEM dash. Because of this, while virtually everything about the bike is easily available from the CAN bus with a cheap dongle, these values aren't because they never leave the OEM dash. The trip meter in particular is really important to me because I'm normally a frequent user of it while riding offroad, but the Tenere's UI makes them unusable for this purpose.

This is something that I've wanted to have accessible, along with a more accurate GPS. I know there are app-based solutions for this, but they don't work exactly how I would like and they don't work with my navigation setup; I'm looking for dedicated displays. The intent is to have an initial setup similar to this:

![Electronics package](images/dash_concept.jpg)