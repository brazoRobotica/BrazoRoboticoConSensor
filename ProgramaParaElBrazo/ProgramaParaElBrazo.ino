#include <Servo.h>

// Definición de los servos
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// Pines para los joysticks
const int pinJoyX1 = A0;
const int pinJoyY1 = A1;
const int pinJoyX2 = A2;
const int pinJoyY2 = A3;

// Pines para los botones de los joysticks (si deseas usarlos)
const int pinJoyButton1 = 9;
const int pinJoyButton2 = 10;
bool buttonPressed = false;   // Almacena si el botón está presionado
bool previousButtonState = HIGH; // Estado anterior del botón para detectar cambios

const int pinTrigger = 12;
const int pinEcho = 11;

const int safeDistance = 10; // Define que tan cerca debe estar el objeto para que el brazo frene.
long duration;
// Ángulos iniciales y objetivos para los servos
int angle1 = 80; // Giro
int angle2 = 80; // Distancia
int angle3 = 10; // Pinza
int angle4 = 80; // Altura

int targetAngle1 = 90;
int targetAngle2 = 80;
int targetAngle4 = 90;

// Umbral para considerar el joystick en reposo
const int threshold = 500;

// Paso de incremento en los ángulos para suavizar el movimiento
const int step = 1;

// Función para mover el servo suavemente
void moveSmoothly(Servo &servo, int &currentAngle, int targetAngle) {
  if (currentAngle < targetAngle) {
    currentAngle += step;
  } else if (currentAngle > targetAngle) {
    currentAngle -= step;
  }
  servo.write(currentAngle);
}
/*
void scanForObject(Servo &servo,Servo &servo1, Servo &servo2,int distancia, int safeDistance, int &currentAngle, int targetAngle){
    servo.write(90);
    servo1.write(90);
    while (distancia > safeDistance){
      if(currentAngle < 80 && currentAngle>=0){
        moveSmoothly(servo2,currentAngle,0);
        moveSmoothly(servo2,currentAngle,80);
      }else{
        moveSmoothly(servo2,currentAngle,160);
        moveSmoothly(servo2,currentAngle,79);
      }
    }
    stop(servo2);
}*/
void setup() {
  // Configurar los botones de los joysticks como entrada
  pinMode(pinJoyButton1, INPUT_PULLUP);
  pinMode(pinJoyButton2, INPUT_PULLUP);
  pinMode(pinTrigger, OUTPUT);
  pinMode(pinEcho, INPUT);
  // Conectar los servos a los pines correspondientes
  servo1.attach(8); //Grio
  servo2.attach(7); //Distancia 
  servo3.attach(6); //Pinza
  servo4.attach(5); //Altura


  // Posición inicial de los servos
  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);
  servo4.write(angle4);

  // Iniciar comunicación serial (opcional para monitoreo)
  Serial.begin(9600);
}

void loop() {
  // Leer valores de los ejes X y Y de ambos joysticks
  int xValue1 = analogRead(pinJoyX1);
  int yValue1 = analogRead(pinJoyY1);
  int xValue2 = analogRead(pinJoyX2);
  int yValue2 = analogRead(pinJoyY2);
  bool buttonState= digitalRead(pinJoyButton2); 

  // Si el valor del joystick se aleja del centro (más allá del umbral), se actualiza el ángulo objetivo
  if (abs(xValue1 - 512) > threshold) {
    targetAngle1 = map(xValue1, 0, 1023, 0, 160);  // Servo 1 controlado por eje X del Joystick 1. Giro
  }
  
  if (abs(yValue1 - 512) > threshold) {
    targetAngle2 = map(yValue1, 0, 1023, 0, 160);  // Servo 2 controlado por eje Y del Joystick 1. Distancia
  }
  if (abs(yValue2 - 512) > threshold) {
    targetAngle4 = map(yValue2, 0, 1023, 0, 160);  // Servo 4 controlado por eje Y del Joystick 2. Altura
  }
  if (buttonState == LOW && previousButtonState == HIGH) {
    buttonPressed = !buttonPressed; // Alterna el estado del botón
    if (angle3 == 50) {
      angle3 = 10;             // Si el ángulo es 50, cambia a 0
    } else {
      angle3 = 50;            // Si no, cambia a 50
    }
    servo3.write(angle3);     // Ajusta el servo al nuevo ángulo
    Serial.print("Ángulo del servo: ");
    Serial.println(angle3);
  }

  previousButtonState = buttonState;
  // Usar la función moveSmoothly() para mover los servos suavemente
  moveSmoothly(servo1, angle1, targetAngle1);
  moveSmoothly(servo2, angle2, targetAngle2);
  moveSmoothly(servo4, angle4, targetAngle4);
  //int distance = measureDistance();
  /*// Opcional: Mostrar el valor de los ángulos en el monitor serial
  Serial.print("Servo1: ");
  Serial.print(angle1);
  Serial.print(" | Servo2: ");
  Serial.print(angle2);
  Serial.print(" | Servo3: ");
  Serial.print(angle3);
  Serial.print(" | Servo4: ");
  Serial.println(angle4);
  */
  // Pausa pequeña para suavizar el movimiento
  delay(20);

}/*
int measureDistance() {
// Primero, generar un pulso corto de 2-5 microsegundos.
    digitalWrite(pinTrigger, LOW);
    delayMicroseconds(5);
    digitalWrite(pinTrigger, HIGH);
// Después de ajustar un nivel de señal alto, esperamos unos 10 microsegundos. En este punto el sensor enviará señales con una frecuencia de 40 kHz.
    delayMicroseconds(10);
    digitalWrite(pinTrigger, LOW);

// Tiempo de retardo de la señal acústica en el sonar.
    duration = pulseIn(pinEcho, HIGH);

// Convertir el tiempo a distancia (cm).
    int distance = (duration / 2) / 29.1;
    return distance;
}

void stop(Servo &servo) {
    servo.detach(); // Deja de mandar señales y frena el servo en su posición actual. 
}*/