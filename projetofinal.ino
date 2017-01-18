int sensor = 5;
int movimento = LOW; 

void setup() {
  pinMode(sensor, INPUT); 
  Serial.begin(9600);

}

void loop() {
  movimento = digitalRead(sensor);  
  if (movimento == HIGH){
    Serial.println("ola");
  }

}
