  #define echo 16
  #define trigger 15
  float cm = 0;

  void setup() 
  {
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
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
    cm = 0.01723 * readUltrasonicDistance(trigger,echo); //se calculara la distancia multiplicando la velocidad en la que el sonido recorre un centimetro por el tiempo de rebote obtenido.
    Serial.print("Distancia: ");
    Serial.print(cm);
    Serial.println("cm");
  }
