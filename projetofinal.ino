// Código do Stepper. Ainda não foram implementadas as funções de controle do motor, pois não foram definidos ainda
// os angulos que o motor deverá percorrer.
#include <Ethernet.h>
#include <SPI.h>
#include "RestClient.h"
#include <Stepper.h>

RestClient client = RestClient("query.yahooapis.com");
char apiWeather[256] = "/v1/public/yql?q=select%20item.condition%20from%20weather.forecast%20where%20woeid%20%3D%20449704%20AND%20u%3D%27c%20%27&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys";

int count = 0;
String response;
int anguloAntigo = 0;
int anguloNovo = 0;
int flagChuva = 0;

int sensor = 5; // Receptor do sensor de presença na porta 5.
int movimento = LOW; // Sensor de presença ativado ou não.

const int stepsPerRevolution = 48;

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 6,7 ,8, 9);

void setup() {
  pinMode(sensor, INPUT); 
  myStepper.setSpeed(60); //Velocidade de 60rpm
  myStepper.step(48); //Posição inicial do motor
  
  // initialize the serial port:
  Serial.begin(9600);
  Serial.println("connect to network");
  client.dhcp();
  Serial.println("Setup!");
}

void loop() {
  myStepper.setSpeed(25);
  movimento = digitalRead(sensor);  //Recebe o sinal do motor (se há presença ou não 
  if (movimento == HIGH){ //Se houver presença, o motor mexe
     
    //api weather
    while(count != 1){
      response = "";
      int statusCode = client.get(apiWeather, &response);
      Serial.print("Status code from server: ");
      Serial.println(statusCode);
      Serial.print("Response body from server: ");
      Serial.println(response);
      
      int index = response.indexOf("count");
      String lerCount = response.substring(index+7, index+8);
      count = lerCount.toInt();
    }
    count = 0;
    
    int indexTemp = response.indexOf("temp");
    String lerTemperatura = response.substring(indexTemp+7, indexTemp+9);
    int temperatura = lerTemperatura.toInt();
    Serial.println(temperatura);
    
    int indexCode = response.indexOf("code");
    String lerCode = response.substring(indexCode+7, indexCode+9);
    int code = lerCode.toInt();
    Serial.println(code);
    
    if (code ==  1 ||code ==  3 ||code ==  4 ||code ==  6 ||code ==  9 ||code ==  11 ||code ==  12 ||code ==  17 ||code ==  18 ||code ==  20 ||code ==  21 ||code ==  35 ||code ==  37 ||code ==  38 ||code ==  39 ||code ==  40 ||code ==  45 ||code ==  47 )
      flagChuva=1;
     else
       flagChuva = 0;
       
    if (flagChuva == 1){
      anguloNovo = anguloAntigo - 2;
      myStepper.step(-anguloNovo);
      delay(500);
      anguloAntigo = 2;
    }
    else{
      if (temperatura < 15){
        anguloNovo = anguloAntigo - 6;
        myStepper.step(-anguloNovo);
        delay(500);
        anguloAntigo = 6;
      }
      
      if (temperatura >= 15 && temperatura <20){
        anguloNovo = anguloAntigo - 10;
        myStepper.step(-anguloNovo);
        delay(500);
        anguloAntigo = 10;
      }
      
      if (temperatura >= 20 && temperatura <25){
        anguloNovo = anguloAntigo - 14;
        myStepper.step(-anguloNovo);
        delay(500);
        anguloAntigo = 14;
      }
      
      if (temperatura >= 25 && temperatura <35){
        anguloNovo = anguloAntigo - 18;
        myStepper.step(-anguloNovo);
        delay(500);
        anguloAntigo = 18;
      }
      
      if (temperatura >= 35){
        anguloNovo = anguloAntigo - 22;
        myStepper.step(-anguloNovo);
        delay(500);
        anguloAntigo = 22;
      }
    }
    
  }
  
}
