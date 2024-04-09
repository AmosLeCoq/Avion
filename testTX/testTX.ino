#define PIN_RECEIVER 2  // Définissez le numéro de broche où est connecté le récepteur
#define ALPHA 1 // Facteur de lissage de l'EMA

float filteredValue = 0; // Valeur filtrée

void setup() {
  Serial.begin(9600);  // Démarre la communication série
  pinMode(13, OUTPUT); // Définir la broche 13 comme sortie
}

void loop() {
  int pulseWidth = pulseIn(PIN_RECEIVER, HIGH);  // Lit la largeur d'impulsion
  filteredValue = ALPHA * pulseWidth + (1 - ALPHA) * filteredValue;  // Calcul de la nouvelle valeur filtrée
  // Utilisez filteredValue pour les opérations ultérieures, par exemple :
  if (filteredValue > 1500) {
    digitalWrite(13, HIGH); // Allumer la LED
    Serial.println("Bouton activé !");
  }
  delay(500);  // Attend un court instant
  digitalWrite(13, LOW); // Éteindre la LED
}