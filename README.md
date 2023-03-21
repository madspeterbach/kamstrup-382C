# kamstrup-382C

This is a custom component for ESPhome - to read energy consumption from a Kamstrup 382C energy meter.

My Kamstrup 382C only seems to support basic IEC 1107 mode A at 300 bps, 7e1, see https://github.com/lvzon/dsmr-p1-parser/blob/master/doc/IEC-62056-21-notes.md

After sending the signon sequence "/?!\r\n", my meter responds with:
```
/KAM 68438BC450000
0.0.0(14410000)
1.8.0(0036886*kWh)
1.8.1(0036886*kWh)
1.8.2(0000000*kWh)
1.8.3(0000000*kWh)
1.8.4(0000000*kWh)
96.8.0(0119899*h)
0.128.1(0000000)
1.6.0(1.078*kW)
```
The /KAM line identifies the manufacturer (Kamstrup). If the device support mode C (changing speed), the character after the KAM sequence would show the speed supported (e.g. 5 for 9600 bps).

The codes are OBIS codes, see https://onemeter.com/docs/device/obis/

1.8.0 seems to be "active energy has been consumed"
1.8.1 seems to be "active energy has been consumed in tarif zone 1"

It seems tarif zones 2-4 is unused here.

96.8.0 is the "Time of operation" - in hours. I.e. this meter has been in operation for close to 14 years.

1.6.0 is the current power draw.

So far, not working, unsure if configuration, hw or sw. 

Let's try with an IR eye from the volkszäler, since they're available on eBay:

https://wiki.volkszaehler.org/hardware/controllers/ir-schreib-lesekopf-rs232-ausgang
https://www.ebay.de/itm/354359025520

Shamelessly stolen from https://github.com/bipsendk/kamstrup-382Lx7:
-----

Code inspired from https://github.com/kquinsland/ws3-to-esphome-bridge - and mostly based on https://github.com/Hal9k-dk/kamstrup/tree/master/Software%20eksempler/kamstrup_powermeter

I have this code running on a NodeMCU with an ESP32-Wroom module.
I have tried it on an ESP8266, where ESPhome should use SoftwareSerial - I was not able to get it to work.
