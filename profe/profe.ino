//Hacer un programa en arduino que mueva un brazo robótico con 4 servos y que tenga un sensor de proximidad que cuando detecte algo a menos de 20 cm, se dirijo al objeto abra la pina y levante el objeto. Dejándolo en alguno de sus laterales.

#include <Servo.h>

// Definición de pines
const int trigPin = 11;
const int echoPin = 12;

// Definición de los servos
Servo servoBase;
Servo servoBrazo1;
Servo servoBrazo2;
Servo servoPinza;


// Definición de los pines de los servos
const int servoBasePin = 8;
const int servoBrazoAlturaPin = 7;
const int servoPinzaPin = 6;
const int servoBrazoDistanciaPin = 5;

// Variables de distancia y ángulo
long duration;
int distance;

void setup() {
  // Inicialización de la comunicación serie
  Serial.begin(9600);

  // Inicialización de los pines del sensor ultrasónico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Inicialización de los servos
  servoBase.attach(servoBasePin);
  servoBrazo1.attach(servoBrazoAlturaPin);
  servoBrazo2.attach(servoBrazoDistanciaPin);
  servoPinza.attach(servoPinzaPin);

  // Inicializar los servos a la posición inicial
  servoBase.write(90);
  servoBrazo1.write(90);
  servoBrazo2.write(90);
  servoPinza.write(20); // Pinza abierta
}

void loop() {
  // Lógica de detección de proximidad y manipulación del objeto
  if (detectObject()) {
    // Dirigirse al objeto
    moveToObject();

    // Abrir pinza
    servoPinza.write(20); // Abrir pinza
    delay(500);

    // Levantar objeto
    liftObject();

    // Mover el objeto a uno de los laterales
    moveToSide();

    // Soltar objeto
    servoPinza.write(40); // Cerrar pinza
    delay(500);
  }
  
  delay(500);
}

bool detectObject() {
  // Envia el pulso de trigger
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Lee la señal de echo
  duration = pulseIn(echoPin, HIGH);
  
  // Calcula la distancia
  distance = duration * 0.034 / 2;
  
  // Si la distancia es menor a 20 cm, se considera que se detectó un objeto
  return (distance < 5); // Ajusta el umbral según sea necesario
}

void moveToObject() {
  // Ajusta los ángulos de los servos para mover el brazo hacia el objeto
  // Estos valores son ejemplos y deben ser ajustados según tu configuración
  servoBase.write(45);
  delay(1000);
  servoBrazo1.write(30);
  delay(1000);
  servoBrazo2.write(30);
  delay(1000);
}

void liftObject() {
  // Levanta el objeto ajustando los ángulos de los servos
  // Estos valores son ejemplos y deben ser ajustados según tu configuración
  servoBrazo1.write(60);
  delay(1000);
  servoBrazo2.write(60);
  delay(1000);
}

void moveToSide() {
  // Mueve el brazo hacia un lateral para soltar el objeto
  // Estos valores son ejemplos y deben ser ajustados según tu configuración
  servoBase.write(135);
  delay(1000);
}
