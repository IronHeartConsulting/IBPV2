// Adafruit Ultimate GPS module using MTK33x9 chipset
// http://www.adafruit.com/products/746
// Arduino Leonardo

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

#include "stations.h"

SoftwareSerial gps_serial(8, 7);
Adafruit_GPS GPS(&gps_serial);

#define GPSECHO false

volatile boolean ticked = false;

// Clicks goes from 0-180 because there is a 3-minute beacon schedule.
volatile byte clicks = 0;

volatile byte next_tx_on_time = 255;

volatile boolean disciplining_milliclock = false;
volatile int last_millis = 0;
volatile int millis_per_second = 0;
int seven_fifty_millis = 750;

// tick interrupt
// keep track of clicks; GPS will discipline clicks after interrupt disable.
void tick() {
  clicks = (clicks+1) % (3*60);
  if (clicks == next_tx_on_time) {
    txon();
  }

  if (disciplining_milliclock) {
    int t = last_millis;
    last_millis = millis();
    millis_per_second = millis() - t;
  }

  ticked = true;
}

void setup()  
{

  // connect at 115200 so we can read the GPS fast enough and echo without dropping chars
  // also spit it out
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println(F("NCDXC/IARU Beacon IBPV2"));

  // PPS interrupt from GPS on pin 3 (Int.0) on Arduino Leonardo
  pinMode(3, INPUT_PULLUP);         // PPS is 2.8V so give it pullup help
  attachInterrupt(0, tick, RISING); // tick happens 0.5s after GPS serial sends the time.

  // LED
  pinMode(13, OUTPUT);             // LED

  // GPS Setup 
  {
    GPS.begin(9600);

    // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
    // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

    // minimum recommended sentence only
    GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);

    GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate

    // GPS.sendCommand(PGCMD_ANTENNA);
    delay(1000);

    // Ask for firmware version
    // gps_serial.println(PMTK_Q_RELEASE);
  }

  do {
    Serial.println(F("*** GPS Discipline clock"));
  } while (! gps_discipline_clock(131072));
  Serial.println(F("*** GPS Locked "));

  Serial.println(F("*** GPS Discipline Milliclock"));
  {
    gps_begin_milliclock_discipline(); 
    delay(5000);
    gps_end_milliclock_discipline();
  }
  Serial.println(F("*** Milliclock disciplined "));
}

void loop()
{
  boolean tocked = false;
  if (ticked) {
    ticked = false;
    tocked = true;
  }

  if (tocked) {
    handle_tick();
  }

}



