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

// Pines para los botones de los joysticks
const int pinJoyButton1 = 9;
const int pinJoyButton2 = 10;
bool buttonPressed = false;   // Almacena si el botón está presionado
bool previousButtonState = HIGH; // Estado anterior del botón para detectar cambios

// Ángulos iniciales y objetivos para los servos
int angle1 = 90;
int angle2 = 80;
int angle3 = 15;
int angle4 = 80;

int targetAngle1 = 90;
int targetAngle2 = 80;
int targetAngle4 = 90;

// Umbral para considerar el joystick en reposo
const int threshold = 300;

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

unsigned long previousMillis = 0; // Variable para almacenar el tiempo de la última actualización
const long interval = 20;  // Intervalo de tiempo en milisegundos para actualizar los servos

void setup() {
  // Configurar los botones de los joysticks como entrada
  pinMode(pinJoyButton1, INPUT_PULLUP);
  pinMode(pinJoyButton2, INPUT_PULLUP);
  
  // Conectar los servos a los pines correspondientes
  servo1.attach(8);
  servo2.attach(7);
  servo3.attach(6);
  servo4.attach(5);

  // Posición inicial de los servos
  servo1.write(angle1);
  servo2.write(angle2);
  servo3.write(angle3);
  servo4.write(angle4);

  // Iniciar comunicación serial (opcional para monitoreo)
  Serial.begin(9600);
}

void loop() {
  // Leer los valores de los joysticks
  int xValue1 = analogRead(pinJoyX1);
  int yValue1 = analogRead(pinJoyY1);
  int xValue2 = analogRead(pinJoyX2);
  int yValue2 = analogRead(pinJoyY2);
  bool buttonState = digitalRead(pinJoyButton2); 

  // Comprobar si se ha presionado el botón para alternar el ángulo del servo 3
  if (buttonState == LOW && previousButtonState == HIGH) {
    buttonPressed = !buttonPressed; // Alterna el estado del botón
    if (angle3 == 50) {
      angle3 = 15;  // Si el ángulo es 50, cambia a 15
    } else {
      angle3 = 50;  // Si no, cambia a 50
    }
    servo3.write(angle3);  // Ajusta el servo al nuevo ángulo
    Serial.print("Ángulo del servo 3: ");
    Serial.println(angle3);
  }

  previousButtonState = buttonState;

  // Si el valor del joystick se aleja del centro (más allá del umbral), se actualiza el ángulo objetivo
  if (abs(xValue1 - 512) > threshold) {
    targetAngle1 = map(xValue1, 0, 1023, 0, 160);  // Servo 1 controlado por eje X del Joystick 1
  }
  
  if (abs(yValue1 - 512) > threshold) {
    targetAngle2 = map(yValue1, 0, 1023, 0, 160);  // Servo 2 controlado por eje Y del Joystick 1
  }

  if (abs(yValue2 - 512) > threshold) {
    targetAngle4 = map(yValue2, 0, 1023, 25, 160);  // Servo 4 controlado por eje Y del Joystick 2
  }

  // Mover los servos suavemente
  moveSmoothly(servo1, angle1, targetAngle1);
  moveSmoothly(servo2, angle2, targetAngle2);
  moveSmoothly(servo4, angle4, targetAngle4);
  moveSmoothly(servo3, angle3, angle3);  // Movemos servo 3 también de manera suave

  // Pausar el ciclo principal para evitar que se ejecute demasiado rápido
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Opcional: Mostrar los valores de los servos en el monitor serial
    /*
    Serial.print("Servo1: ");
    Serial.print(angle1);
    Serial.print(" | Servo2: ");
    Serial.print(angle2);
    Serial.print(" | Servo3: ");
    Serial.print(angle3);
    Serial.print(" | Servo4: ");
    Serial.println(angle4);
    */
  }
}
