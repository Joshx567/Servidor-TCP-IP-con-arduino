#include <WiFi.h>
#define echo 16
#define trigger 15
int cm = 0;

const char *WIFI_SSID = "LOPEZ";
const char *WIFI_PASS = "76486651";

const char *SERVER_ADDRESS = "192.168.0.2"; //YOUR_SERVER_IP_ADDRESS mi maquina
const int SERVER_PORT = 5000;            //YOUR_SERVER_PORT

void setup() 
{
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
  Serial.begin(115200);
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
    return;
  }
    Serial.print(cm);
    Serial.print("\n");
    client.println("Hello World!\n");

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
      if (line == "ON") 
      {
          //Serial.print("Distancia: ");
          Serial.print(cm); //se muestra en el arduino
          client.print(cm); //imprime como cliente el arduino hacia el server que recibe algo
          //Serial.println("cm");
          //enviar datos al servidor
      }
          
      else
      {
          return;
      }     
  }
  
  Serial.println("Closing connection.");
  client.stop();  
}
