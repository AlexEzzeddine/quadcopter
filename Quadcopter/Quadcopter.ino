#include "Configuration.h"
#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>
#include <PID_v1.h>
#include <Servo.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"


float angles[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

uint16_t receiver_values[RC_NUM_CHANNELS];

int throttle;
int throttle1;
int throttle2;
int throttle3;
int throttle4;

void setup() {
	Serial.begin(115200);

	motors_initialize();
	receiver_initialize();
	pid_initialize();
	sensor_initialize();
	motors_arm();
}

void loop() {
	/*
	Serial.print("Throttle: \t");
	Serial.print(throttle1);
	Serial.print("\t");
	Serial.print(throttle2);
	Serial.print("\t");
	Serial.print(throttle3);
	Serial.print("\t");
	Serial.println(throttle4);
	*/

  
  Serial.print("ypr\t");
  Serial.print(angles[2] * 180/M_PI);
  Serial.print("\t");
  Serial.print(angles[1] * 180/M_PI);
  Serial.print("\t");
  Serial.println(angles[0] * 180/M_PI);
  

  /*Serial.print("CH1:"); Serial.print(receiver_values[PIN_RX_ROLL - RX_PINS_OFFSET]); Serial.print("\t");
  Serial.print("CH2:"); Serial.print(receiver_values[PIN_RX_PITCH - RX_PINS_OFFSET]); Serial.print("\t");
  Serial.print("CH3:"); Serial.print(receiver_values[PIN_RX_THROTTLE - RX_PINS_OFFSET]); Serial.print("\t");
  Serial.print("CH4:"); Serial.println(receiver_values[PIN_RX_YAW - RX_PINS_OFFSET]);*/
	receiver_read_values();
	sensor_read_angles();
	control_update();
	motors_send_throttle();
}
