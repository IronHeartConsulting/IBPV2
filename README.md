IBPV2
=====

*DRAFT*

International Beacon Program - V2.0

http://www.ncdxf.org/pages/beacons.html
http://www.ncdxf.org/beacon/schematic.pdf
http://www.ncdxf.org/beacon/source.txt

The purpose of the NCDXF/IARU HF Beacon is to allow worldwide amateur
radio operators to judge HF propagation at your location, for your
local time.  To use it, tune in the beacons, and if you can hear a
beacon, you might be able to work some HF DX to that area of the
world.

The v1.0 beacon equipment consists of a HF Vertical antenna, Kenwood TS50
transceiver, a Trimble Acutime GPS unit, and a custom controller built
around a 8748 micro controller.

The network has been on the air for about 10 years. The current
problem is the equipment is getting old, and dying.  The original
designers never expected the units to remain in the field 10 years.

# HF V2.0 Beacon Network 

The goal is to design a replacement equipment package that duplicates
the existing features.  There has been a lot of proposals for more
features, other ways, etc.  But, none of those proposing have been
willing to put any work into it.

The aim is a practical design, with few or no new features.




# Equipment 

## Minimum Equipment
- ICOM IC-7200 Transceiver
- New TX Antenna
- GPS
- Switches
- LCD
- Rig Interface board
- Power Supply
- Filtering and isolation
- Chassis

## Additional possibilities
- Backup RTC with TXC (DS3231 or http://www.adafruit.com/products/255)
- Network: WiFi or Ethernet for command & control, status

# Functionality 

Each beacon transmits every three minutes, day and night.

A transmission consists of the callsign of the beacon sent at 22 words
per minute followed by four one-second dashes. The callsign and the
first dash are sent at 100 watts. The remaining dashes are sent at 10
watts, 1 watt and 100 milliwatts.

The beacons cycle thru the bands 14 Mhz, 18 MHz, 21 MHz, 24 MHz and 28 MHz.

Each station is given a specific time slot, such that only one
frequency is needed (on 20m, it’s 14.100).  The unit uses GPS to get
an accurate time, to synchronize it’s timeslot.  The beacon transmits
on each band for ten seconds, once every three minutes.

- Check switch for test sqeuence.
- Monitor the GPS unit - get the current time of day, and monitor the 1PPS signal.  
- Compute the correct timeslot for the station
- Check switches for band enable/disable.
- Send commands via  CI-V / CAT to the radio to select the frequency, and power level via commands
- Send the callsign and long dashes in CW.

This table gives the minute and second of the start of the first
transmission within the hour for each beacon on each frequency, and
reports the status.

http://www.ncdxf.org/beacon/beaconschedule.html

## Test Mode

Below is the v1.0 test mode

The test sequence is used for adjusting the power levels and checking
the keying waveforms.  Control the power level and request
either a string of dots or continual keying by using the switches
which normally disable particular bands:

| Effect    | Switch  |
| --------- | ------- |
| 100 watts | 28 MHz  |
| 10 watts  | 24 MHz  |
| 1 watt    | 21 MHz  |
| 0.1 watt  | 18 MHz  |
| Dits      | 14 MHz  |
| Carrier   | 10 MHz  |


# Equipment and Components Details 

## ICOM IC-7200 Transceiver

The ICOM IC-7200 transceiver offers CI-V commands for control of frequency, transmissions, and for monitoring.

For the CI-V commands see: http://www.plicht.de/ekki/civ/civ-p4.html

Statistics could also be gathered: for example, SWR could be measured.

## New TX Antenna

There is likely no impact of antenna choice on this part of the v2.0 beacon project.

## GPS

The GPS data stream will tell us the correct time of day.  Once we
know that, and our slot ID, we can just init a countdown register to
our time slot.

## Switches

The v1.0 system has the following switches:

- Band Switches (6)
- Test Switches (1 used, 3 reserved)

The band switches control which bands are used.  v1.0 covers 5 bands
with a 6th slot reserved for 10 MHz.

One test switch controls test mode, and in test mode, the band switches
have other functions.  Three other switches are reserved.

## LCD

The v1.0 system has a single LED for showing GPS and other status.

Switching to a small LCD character display will make operation easier
and will not greatly increase cost.  An i2c LCD is probably the best
choice.  

There are combination switch/LCD I2C products that we can use and
avoid the switch overloading by using screen feedback.  For example,
http://www.adafruit.com/products/772

Or we could use the Akafugu LCD backpack and use a separate mechanism for
switches.  This product has the advantage that it is mechanically easier
to mount in case.  https://store.akafugu.jp/products/26


## Arduino

- The Arduino Uno will be standard and remain easily understood for years.
- The Arduino Leonardo has the advantage of a separate programming port and hardware TTL port for the GPS.  Software TTL for CI-V rig control.
- The Arduino Mega has the disadvantage of being off the Arduino main line, and users often have trouble with support for hardware and libraries.
- The Arduino Yun is a combination of the Arduino Leonardo, a small Linux system running OpenWRT, and integrated Wifi connection, and an Ethernet connection.

Tentative Recommendations:
- The Arduino Leonardo is probably the best choice for the non-network version
- The Arduino Yun is the best choice for the connected version.

## Interface Board

The interface board is an Arduino Shield.  It has headers for
connections to the IC-7200 CI-V TTL serial, IC-7200 CW line, GPS TTL
serial, LCD, and possibly RTC.

## Optional item: Network: WiFi or Ethernet for command & control, status

The sites are mostly hosted.  If we had a USB connection to a local
PC, then we could send the host a file to load new code on the
controller, and we could also collect stats, etc.

If there is a network (WiFi or wired) we could also report the statistics,
such as whether it's online, what the SWR is.

## Optional item: Backup RTC with TXC

It might be nice to include a quartz crystal based time module, to
keep it disciplined with the GPS, in case there is some loss of GPS
signal.

Backup disciplined clock for use when GPS isn't available, for example, indoors.

- DS3231 board
- http://www.adafruit.com/products/255

# Engineering Resources 

- Project Management: Kevin K6TD
- Arduino system: Leigh WA5ZNU
- Programming: Leigh WA5ZNU
- Systems Design: ?
- Hardware Design: ?
- Testing: ?
