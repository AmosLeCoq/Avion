#include <SPI.h>
#include <SD.h>
#include "Arduino_BMI270_BMM150.h"
#include <Arduino_LPS22HB.h>

File myFile;

float referencePressure; // Pression de référence à l'emplacement de l'initialisation

float ax, ay, az; // Variables for accelerometer data
float gx, gy, gz; // Variables for gyroscope data
float mx, my, mz; // Variables for magnetometer data

int degreesX = 0;
int degreesY = 0;

unsigned long previousTime = 0; // Previous time for time interval calculation
float elapsedTime = 0; // Time interval between readings
float previousGyroX = 0; // Previous gyro reading for integration
float previousGyroY = 0; // Previous gyro reading for integration

const char* file1 = "alt.txt";
const char* file2 = "position.txt"; 

// Fonction pour initialiser la carte SD
void initSD() {
  Serial.print("Initializing SD card...");
  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
}

// Fonction pour écrire dans le fichier
void writeFile(const char* fileName, const char* data) {
  myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    myFile.println(data);
    myFile.close();
  } else {
    Serial.print("error opening ");
    Serial.println(fileName);
  }
}

void position() {
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

    // Write orientation data to file
    char buffer[20];
    sprintf(buffer, "%d,%d", degreesX, degreesY);
    writeFile(file2, buffer);

    delay(50); // Adjust delay according to your needs
  }
}

void alt() {
  float pressure = BARO.readPressure(); // Lire la pression en Pa
  float altitude = pressureToAltitude(pressure, referencePressure); // Convertir la pression en altitude en mètres

  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  // Convertir la valeur float en String
  char buffer[20];
  sprintf(buffer, "%.2f", altitude);

  // Imprimer l'altitude dans le fichier
  writeFile(file1, buffer);
}

void setup() {
  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }

  Serial.println("Pressure sensor initialized.");

  // Capturer la pression de référence à l'emplacement de l'initialisation
  referencePressure = BARO.readPressure();

  // Open serial communications and wait for port to open:
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

  // Initialiser la carte SD
  initSD();

  // Écrire dans le fichier
  writeFile(file1, "----------------------------------------------------------------");
  writeFile(file2, "----------------------------------------------------------------");

  // Lire le contenu du fichier
  //readFile(file);
}

float pressureToAltitude(float pressure, float referencePressure) {
  // Utiliser la formule de conversion pression-altitude en utilisant la pression de référence
  // Dans cet exemple, nous utilisons une formule simple pour la démonstration
  return ((float)44330 * (1 - pow((pressure / referencePressure), 0.1903)));
}

void loop() {
  position();
  alt();
}
