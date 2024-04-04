#include <Arduino_LPS22HB.h>

float referencePressure; // Pression de référence à l'emplacement de l'initialisation

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!BARO.begin()) {
    Serial.println("Failed to initialize pressure sensor!");
    while (1);
  }

  Serial.println("Pressure sensor initialized.");

  // Capturer la pression de référence à l'emplacement de l'initialisation
  referencePressure = BARO.readPressure();
}

void loop() {
  float pressure = BARO.readPressure(); // Lire la pression en Pa
  float altitude = pressureToAltitude(pressure, referencePressure); // Convertir la pression en altitude en mètres

  Serial.print("Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  delay(500);
}

float pressureToAltitude(float pressure, float referencePressure) {
  // Utiliser la formule de conversion pression-altitude en utilisant la pression de référence
  // Dans cet exemple, nous utilisons une formule simple pour la démonstration
  return ((float)44330 * (1 - pow((pressure / referencePressure), 0.1903)));
}
