//#include "MPU6050.h" // not necessary if using MotionApps include file
MPU6050 sensor;

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint8_t fifoBuffer[64]; // FIFO storage buffer

Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector

volatile bool mpuInterrupt = false;

void dmpDataReady() {
	mpuInterrupt = true;
}

void sensor_initialize() {
	Wire.begin();
	Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties

	while (!Serial); // wait for Leonardo enumeration, others continue immediately

	// initialize device
	Serial.println(F("Initializing I2C devices..."));
	sensor.initialize();
	pinMode(INTERRUPT_PIN, INPUT);

	// verify connection
	Serial.println(F("Testing device connections..."));
	Serial.println(sensor.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

	// wait for ready
	Serial.println(F("\nSend any character to begin DMP programming and demo: "));
	while (Serial.available() && Serial.read()); // empty buffer
	while (!Serial.available());                 // wait for data
	while (Serial.available() && Serial.read()); // empty buffer again

	// load and configure the DMP
	Serial.println(F("Initializing DMP..."));
	devStatus = sensor.dmpInitialize();

	// supply your own gyro offsets here, scaled for min sensitivity
	sensor.setXGyroOffset(220);
	sensor.setYGyroOffset(76);
	sensor.setZGyroOffset(-85);
	sensor.setZAccelOffset(1788); // 1688 factory default for my test chip

	// make sure it worked (returns 0 if so)
	if (devStatus == 0) {
		// turn on the DMP, now that it's ready
		Serial.println(F("Enabling DMP..."));
		sensor.setDMPEnabled(true);

		// enable Arduino interrupt detection
		Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
		enableInterrupt(INTERRUPT_PIN, &dmpDataReady, RISING);
		mpuIntStatus = sensor.getIntStatus();

		// set our DMP Ready flag so the main loop() function knows it's okay to use it
		Serial.println(F("DMP ready! Waiting for first interrupt..."));
		dmpReady = true;

		// get expected DMP packet size for later comparison
	} else {
		// ERROR!
		// 1 = initial memory load failed
		// 2 = DMP configuration updates failed
		// (if it's going to break, usually the code will be 1)
		Serial.print(F("DMP Initialization failed (code "));
		Serial.print(devStatus);
		Serial.println(F(")"));
	}
}

void sensor_read_angles()
{
	if (!dmpReady) return;
	if (mpuInterrupt && sensor.dmpPacketAvailable())
	{
		mpuInterrupt = false;
		sensor.getFIFOBytes(fifoBuffer, 42);
		sensor.dmpGetQuaternion(&q, fifoBuffer);
		sensor.dmpGetGravity(&gravity, &q);
		sensor.dmpGetYawPitchRoll(angles, &q, &gravity);
	}
}
