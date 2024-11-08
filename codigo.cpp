#include <Servo.h> 
#include <NewPing.h> 

#define TRIG_PIN 9
#define ECHO_PIN 10
#define MAX_DISTANCE 200 // Máxima distancia a medir en cm

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);


#define ENA 5 // Velocidad motor izquierdo
#define IN1 4 // Dirección motor izquierdo
#define IN2 3 // Dirección motor izquierdo
#define ENB 6 // Velocidad motor derecho
#define IN3 7 // Dirección motor derecho
#define IN4 8 // Dirección motor derecho


char bluetoothCommand;


Servo canon;
#define SERVO_PIN 11


boolean modoAutomatico = false;

void setup() {
  
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  
  Serial.begin(9600); 


  canon.attach(SERVO_PIN);


  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

 
  randomSeed(analogRead(0)); 
}

void loop() {
  if (Serial.available()) {
    bluetoothCommand = Serial.read();
    controlarTanqueManual(bluetoothCommand);
  }

  if (modoAutomatico) {
    moverTanqueAutomatico();
  }
}


void controlarTanqueManual(char command) {
  switch (command) {
    case 'F': 
      moverAdelante();
      break;
    case 'B':
      moverAtras();
      break;
    case 'L': 
      girarIzquierda();
      break;
    case 'R': 
      girarDerecha();
      break;
    case 'S': 
      detener();
      break;
    case 'A': 
      modoAutomatico = true;
      break;
    case 'M': 
      modoAutomatico = false;
      detener();
      break;
    case 'C': 
      dispararCanon();
      break;
  }
}


void moverAdelante() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 255); 

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 255);
}

void moverAtras() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 255);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 255);
}

void girarIzquierda() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 150);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 150);
}

void girarDerecha() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 150);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 150);
}

void detener() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 0);
}


void moverTanqueAutomatico() {
  int distancia = sonar.ping_cm();
  
  if (distancia > 0 && distancia < 20) { 
    moverAtras();
    delay(500);
    
   
    int randomDirection = random(0, 2);
    if (randomDirection == 0) {
      girarIzquierda();
    } else {
      girarDerecha();
    }
    delay(500); 
  } else {
    moverAdelante();
  }
}


void dispararCanon() {
  canon.write(0);   
  delay(500);
  canon.write(90); 
}
