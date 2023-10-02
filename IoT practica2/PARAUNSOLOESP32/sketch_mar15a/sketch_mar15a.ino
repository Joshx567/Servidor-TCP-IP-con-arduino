#include <WiFi.h>
#include <PubSubClient.h>

#define echo 16
#define trigger 15
int cm = 0;
  
int ports[3] = {32, 33, 26}; //Puertos del MCU

const char *WIFI_SSID = "LOPEZ";
const char *WIFI_PASS = "76486651";

const char *SERVER_ADDRESS = "192.168.0.8"; //YOUR_SERVER_IP_ADDRESS mi maquina 192.168.78.48
const int SERVER_PORT = 5000;            //YOUR_SERVER_PORT

  byte mac[] = {
    0xDE,
    0xED,
    0xBA,
    0xFE,
    0xFE,
    0xED};

void setup() 
{
    for (int i = 0; i < 3; i++)
    {
      pinMode(ports[i], OUTPUT);
    }
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    digitalWrite(trigger, LOW);//Inicializamos el pin con 0
  
  Serial.begin(115200);
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);
  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASS); 

  while(WiFi.status() != WL_CONNECTED)
  {
      delay(500);
      Serial.print(".");
  }
  
  Serial.print("\nÏP address: "); 
  Serial.println(WiFi.localIP());
}

  long readUltrasonicDistance(int triggerPin, int echoPin)
  {
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);
    pinMode(echoPin, INPUT);
    return pulseIn(echoPin, HIGH); //Se devolvera el tiempo entre el envio y la recepcion
  }

void loop() 
{ 
  delay(500);
  cm = 0.01723 * readUltrasonicDistance(trigger,echo); //se calculara la distancia multiplicando la velocidad en la que el sonido recorre un centimetro por el tiempo de rebote obtenido.
  
  Serial.print("Connecting to: ");
  Serial.println(SERVER_ADDRESS);
  
  WiFiClient client;
  
  if(!client.connect(SERVER_ADDRESS, SERVER_PORT))
  {
    Serial.println("Connection failed!");
    delay(5000);
    return; //va a volver a iniciar el codigo 
  }
            
  Serial.print(cm); //manda al serial los cm leidos
  Serial.print("\n"); 
  client.print(cm); //manda por internet al server los cm
  
  unsigned long timeout = millis();
  
  while(client.available() == 0)
  {
    if(millis()-timeout>10000)
    {
        Serial.println("Server timeout");
        client.stop();
        return;
    }  
  }
   
  if(client.available()>0)
  {    
      String line = client.readStringUntil('\n'); 
      
      if (line == "BLUE") 
      {
        digitalWrite(ports[0], HIGH);
        delay(1000); 
        digitalWrite(ports[0], LOW);
      }
      if(line == "ORANGE")
      {
        digitalWrite(ports[1], HIGH);
        delay(1000); 
        digitalWrite(ports[1], LOW);
      }
      if(line == "RED")
      {
        digitalWrite(ports[2], HIGH);
        delay(1000); 
        digitalWrite(ports[2], LOW); 
      }    
  }
  
  //Serial.println("Cerrando conection.");
  Serial.println("Closing connection.");
  client.stop();  
}
