void setup() {
  pinMode(13, OUTPUT); // Définir la broche 13 comme sortie
}

void loop() {
  digitalWrite(13, HIGH); // Allumer la LED
  delay(1000); // Attendre 1 seconde
  digitalWrite(13, LOW); // Éteindre la LED
  delay(1000); // Attendre 1 seconde
}
