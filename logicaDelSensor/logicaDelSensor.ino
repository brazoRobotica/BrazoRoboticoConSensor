const int pinTrigger = 12;
const int pinEcho = 11;

const int safeDistance = 10; // Define que tan cerca debe estar el objeto para que el brazo frene.
long duration;

void setup() {
    pinMode(pinTrigger, OUTPUT);
    pinMode(pinEcho, INPUT);
}

void loop() {
    int distance = measureDistance();
    Serial.print("Distancia al objeto: ");
    Serial.print(distance);

// Frena el brazo si el objeto está dentro de la distancia segura
    if (distance < safeDistance) {
        stop(servoX);
        while (true);
    }
}

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
}

int measureDistance() {
// Primero, generar un pulso corto de 2-5 microsegundos.
    digitalWrite(pinTrigger, LOW);11
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
}