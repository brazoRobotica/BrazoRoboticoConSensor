const int pinTrigger = 12;
const int pinEcho = 11;

const int safeDistance = 10; // Define que tan cerca debe estar el objeto para que el brazo frene y lo agarre.

void setup() {
    pinMode(pinTrigger, OUTPUT);
    pinMode(pinEcho, INPUT);
}

void loop() {}

void scanForObject(Servo &servoX, Servo &servoY, Servo &servoZ, int safeDistance, int &currentAngle, int targetAngle) {
    // Setear el brazo en la posición de scaneo
    servoX.write(90);
    servoY.write(90);
    while (true) {
        int distance = measureDistance();
        if (distance < safeDistance) {
            Serial.println("Objeto detectado");
            stop(servoX);
            delay(500);
            break;
        }
        // Movimiento del brazo en 180 grados
        if (currentAngle <= 80) {
            moveSmoothly(servoX, currentAngle, 160);
        } else {
            moveSmoothly(servoX, currentAngle, 0);
        }
    }
    servoX.attach(1); // Volver a mandar señal al servo
    // Mover el brazo a la posición de agarrar el objeto
    servoY.write(45); // Ajustar el brazo a la altura del objeto
    delay(500);
    servoZ.write(30); // Apretar la pinza del brazo
    delay(500);
    // Rotar el brazo a la posición de soltar el objeto
    servoX.write(90 - 90); // Girar a la izquierda
    delay(1000);
    servoZ.write(90); // Soltar la pinza del brazo
    delay(500);
    // Resetear el brazo a la posición de scaneo
    servoY.write(90);
    servoZ.write(90);
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
    duration = pulseIn(pinEcho, HIGH);

// Convertir el tiempo a distancia (cm).
    int distance = (duration / 2) / 29.1;
    return distance;
}

void stop(Servo &servo) {
    servo.detach(); // Deja de mandar señales y frena el servo en su posición actual. 
}