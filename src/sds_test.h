#include "SdsDustSensor.h"
#include <HardwareSerial.h>

HardwareSerial SdsSerial(1);

// tested on Arduino Leonardo with Serial1
SdsDustSensor sds(SdsSerial); // passing HardwareSerial& as parameter

void setup() {
  Serial.begin(115200);
  SdsSerial.begin(9600, SERIAL_8N1, 25, 26); // 25 rx 26 tx
  //pinMode(25, OUTPUT);
  //digitalWrite(25, 1);

  sds.begin(); // this line will begin Serial1 with given baud rate (9600 by default)

  Serial.println(sds.queryFirmwareVersion().toString()); // prints firmware version
  Serial.println(sds.setQueryReportingMode().toString()); // ensures sensor is in 'query' reporting mode
}

void loop() {
  sds.wakeup();
  delay(10000); // working 30 seconds
  Serial.println("Start sampling ...");

  PmResult pm = sds.queryPm();
  if (pm.isOk()) {
    Serial.print("PM2.5 = ");
    Serial.print(pm.pm25);
    Serial.print(", PM10 = ");
    Serial.println(pm.pm10);

    // if you want to just print the measured values, you can use toString() method as well
    //Serial.println(pm.toString());
  } else {
    Serial.print("Could not read values from sensor, reason: ");
    Serial.println(pm.statusToString());
  }

  WorkingStateResult state = sds.sleep();
  if (state.isWorking()) {
    Serial.println("Problem with sleeping the sensor.");
  } else {
    Serial.println("Sensor is sleeping");
    delay(2000); // wait 1 minute
  }
}