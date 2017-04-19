# Welcome to bGeigie Nano Integrated Version project  
(this file in in process of updating)

**This repository contains the firmware for the new integrated hardware for the bGeigie Nano developed at IRNAS.**

The major difference with a regular bGeigie Nano is that the device is centered
around the [RedBear Duo](https://github.com/redbear/Duo) board that has an ARM
architecture.
For the regular bGeigie Nano based on Arduino, head out [here](https://github.com/Safecast/bGeigieNanoKit).

This is a lighter version of the bGeigie Mini which is meant to fit in a Pelican Micro Case 1010.

# Acknowledgements
This work would not exist without the original development by [bidouilles] (https://github.com/bidouilles/bGeigieNano) and contributions of other [safecast] (https://github.com/Safecast/bGeigieNano) developers.

The bGeigie Nano code for the atmega328p was ported to the RedBear Duo by @fakufaku and @robouden.

# Requirements
* bGeigieNanoKit can be bought online from [Mecom](http://www.nanoxpress.com/)
* for programing/updating software you need a FTDI cable drivers are at http://www.ftdichip.com/Drivers/VCP.htm
* solder iron
* time to make the kit (3-4 hours is normal)


# Build process 

## Using Arduino IDE

After everything is installed in the Arduino IDE:

1. go to `Tools -> Board -> Boards Manager`
2. scroll to RedBear Duo pack
3. Click once so that the button appears
4. Then from dropdown menu on the right select version `2.10.0`
5. press update
6. Wait for the update to finish. (edited)

The it is time to get the bGeigieIntegrated code.

From the terminal:

    git clone https://github.com/fakufaku/bGeigieNanoKit.git bGeigieNano
    cd bGeigieNano
    git checkout RedBearDuo_dev

From the arduino IDE:

* Open `bGeigieNano/bGeigieNano.ino`
* Connect the redbear duo board from the bGeigie to the computer via usb
* From `Tools -> Board` select `RedBear Duo (native USB port)`
* From `Tools -> Serial Port` Select the correct serial port
* Click the compile and upload button (on the sketch window, the right pointing arrow)

## Using the Makefile

TBA

## Using the prebuilt image

TBA

# Assembly

## Pins assignment

# Power consumption

TBA

## Estimation

TBA

## Summary table

TBA

# Usage
Once powered on the bGeigieNano will initiliaze a new log file on the SD card, setup the GPS and start counting the CPM.

# Sample log

    # NEW LOG
    # format=1.0.0nano
    $BNRDD,204,2012-09-20T16:53:58Z,776,63,33895,A,5641.7788,N,1411.8820,E,9861.20,A,109,9*46
    $BNRDD,204,2012-09-20T16:54:03Z,771,61,33956,A,5642.2047,N,1412.9433,E,9862.60,A,109,9*4D
    $BNRDD,204,2012-09-20T16:54:08Z,773,70,34026,A,5642.6305,N,1414.0053,E,9865.00,A,109,9*40
    $BNRDD,204,2012-09-20T16:54:13Z,768,59,34085,A,5643.0562,N,1415.0662,E,9866.80,A,108,9*4D
    $BNRDD,204,2012-09-20T16:54:18Z,765,59,34144,A,5643.4820,N,1416.1277,E,9868.10,A,108,9*4D
    $BNRDD,204,2012-09-20T16:54:23Z,776,70,34214,A,5643.9077,N,1417.1884,E,9870.40,A,90,10*4E
    $BNRDD,204,2012-09-20T16:54:28Z,790,69,34283,A,5644.3330,N,1418.2491,E,9871.30,A,90,10*44
    $BNRDD,204,2012-09-20T16:54:33Z,792,77,34360,A,5644.7576,N,1419.3115,E,9871.40,A,90,10*41
    $BNRDD,204,2012-09-20T16:54:38Z,800,73,34433,A,5645.1819,N,1420.3749,E,9872.60,A,90,10*4C
    $BNRDD,204,2012-09-20T16:54:43Z,784,57,34490,A,5645.6060,N,1421.4371,E,9873.10,A,89,10*4A
    $BNRDD,204,2012-09-20T16:54:48Z,787,58,34548,A,5646.0298,N,1422.4998,E,9874.10,A,89,10*40
    $BNRDD,204,2012-09-20T16:54:53Z,792,76,34624,A,5646.4534,N,1423.5620,E,9874.80,A,98,9*73
    $BNRDD,204,2012-09-20T16:54:58Z,804,75,34699,A,5646.8769,N,1424.6242,E,9874.30,A,98,9*74
    $BNRDD,204,2012-09-20T16:55:03Z,808,65,34764,A,5647.3011,N,1425.6873,E,9877.10,A,98,9*7F
    $BNRDD,204,2012-09-20T16:55:08Z,793,55,34819,A,5647.7236,N,1426.7514,E,9876.30,A,89,10*49
    $BNRDD,204,2012-09-20T16:55:13Z,799,65,34884,A,5648.1464,N,1427.8159,E,9876.10,A,98,9*7F
    $BNRDD,204,2012-09-20T16:55:18Z,795,55,34939,A,5648.5688,N,1428.8810,E,9877.80,A,98,9*7B
    $BNRDD,204,2012-09-20T16:55:23Z,774,49,34988,A,5648.9898,N,1429.9472,E,9878.50,A,85,10*46
    $BNRDD,204,2012-09-20T16:55:28Z,768,63,35051,A,5649.4098,N,1431.0137,E,9877.40,A,224,8*4A
    $BNRDD,204,2012-09-20T16:55:33Z,754,63,35114,A,5649.8297,N,1432.0798,E,9876.30,A,223,8*4F
    $BNRDD,204,2012-09-20T16:55:38Z,752,71,35185,A,5650.2496,N,1433.1447,E,9873.20,A,106,9*4C
    $BNRDD,204,2012-09-20T16:55:43Z,753,58,35243,A,5650.6698,N,1434.2092,E,9871.30,A,106,9*40
    $BNRDD,204,2012-09-20T16:55:48Z,765,70,35313,A,5651.0904,N,1435.2737,E,9869.50,A,106,9*4B

# Notes

Nothing here yet.

# Licenses
 * [InterruptHandler and bGeigieMini code][5] - Copyright (c) 2011, Robin Scheibler aka FakuFaku
 * [TinyGPS][6] - Copyright (C) 2008-2012 Mikal Hart
 * bGeigieNano - Copyright (c) 2012, Lionel Bergeret
 * [Makefile][8] - Copyright (c) 2012, Tim Marston


  [1]: https://github.com/sparkfun/OpenLog "OpenLog"
  [2]: https://www.adafruit.com/products/661 "Monochrome 128x32 OLED"
  [3]: https://www.sparkfun.com/products/10116 "Arduino Fio"
  [4]: http://www.lndinc.com/products/17/ "LND-7317"
  [5]: https://github.com/fakufaku/SafecastBGeigie-firmware "SafecastBGeigie-firmware"
  [6]: http://arduiniana.org/libraries/tinygps/ "TinyGPS"
  [7]: https://www.adafruit.com/products/746 "Ultimate GPS"
  [8]: http://ed.am/dev/make/arduino-mk "Arduino Makefile"
  [9]: http://www.pelican.com/cases_detail.php?Case=1010 "Pelican 1010"
