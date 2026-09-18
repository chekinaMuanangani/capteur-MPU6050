#include <Servo.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

Servo servoHautBas; // Pin 9
Servo servoGaucheDroite; // Pin 10

// --- Variables pour le calcul du temps ---
unsigned long tempsPrecedent;
float dt;

// --- Positions des Servos ---
float angleZ_Giro = 90.0;
int dernierAngleX = 90;
int dernierAngleZ = 90;

// --- Paramètres de réglage ---
const int seuilMouvement = 2;
const float sensibiliteGyro = 131.0;

void setup() {
Serial.begin(9600);
Wire.begin();
mpu.initialize();

servoHautBas.attach(9);
servoGaucheDroite.attach(10);

// Initialisation au centre
servoHautBas.write(90);
servoGaucheDroite.write(90);

tempsPrecedent = millis();
}

void loop() {
int16_t ax, ay, az, gx, gy, gz;
mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

// 1. Calcul du delta temps
unsigned long tempsActuel = millis();
dt = (tempsActuel - tempsPrecedent) / 1000.0;
tempsPrecedent = tempsActuel;

// --- AXE VERTICAL (Haut / Bas) ---
float angleX = atan2(ax, az) * 180 / PI;
int cibleX = map(constrain(angleX, -40, 40), -40, 40, 50, 130);

// --- AXE HORIZONTAL (Gauche / Droite) ---
float vitesseZ = gz / sensibiliteGyro;

if (abs(vitesseZ) > 0.8) {
angleZ_Giro += vitesseZ * dt;
}

// Correction ici : la ligne était mal terminée dans ton erreur
angleZ_Giro = constrain(angleZ_Giro, 40, 140);
int cibleZ = (int)angleZ_Giro;

// --- ENVOI AUX SERVOMOTEURS ---
if (abs(cibleX - dernierAngleX) >= seuilMouvement) {
servoHautBas.write(cibleX);
dernierAngleX = cibleX;
}

if (abs(cibleZ - dernierAngleZ) >= seuilMouvement) {
servoGaucheDroite.write(cibleZ);
dernierAngleZ = cibleZ;
}

// Debugging
Serial.print("X: "); Serial.print(cibleX);
Serial.print(" | Z: "); Serial.println(cibleZ);

delay(10);
}