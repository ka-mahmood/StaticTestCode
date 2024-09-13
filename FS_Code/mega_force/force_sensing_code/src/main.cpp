/**
 * Code for stiffness testing
 * 
**/
#include "HX711.h"
#include <Arduino.h>

float get_calibration_val(HX711);
float* preset_cal_all(HX711[]);

int addition = 20; // addition value due to calibration
 
// HX711 circuit wiring - to digital pins
HX711 LOADCELLS[4]; 
const int LOADCELLS_DOUT[4] = {A0, A2, A4, A6};
const int LOADCELLS_SCK[4] = {A1, A3, A5, A7};
const float TARE_VALUES[4] = {0, 0, 0, 0};
float SCALE_VALUES[4];
float OUT[4];

unsigned long tareTime;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing");

  LOADCELLS[0].begin(LOADCELLS_DOUT[0], LOADCELLS_SCK[0]);
  LOADCELLS[1].begin(LOADCELLS_DOUT[1], LOADCELLS_SCK[1]);
  LOADCELLS[2].begin(LOADCELLS_DOUT[2], LOADCELLS_SCK[2]);
  LOADCELLS[3].begin(LOADCELLS_DOUT[3], LOADCELLS_SCK[3]);


  // ignore the zero value
  SCALE_VALUES[0] = 1; // cal
  SCALE_VALUES[1] = -1711.48; // cal
  SCALE_VALUES[2] = 1; // cal
  SCALE_VALUES[3] = 1; // cal

  Serial.println("Do you want to recalibrate the scale? 0 for no, 1 for S1, 2 for S2, 3 for S3, 4 for S4, 5 for all");
  int recalibrate = 0;

  while (Serial.available() == 0) {
    recalibrate = Serial.parseInt();
  }

  // LOADCELLS[0].set_scale();
  LOADCELLS[0].read_average(10); // blank reads to make sure it tares properly
  LOADCELLS[0].tare(); // tare the value
        Serial.println("1 pass.");
  // LOADCELLS[1].set_scale();
  LOADCELLS[1].read_average(10);
  LOADCELLS[1].tare(); // tare the value
      Serial.println("2 pass");
  // LOADCELLS[2].set_scale();
  LOADCELLS[2].read_average(10);

  LOADCELLS[2].tare(); // tare the value
  // LOADCELLS[3].set_scale();
        Serial.println("3 pass");
  LOADCELLS[3].read_average(10);
  LOADCELLS[3].tare(); // tare the value
      Serial.println("4 pass.");


  if (recalibrate != 0) {

    if (recalibrate != 5) {
    // LOADCELLS[recalibrate-1].read_average(20); // blank read to make sure it is set up
    LOADCELLS[recalibrate-1].set_scale();
    SCALE_VALUES[recalibrate-1] = get_calibration_val(LOADCELLS[recalibrate-1]);
    } else {
      float* scal_val = preset_cal_all(LOADCELLS);

      SCALE_VALUES[0] = scal_val[0]; 
      SCALE_VALUES[1] = scal_val[1];
      SCALE_VALUES[2] = scal_val[2];
      SCALE_VALUES[3] = scal_val[3];

    }

  }

  LOADCELLS[0].set_scale(SCALE_VALUES[0]);
  LOADCELLS[1].set_scale(SCALE_VALUES[1]);
  LOADCELLS[2].set_scale(SCALE_VALUES[2]);
  LOADCELLS[3].set_scale(SCALE_VALUES[3]);

  Serial.println("PRESS ANY NUMBER & ENTER TO START DATA COLLECTION");
  while (Serial.parseInt() != -1) {
    if (Serial.available() > 0) {
      break;
    }
  }

  Serial.println("---------------------------------");
  Serial.println("Live readings:");

  Serial.println("Time (ms), Sensor 1 Force (N), Sensor 2 Force (N), Sensor 3 Force (N), Sensor 4 Force (N)");
  tareTime = millis();
  
}

void loop() {
 
  // note: the readings are printed in a CSV format, the log file can be copied and pasted, then saved as .csv

  Serial.print(millis() - tareTime);
  Serial.print(", ");
  Serial.print((LOADCELLS[0].get_units(5)+TARE_VALUES[0]), 3); 
  Serial.print(", ");
  Serial.print((LOADCELLS[1].get_units(5)+TARE_VALUES[1]), 3);
  Serial.print(", ");
  Serial.print((LOADCELLS[2].get_units(5)+TARE_VALUES[2]), 3);
  Serial.print(", ");
  Serial.println((LOADCELLS[3].get_units(5)+TARE_VALUES[3]), 3);


}

float* preset_cal_all(HX711 sensors[]) {


  float temp_scale_val[4] = {0, 0, 0, 0};

  Serial.print("Tare values: ");
  Serial.print(sensors[0].read());
  Serial.print(", ");
  Serial.print(sensors[1].read());
  Serial.print(", ");
  Serial.print(sensors[2].read());
  Serial.print(", ");
  Serial.println(sensors[3].read());

  // delay to ensure that the load cell is set up
  Serial.println("Loosen the cables to set the tension. Press any number and enter to continue.");
  Serial.println("NOTE: this process will calibrate to Newtons.");

  float pretensions[4] = {-19.553, -27.571, -45.191, -50.933};
  temp_scale_val[0] = sensors[0].get_value(10)/pretensions[0];
  temp_scale_val[1] = sensors[0].get_value(10)/pretensions[1];
  temp_scale_val[2] = sensors[0].get_value(10)/pretensions[2];
  temp_scale_val[3] = sensors[0].get_value(10)/pretensions[3];

  while (Serial.parseInt() != -1) {
    if (Serial.available() > 0) {
      break;
    }
  }

  Serial.print("Calibrating to ");
  Serial.print(pretensions[0]);
  Serial.print(", ");
  Serial.print(pretensions[1]);
  Serial.print(", ");
  Serial.print(pretensions[2]);
  Serial.print(", ");
  Serial.print(pretensions[3]);
  Serial.print(", ");  
  Serial.println(" N...");


  Serial.println("Calibrated values of weight: \t\t");
  Serial.println(sensors[0].get_units(10), 10);
  Serial.println(sensors[1].get_units(10), 10);
  Serial.println(sensors[2].get_units(10), 10);
  Serial.println(sensors[3].get_units(10), 10);

  Serial.print("Calibration complete, save and put in code if required. Set to: \t\t");
  Serial.print(temp_scale_val[0]);
  Serial.print(", ");
  Serial.print(temp_scale_val[1]);
  Serial.print(", ");
  Serial.print(temp_scale_val[2]);
  Serial.print(", ");
  Serial.println(temp_scale_val[3]);
  //19.553, 27.571, 45.191, 50.933

  return (float*) temp_scale_val;

}

float get_calibration_val(HX711 sensor) {

  float temp_scale_val = 0;
  // delay to ensure that the load cell is set up
  Serial.println("Place a 50 N weight onto the load cell, press any number then enter.");
  Serial.println("NOTE: this process will calibrate to Newtons.");

  float mass = 50;

  while (Serial.parseInt() != -1) {
    if (Serial.available() > 0) {
      break;
    }
  }

  Serial.print("Calibrating to ");
  Serial.print(mass);
  Serial.println(" N...");

  Serial.print("Tare value: ");
  float tare_val = sensor.read();
  Serial.println(tare_val);

  Serial.print("Averaged value with weight, minus tare: \t\t");
  temp_scale_val = sensor.get_value(10)/mass;
  sensor.set_scale(temp_scale_val);
  Serial.println(temp_scale_val, 10);
  Serial.print("Calibrated value of weight: \t\t");
  Serial.println(sensor.get_units(10), 10);

  Serial.print("Calibration complete, save and put in code if required. Set to: \t\t");
  Serial.println(temp_scale_val, 10);

  return temp_scale_val;

}