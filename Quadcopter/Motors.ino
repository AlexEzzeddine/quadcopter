
Servo motor1;
Servo motor2;
Servo motor3;
Servo motor4;

void motors_initialize(){
	motor1.attach(PIN_MOTOR0);
	motor2.attach(PIN_MOTOR1);
	motor3.attach(PIN_MOTOR2);
	motor4.attach(PIN_MOTOR3);
	motor1.writeMicroseconds(MOTOR_ZERO_LEVEL);
	motor2.writeMicroseconds(MOTOR_ZERO_LEVEL);
	motor3.writeMicroseconds(MOTOR_ZERO_LEVEL);
	motor4.writeMicroseconds(MOTOR_ZERO_LEVEL);
}

void motors_arm(){
	motor1.writeMicroseconds(MOTOR_ZERO_LEVEL);
	motor2.writeMicroseconds(MOTOR_ZERO_LEVEL);
	motor3.writeMicroseconds(MOTOR_ZERO_LEVEL);
	motor4.writeMicroseconds(MOTOR_ZERO_LEVEL);
}

void motors_send_throttle()
{
	motor1.writeMicroseconds(throttle1);
	motor2.writeMicroseconds(throttle2);
	motor3.writeMicroseconds(throttle3);
	motor4.writeMicroseconds(throttle4);
}
