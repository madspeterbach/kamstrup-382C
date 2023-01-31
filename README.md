# kamstrup-382Lx7

This is a custom component for ESPhome - to read energy consumption from a Kamstrup 382C energy meter.

Let's try with an IR eye from the volkszäler, since they're available on eBay:

https://wiki.volkszaehler.org/hardware/controllers/ir-schreib-lesekopf-rs232-ausgang
https://www.ebay.de/itm/354359025520

Shamelessly stolen from https://github.com/bipsendk/kamstrup-382Lx7:
-----

Code inspired from https://github.com/kquinsland/ws3-to-esphome-bridge - and mostly based on https://github.com/Hal9k-dk/kamstrup/tree/master/Software%20eksempler/kamstrup_powermeter

I have this code running on a NodeMCU with an ESP32-Wroom module.
I have tried it on an ESP8266, where ESPhome should use SoftwareSerial - I was not able to get it to work.
