# GPS and Timing and Communications

Use the https://github.com/mikalhart/TinyGPS/releases/tag/v13 library.

Which GPS board gives us guarantees that the NMEA sentence is over before the corresponding PPS happens, and gives us enough processing time?

Event loop starts at whatever time the Arduino boots, which could be in the middle of some long dash with no ID, so a software watchdog makes sure TX doesn't happen if there's been no ID on a band.

Can we use Hardware GPS to talk to the rig and then avoid any disturbances of millis() at all?  Or will the PPS interrupt still make millis() unreliable?
We have to discipline millis() or at least calculate the correct value for 750ms anyway, so maybe it's not worth it.

If we use Serial1 on the Leonardo to talk to the rig, then we lose any chance of using the Arduino YUN for network remote control.  Serial on Leonardo is USB-only, and Serial1 is pins 0-1.

# UI
UI input shouldn't happen when we're transmitting.  It won't interfere with the millis() or non-TX times, which is 2/3 of the minutes.
UI output might happen when we're in the TX minute, but we can avoid that as well if it's expensive.
Need to decide what the UI status to show is anyway.  Time, station ID, band?  Skipped bands?  Test mode / live mode.  SWR?  GPS number of sats and lock for setup? 
Power on time? Battery backup voltage?

For UI input, do we let it program the start minute and second or do we just program in the callsign and let that govern the start minute and second?  

# Rig
We can read the millis() just after the TX serial command and then millis() will be correctly incrementing during that time since there's no Serial output going on; then after 750ms we turn it off and wait for the next PPS to start again.  Don't depend on the millis() value across PPS ticks, just for sub-second timing.

Do we add a flag that says PPS is inaccurate during Serial output, so that discipline can know not to discipline during that time?  It taints the whole second.  Maybe it's just handled by the current strategy of using the event-time circle to avoid discipline at all during TX minutes.

Can we read the RIG SWR after transmit or do we have to read it during transmit?  If it's during transmit that means we have to do more serial IO and that can interfere with our precise 750ms timing.

# Beacon Receivers
What are the exact timings that the beacon programs rely on?  Do they know the length of the long dashes?  How about the start times, do they expect exact PPS times or are they not sensitive (i.e., will this be an improvement)?  How about the CW ID speed?  Does it read that?

# hardware
try to use generic hardware as much as possible so it can be replaced.  stuff that will be made in quantity for a while.
