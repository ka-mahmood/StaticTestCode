/**
 * Code for stiffness testing
 * 
**/
#include "HX711.h"
#include <Arduino.h>

float get_calibration_val(HX711);

// HX711 circuit wiring - to digital pins
HX711 LOADCELLS[4]; 
const int LOADCELLS_DOUT[4] = {A0, A2, A4, A6};
const int LOADCELLS_SCK[4] = {A1, A3, A5, A7};
float SCALE_VALUES[4];
float OUT[4];

unsigned long tareTime;

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing");

  LOADCELLS[0].begin(LOADCELLS_DOUT[0], LOADCELLS_SCK[0]);
  // LOADCELLS[1].begin(LOADCELLS_DOUT[1], LOADCELLS_SCK[1]);
  // LOADCELLS[2].begin(LOADCELLS_DOUT[2], LOADCELLS_SCK[2]);
  // LOADCELLS[3].begin(LOADCELLS_DOUT[3], LOADCELLS_SCK[3]);


  SCALE_VALUES[0] = 213196/9.81;
  // SCALE_VALUES[1] = 213196/9.81; 
  // SCALE_VALUES[2] = 213196/9.81; 
  // SCALE_VALUES[3] = 213196/9.81; 

  Serial.println("Do you want to recalibrate the scale? 0 for no, 1 for S1, 2 for S2, 3 for S3, 4 for S4");
  int recalibrate = 0;

  while (Serial.available() == 0) {
    recalibrate = Serial.parseInt();
  }

  LOADCELLS[0].set_scale();
  LOADCELLS[0].tare(); // tare the value
  // LOADCELLS[1].set_scale();
  // LOADCELLS[1].tare(); // tare the value
  // LOADCELLS[2].set_scale();
  // LOADCELLS[2].tare(); // tare the value
  // LOADCELLS[3].set_scale();
  // LOADCELLS[3].tare(); // tare the value

  if (recalibrate != 0) {
    SCALE_VALUES[recalibrate-1] = get_calibration_val(LOADCELLS[recalibrate-1]);
  }

  LOADCELLS[0].set_scale(SCALE_VALUES[0]);
  // LOADCELLS[1].set_scale(SCALE_VALUES[1]/9.81);
  // LOADCELLS[2].set_scale(SCALE_VALUES[2]/9.81);
  // LOADCELLS[3].set_scale(SCALE_VALUES[3]/9.81);

  Serial.println("PRESS ANY CHARACTER & ENTER TO START DATA COLLECTION");
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
  Serial.println(LOADCELLS[0].get_units(15), 10);
  // Serial.print(", ");
  // Serial.print(LOADCELLS[1].get_units(10), 10);
  // Serial.print(", ");
  // Serial.print(LOADCELLS[2].get_units(10), 10);
  // Serial.print(", ");
  // Serial.println(LOADCELLS[3].get_units(10), 10);


}

float get_calibration_val(HX711 sensor) {

    float temp_scale_val = 0;
    // delay to ensure that the load cell is set up
    Serial.println("Place a 1 kg weight onto the load cell, press any character then enter.");
    Serial.println("NOTE: this process will calibrate to Newtons.");

    delay(100);

  float mass = 1;

  while (Serial.parseInt() != -1) {
    if (Serial.available() > 0) {
      break;
    }
  }

  Serial.print("Calibrating to ");
  Serial.print(mass);
  Serial.print(" kg...");

  // while (true) {
  //   Serial.println(sensor.get_units());
  // }

  Serial.print("Averaged value with weight, minus tare: \t\t");
  temp_scale_val = sensor.get_value(10)/mass;
  Serial.println(temp_scale_val, 10);

  delay(100);

  Serial.print("Calibrated value of weight: \t\t");
  Serial.println(sensor.get_units(10), 10);

  Serial.print("Calibration complete, save and put in code if required. Set to: \t\t");
  Serial.println(temp_scale_val, 10);

  return temp_scale_val;

}

// /**
//  *
//  * HX711 library for Arduino - example file
//  * https://github.com/bogde/HX711
//  *
//  * MIT License
//  * (c) 2018 Bogdan Necula
//  *
// **/
// #include "HX711.h"
// #include <Arduino.h>

// // HX711 circuit wiring
// const int LOADCELL_DOUT_PIN = A0;
// const int LOADCELL_SCK_PIN = A1;


// HX711 scale;

// void setup() {
//   Serial.begin(9600);
//   Serial.println("HX711 Demo");

//   Serial.println("Initializing the scale");

//   // Initialize library with data output pin, clock input pin and gain factor.
//   // Channel selection is made by passing the appropriate gain:
//   // - With a gain factor of 64 or 128, channel A is selected
//   // - With a gain factor of 32, channel B is selected
//   // By omitting the gain factor parameter, the library
//   // default "128" (Channel A) is used here.
//   scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

//   Serial.println("Before setting up the scale:");
//   Serial.print("read: \t\t");
//   Serial.println(scale.read());			// print a raw reading from the ADC

//   Serial.print("read average: \t\t");
//   Serial.println(scale.read_average(20));  	// print the average of 20 readings from the ADC

//   Serial.print("get value: \t\t");
//   Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight (not set yet)

//   Serial.print("get units: \t\t");
//   Serial.println(scale.get_units(5), 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided
// 						// by the SCALE parameter (not set yet)

//   scale.set_scale();				        // this value is obtained by calibrating the scale with known weights; see the README for details
//   scale.tare();				        // reset the scale to 0

//   Serial.println("After setting up the scale:");

//   Serial.print("read: \t\t");
//   Serial.println(scale.read());                 // print a raw reading from the ADC

//   Serial.print("read average: \t\t");
//   Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

//   Serial.print("get value: \t\t");
//   Serial.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

//   Serial.print("get units: \t\t");
//   Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
// 						// by the SCALE parameter set with set_scale

//   Serial.println("Readings:");
// }

// void loop() {
//   Serial.print("one reading:\t");
//   Serial.print(scale.get_units(), 10);
//   Serial.print("\t| average:\t");
//   Serial.println(scale.get_units(10), 10);

//   // scale.power_down();			        // put the ADC in sleep mode
//   delay(500);
//   // scale.power_up();
// }