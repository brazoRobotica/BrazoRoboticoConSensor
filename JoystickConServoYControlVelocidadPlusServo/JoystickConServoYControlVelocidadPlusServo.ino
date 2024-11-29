#include <Servo.h>

/*
GND - GND
Vcc - 5v
VRx - A0
VRy - A1
SW -  D9
*/

// Definición de los servos
Servo servoBase; // Giro del brazo
Servo servoAltura; // Distancia
Servo servoDistancia; // Pinza
Servo servoPinza; // Altura

// Pines para los joysticks
const int pinJoyX1 = A0;  
const int pinJoyY1 = A1;
const int pinJoyX2 = A2;
const int pinJoyY2 = A3;

// Pines para los botones de los joysticks
const int pinJoyButton1 = 9;
const int pinJoyButton2 = 10;
// Pines para el sensor
const int pinEcho = 11;
const int pinTrigger = 12;

bool buttonPressed1 = false;
bool buttonPressed2 = false;   // Almacena si el botón está presionado
bool previousButtonState1 = HIGH;
bool previousButtonState2 = HIGH; // Estado anterior del botón para detectar cambios

// Ángulos iniciales y objetivos para los servos
int angleBase = 90;
int angleAltura = 90;
int angleDistancia = 90;
int anglePinza = 20;
const int safeDistance = 12; // Define que tan cerca debe estar el objeto para que el brazo frene.

// Umbral para considerar el joystick en reposo
const int umbral = 300;
// Paso de incremento en los ángulos para suavizar el movimiento
const int pasos = 10;
// Valor de reposo
const int valorReposo = 520;
// Delay para la lectura(ms)
const int retraso = 5;
// Limites de los ejes
const int limiteMaxBase = 170;  // izquierdo
const int limiteMinBase = 10;   // derecho
const int limiteMaxAltura = 130; // min ?
const int limiteMinAltura = 60;  // max ?
const int limiteMaxDistancia = 150; 
const int limiteMinDistancia = 60;

void setup() {
  // Configurar los botones de los joysticks como entrada.
  pinMode(pinJoyButton1, INPUT_PULLUP); // Función de busqueda
  pinMode(pinJoyButton2, INPUT_PULLUP); // Pinza
  
  // Pines del sensor
  pinMode(pinTrigger, OUTPUT);
  pinMode(pinEcho, INPUT);

  // Conectar los servos a los pines correspondientes
  servoBase.attach(8); // Giro
  servoDistancia.attach(7); // Distancia
  servoPinza.attach(6); // Pinza
  servoAltura.attach(5); // Altura

  // Posición inicial de los servos
  servoBase.write(angleBase);
  servoDistancia.write(angleDistancia);
  servoPinza.write(anglePinza);
  servoAltura.write(angleAltura);

  // Iniciar comunicación serial (opcional para monitoreo)
  Serial.begin(9600);
}

void loop() {
  // Leer los valores de los joysticks
  int xValue1 = analogRead(pinJoyX1);
  delay(retraso);
  int yValue1 = analogRead(pinJoyY1);
  delay(retraso);
  int xValue2 = analogRead(pinJoyX2);
  delay(retraso);
  int yValue2 = analogRead(pinJoyY2);
  delay(retraso);
  bool buttonState1 = digitalRead(pinJoyButton1); 
  bool buttonState2 = digitalRead(pinJoyButton2); 

  // Comprobar si se ha presionado el botón para alternar el ángulo del servo 3
  if (buttonState2 == LOW && previousButtonState2 == HIGH) {
    buttonPressed2 = !buttonPressed2; // Alterna el estado del botón
    if (anglePinza == 55) {
      anglePinza = 15;  // Si el ángulo es 50, cambia a 15. Angulo de apertura
    } else {
      anglePinza = 55;  // Si no, cambia a 50. Angulo de cierre.
    }
  }
  // Función de busqueda
  if (buttonState1 == LOW && previousButtonState1 == HIGH) {
    buttonPressed1 = !buttonPressed1;
    scanForObject(servoBase, servoAltura, servoPinza, safeDistance);
  }

  if(abs(yValue1 - valorReposo) > umbral) {
    if(yValue1 > valorReposo && angleBase <= limiteMaxBase) {
      angleBase += pasos;
    } else if(yValue1 < valorReposo && angleBase >= limiteMinBase) {
      angleBase -= pasos;
    }
  }
  if(abs(xValue1 - valorReposo) > umbral) {
    if(xValue1 > valorReposo && angleAltura <= limiteMaxAltura) {
      angleAltura += pasos;
    } else if(xValue1 < valorReposo && angleAltura >= limiteMinAltura) {
      angleAltura -= pasos;
    }
  }
  if(abs(xValue2 - valorReposo) > umbral) {
    if(xValue2 < valorReposo && angleDistancia <= limiteMaxDistancia) {
      angleDistancia += pasos;
    } else if(xValue2 > valorReposo && angleDistancia >= limiteMinDistancia) {
      angleDistancia -= pasos;
    }
  }

  servoBase.write(angleBase);
  servoDistancia.write(angleDistancia);
  servoAltura.write(angleAltura);
  servoPinza.write(anglePinza);
  delay(5);
/*
  //Para ver todos los valores por serial.
  Serial.print("ServoBase: ");
  Serial.print(angleBase);
  Serial.print(" | ServoAltura: ");
  Serial.print(angleAltura);
  Serial.print(" | ServoDistancia: ");
  Serial.print(angleDistancia);
  Serial.print(" | ServoPinza: ");
  Serial.print(anglePinza);
  Serial.print(" | x1: ");
  Serial.print(xValue1);
  Serial.print(" | y1: ");
  Serial.print(yValue1);
  Serial.print(" | B1: ");
  Serial.print(buttonState1);
  Serial.print(" | x2: ");
  Serial.print(xValue2);
  Serial.print(" | y2: ");
  Serial.print(yValue2);
  Serial.print(" | B2: ");
  Serial.println(buttonState2);
*/
}

int measureDistance() {
// Primero, generar un pulso corto de 2-5 microsegundos.
  digitalWrite(pinTrigger, LOW);
  delayMicroseconds(5);
  digitalWrite(pinTrigger, HIGH);
// Después de ajustar un nivel de señal alto, esperamos unos 10 microsegundos. En este punto el sensor enviará señales con una frecuencia de 40 kHz.
  delayMicroseconds(10);
  digitalWrite(pinTrigger, LOW);
// Tiempo de retardo de la señal acústica en el sonar.
  int duration = pulseIn(pinEcho, HIGH);
// Convertir el tiempo a distancia (cm).
  int distance = (duration / 2) / 29.1;
  return distance;
}

void scanForObject(Servo &servoBase, Servo &servoAltura, Servo &servoPinza, int safeDistance) {
  bool noHayObjetoDetectado = true;
  // Setear el brazo en la posición de scaneo
  servoBase.write(10);
  servoAltura.write(170);
  while (noHayObjetoDetectado) {
    /*int distance = measureDistance();
    if (distance < safeDistance) {
      Serial.println("Objeto detectado");
      servoBase.detach();
      delay(500);
      break;
    }*/
    int distance = measureDistance();
    // Movimiento del brazo en 180 grados
    for (int angle = 10; angle <= 170; angle += 10) {
    // falta ver que cuando termina el for igualmente agarra un objeto por mas que lo detecte, falta una funcion para lo de abajo del for
    // en el primer llamado, detecta distancia 0 y agarra un objeto
      angleBase = angle;
      Serial.println(angle);
      servoBase.write(angleBase);
      delay(1000);

      int distance = measureDistance();
      Serial.print("Distancia:");
      Serial.println(distance);
      delay(500);

      if (distance < safeDistance) {
        Serial.println("Objeto detectado");
        //servoBase.detach();
        delay(500);
        noHayObjetoDetectado = false;
        break;
      }
    }/*
    for (int angle = 170; angle <= 10; angle -= 10) {
      angleBase = angle;
      servoBase.write(angleBase);
      delay(1000);
    }*/
  }
  //servoBase.attach(8); // Volver a mandar señal al servo
  // Mover el brazo a la posición de agarrar el objeto
  servoAltura.write(45); // Ajustar el brazo a la altura del objeto
  delay(1000);
  servoPinza.write(50); // Apretar la pinza del brazo
  delay(1000);
  // Rotar el brazo a la posición de soltar el objeto
  servoBase.write(10); // Girar a la derecha
  delay(1000);
  servoPinza.write(20); // Soltar la pinza del brazo
  delay(1000);
  // Resetear el brazo a la posición de scaneo
  servoBase.write(10);
  servoAltura.write(170);
}

/*
Idea (Ger)
for (int angle = 10; angle <= 170 and noHayObjetoDetectado; angle += 10)
Para que salga del for y del while sin hacer el break.
*/

/*
void stop(Servo &servo) {
    servo.detach(); // Deja de mandar señales y frena el servo en su posición actual. 
}
*/