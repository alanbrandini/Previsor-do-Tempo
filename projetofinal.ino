// Código do Stepper. Ainda não foram implementadas as funções de controle do motor, pois não foram definidos ainda
// os angulos que o motor deverá percorrer.
#include <Stepper.h>

int sensor = 5; // Receptor do sensor de presença na porta 5.
int movimento = LOW; // Sensor de presença ativado ou não.

const int stepsPerRevolution = 52;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {
  pinMode(sensor, INPUT); 
  myStepper.setSpeed(60); //Velocidade de 60rpm
  myStepper.step(100); //Posição inicial do motor
  
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
  myStepper.setSpeed(25);
  movimento = digitalRead(sensor);  //Recebe o sinal do motor (se há presença ou não 
  if (movimento == HIGH){ //Se houver presença, o motor mexe
    // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution/10);
  delay(500);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution/10);
  delay(500);
  }
  
}

