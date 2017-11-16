double pid_roll_in,   pid_roll_out,   pid_roll_setpoint = 0;
double pid_pitch_in,  pid_pitch_out,  pid_pitch_setpoint = 0;
double pid_yaw_in,    pid_yaw_out,    pid_yaw_setpoint = 0;

PID roll_controller(&pid_roll_in,   &pid_roll_out,  &pid_roll_setpoint,  ROLL_PID_KP, ROLL_PID_KI, ROLL_PID_KD, DIRECT);
PID pitch_controller(&pid_pitch_in, &pid_pitch_out, &pid_pitch_setpoint, PITCH_PID_KP, PITCH_PID_KI, PITCH_PID_KD, DIRECT);
PID yaw_controller(&pid_yaw_in,     &pid_yaw_out,   &pid_yaw_setpoint,   YAW_PID_KP, YAW_PID_KI, YAW_PID_KD, DIRECT); 


void pid_initialize() {
	roll_controller.SetOutputLimits(ROLL_PID_MIN,ROLL_PID_MAX);
	pitch_controller.SetOutputLimits(PITCH_PID_MIN,PITCH_PID_MAX);
	yaw_controller.SetOutputLimits(YAW_PID_MIN,YAW_PID_MAX);
	roll_controller.SetMode(AUTOMATIC);
	pitch_controller.SetMode(AUTOMATIC);
	yaw_controller.SetMode(AUTOMATIC);
	roll_controller.SetSampleTime(10);
	pitch_controller.SetSampleTime(10);
	yaw_controller.SetSampleTime(10);
}

void pid_update(){
	pid_roll_in = angles[2];
	pid_pitch_in = angles[1];
	pid_yaw_in = angles[0]; 
}

void pid_compute() {
	roll_controller.Compute();
	pitch_controller.Compute();
	yaw_controller.Compute();
}

void setpoint_update() {
	// here we allow +- 20 for noise and sensitivity on the RX controls...
	// ROLL rx at mid level?
	if(receiver_values[0] > THROTTLE_RMID - 20 && receiver_values[0] < THROTTLE_RMID + 20)
		pid_roll_setpoint = 0;
	else
		pid_roll_setpoint = map(receiver_values[0],ROLL_RMIN,ROLL_RMAX,ROLL_WMIN,ROLL_WMAX);
	//PITCH rx at mid level?
	if(receiver_values[1] > THROTTLE_RMID - 20 && receiver_values[1] < THROTTLE_RMID + 20)
		pid_pitch_setpoint = 0;
	else
		pid_pitch_setpoint = map(receiver_values[1],PITCH_RMIN,PITCH_RMAX,PITCH_WMIN,PITCH_WMAX);
	//YAW rx at mid level?
	if(receiver_values[3] > THROTTLE_RMID - 20 && receiver_values[3] < THROTTLE_RMID + 20)
		pid_yaw_setpoint = 0;
	else
		pid_yaw_setpoint = map(receiver_values[3],YAW_RMIN,YAW_RMAX,YAW_WMIN,YAW_WMAX);
}

void control_update(){
	throttle = receiver_values[2];
	
	setpoint_update();
	pid_update();
	pid_compute();
	
	// yaw control disabled for stabilization testing...
	throttle1 = throttle + pid_pitch_out - pid_roll_out;//+ pid_yaw_out;
	throttle2 = throttle - pid_pitch_out - pid_roll_out;//- pid_yaw_out;
	throttle3 = throttle - pid_pitch_out + pid_roll_out ;//+ pid_yaw_out;
	throttle4 = throttle + pid_pitch_out + pid_roll_out;//- pid_yaw_out;
	#ifdef SAFE
		if(throttle < THROTTLE_SAFE_SHUTOFF)
		{
			throttle1 = throttle2 = throttle3 = throttle4 = MOTOR_ZERO_LEVEL;
		}
	#endif
}
