// Código do Previsor do Tempo, já utilizando o Stepper, Sensor de Presença, LCD e PushButton.
#include <Ethernet.h>
#include <SPI.h>
#include "RestClient.h"
#include <Stepper.h>
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

// Cores de Fundo padrão do LCD
const int colorR = 120;
const int colorG = 67;
const int colorB = 200;

// Declaração da API Yahoo Weather 
RestClient client = RestClient("query.yahooapis.com");
char apiWeather[186] = "/v1/public/yql?q=select%20item.condition%20from%20weather.forecast%20where%20woeid%20%3D%20449704%20AND%20u%3D%27c%20%27&format=json&env=store%3A%2F%2Fdatatables.org%2Falltableswithkeys"; // São Carlos

// Declaração das Variáveis Globais
int count = 0; // Verificador de resposta da API
int cidade = 0; // Variável de escolha da cidade
String response; // Resposta da API
int flagChuva = 0; // Flag de chuva atual
int flagChoveu = 0; // Flag de chuva em momento anterior
  // Sensor de Presença:
int sensor = 5; // Receptor do sensor de presença na porta 5
int movimento = LOW; // Sensor de presença ativado ou não
  // PushButton:
int botao = 4; // PushButton na porta 4
  // Stepper:
const int stepsPerRevolution = 48; // Número de steps por revolução do Stepper
Stepper myStepper(stepsPerRevolution, 6,7 ,8, 9); // Inicialização do Stepper nas portas 6, 7, 8 e 9

void setup() {
  // Declaração das portas sensor (5) e botao (4) como entradas
  pinMode(sensor, INPUT);
  pinMode(botao, INPUT);
  
  // Inicialização do Stepper
  myStepper.setSpeed(25); //Velocidade de 25rpm
  //myStepper.step(48);
  
  // Inicialização da porta serial
  Serial.begin(9600);
  Serial.println("connect to network");
  client.dhcp();
  Serial.println("Setup!");
  
  // Inicialização do LCD
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB); // Cor de fundo padrão
  
  // Print do título no LCD
  lcd.print("PrevisorDoTempo");

  delay(1000);
}

void ApiLink(){//Função para mudar o link da api, de acordo com a cidade escolhida

  if (cidade == 0){
      apiWeather[91] = '4';
      apiWeather[92] = '4';
      apiWeather[93] = '9';
      apiWeather[94] = '7';
      apiWeather[95] = '0';
      apiWeather[96] = '4';
    }

  if (cidade == 1){
    apiWeather[91] = '4';
    apiWeather[92] = '5';
    apiWeather[93] = '5';
    apiWeather[94] = '8';
    apiWeather[95] = '3';
    apiWeather[96] = '9';
  }
  if (cidade == 2){
    apiWeather[91] = '4';
    apiWeather[92] = '5';
    apiWeather[93] = '6';
    apiWeather[94] = '9';
    apiWeather[95] = '4';
    apiWeather[96] = '0';
  }
}

void ChamarCidade(){ // Função para consulta da previsão na API Yahoo Weather
  ApiLink();
  while(count != 1){
      response = "";
      int statusCode;
      statusCode = client.get(apiWeather, &response);
        //statusCode = client.get(apiWeatherAJ, &response);
      Serial.print("Status code from server: ");
      Serial.println(statusCode);
      Serial.print("Response body from server: ");
      Serial.println(response);
      
      int index = response.indexOf("count");
      String lerCount = response.substring(index+7, index+8);
      count = lerCount.toInt();
    }
    count = 0;
}

void loop() {
  movimento = digitalRead(sensor);  // Recebe o sinal do motor (se há presença ou não)
  
  if (movimento == HIGH){ // Se houver presença, é consultada a previsão na API
    
    // Mensagem de tentativa de conexão
    lcd.setCursor(0, 1);
    lcd.print("Conectando...   "); 
    lcd.setRGB(255,0,0);
    
    ChamarCidade();
    
    // Se deu certo, imprime mensagem de sucesso
    lcd.setCursor(0, 0);
    lcd.clear();
    lcd.print("Conectado!");
    lcd.setRGB(0,255,0);
    delay(1000);
    
    // Leitura da temperatura dentro da resposta da API
    int indexTemp = response.indexOf("temp"); 
    String lerTemperatura = response.substring(indexTemp+7, indexTemp+9);
    int temperatura = lerTemperatura.toInt();
    Serial.println(temperatura);
    
    // Leitura do código do tempo (se está chovendo, nublado, etc.) da resposta da API
    int indexCode = response.indexOf("code");
    String lerCode = response.substring(indexCode+7, indexCode+9);
    int code = lerCode.toInt();
    Serial.println(code);
    
    // Imprime o nome da cidade em questão no LCD
    lcd.clear();
    if (cidade == 0)
      lcd.print("Sao Carlos");
    if (cidade == 1)
      lcd.print("Aracaju");
    if (cidade == 2)
      lcd.print("C. Jordao");
    
    // Imprime a temperatura da cidade em questão lida no LCD
    lcd.setCursor(0, 1);
    lcd.print("Temperatura: ");
    lcd.print(temperatura);
    lcd.print("C");
    
    // Troca a cor de fundo do LCD dependendo da temperatura lida na cidade
    if (temperatura < 15){
      lcd.setRGB(0,0,255);
    }
    
    if (temperatura >= 15 && temperatura <20){
      lcd.setRGB(0,0,125);
    }
    
    if (temperatura >= 20 && temperatura <25){
      lcd.setRGB(16,0,64);
    }
    
    if (temperatura >= 25 && temperatura <35){
      lcd.setRGB(125,0,0);
    }
    
    if (temperatura >= 35){
      lcd.setRGB(255,0,0);
    }
    
    // Verifica se está chovendo ou não, utilizando o código lido e os códigos de chuva fornecidos na documentação da API
    if (code ==  1 ||code ==  3 ||code ==  4 ||code ==  6 ||code ==  9 ||code ==  11 ||code ==  12 ||code ==  17 
            ||code ==  18 ||code ==  20 ||code ==  21 ||code ==  35 ||code ==  37 ||code ==  38 ||code ==  39 ||code ==  40 ||code ==  45 ||code ==  47 )
      flagChuva=1;
     else
       flagChuva = 0;
       
    if (flagChuva == 1){  // Caso esteja chovendo
        if(flagChoveu == 0){  // E o motor já tenha girado
          myStepper.step(-200);  // Fecha a janela
          delay(500);
          flagChoveu = 1;
      }
    }
    if (flagChuva == 0){  // Caso não esteja chovendo
        if(flagChoveu == 1){  // E o motor já tenha girado
          myStepper.step(200);  // Abre a janela
          
          delay(500);
          flagChoveu = 0;
      }
    }   
  }

  // Caso o botão seja pressionado, troca a cidade em questão
  if (digitalRead(botao) == HIGH){   
    if (cidade < 2)
      cidade++;
    else
      cidade = 0; 
    Serial.println(cidade);
    delay(500);
  }
 
}
