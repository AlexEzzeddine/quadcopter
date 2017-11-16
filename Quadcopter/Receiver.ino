volatile int signal_start_times[4];
uint8_t latest_interrupted_pin;

volatile uint16_t receiver_shared[RC_NUM_CHANNELS];

void receiver_read_values() {
	noInterrupts();
	memcpy(receiver_values, (const void *)receiver_shared, sizeof(receiver_shared));
	interrupts();
}

void receiver_initialize() {
	pinMode(PIN_RX_ROLL, INPUT);
	pinMode(PIN_RX_PITCH, INPUT);
	pinMode(PIN_RX_THROTTLE, INPUT);
	pinMode(PIN_RX_YAW, INPUT);
// Serial.print("hello receiver\n");
	enableInterrupt(PIN_RX_ROLL, &receiver_signal_start, RISING);
	enableInterrupt(PIN_RX_PITCH, &receiver_signal_start, RISING);
	enableInterrupt(PIN_RX_THROTTLE, &receiver_signal_start, RISING);
	enableInterrupt(PIN_RX_YAW, &receiver_signal_start, RISING);
}

void receiver_signal_start()
{
 // Serial.print("hello\n");
	latest_interrupted_pin = arduinoInterruptedPin;
	disableInterrupt(latest_interrupted_pin);
	enableInterrupt(latest_interrupted_pin, &receiver_signal_end, FALLING);
	signal_start_times[latest_interrupted_pin - RX_PINS_OFFSET] = micros();
}

void receiver_signal_end()
{
	latest_interrupted_pin = arduinoInterruptedPin;
	disableInterrupt(latest_interrupted_pin);
	enableInterrupt(latest_interrupted_pin, &receiver_signal_start, RISING);
	receiver_shared[latest_interrupted_pin - RX_PINS_OFFSET] = micros() - signal_start_times[latest_interrupted_pin - RX_PINS_OFFSET];
}
