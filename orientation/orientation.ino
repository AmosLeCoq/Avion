#include "Arduino_BMI270_BMM150.h"

float ax, ay, az; // Variables for accelerometer data
float gx, gy, gz; // Variables for gyroscope data
float mx, my, mz; // Variables for magnetometer data

int degreesX = 0;
int degreesY = 0;

unsigned long previousTime = 0; // Previous time for time interval calculation
float elapsedTime = 0; // Time interval between readings
float previousGyroX = 0; // Previous gyro reading for integration
float previousGyroY = 0; // Previous gyro reading for integration

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
}

void loop() {
  unsigned long currentTime = millis(); // Current time
  elapsedTime = (currentTime - previousTime) / 1000.0; // Time interval in seconds
  previousTime = currentTime; // Update previous time

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);

    // Integration to calculate orientation on the X axis
    float gyroX_delta = gx * elapsedTime; // Change in angle since last reading
    degreesX += gyroX_delta; // Add change to total angle

    // Integration to calculate orientation on the Y axis
    float gyroY_delta = gy * elapsedTime; // Change in angle since last reading
    degreesY += gyroY_delta; // Add change to total angle

    // Print orientation in degrees for both X and Y axes
    Serial.print("Orientation X: ");
    Serial.print(degreesX);

    Serial.print(" :Y: ");
    Serial.println(degreesY);

    delay(100); // Adjust delay according to your needs
  }
}
