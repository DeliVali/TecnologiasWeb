int LedIntegrado=1;

void setup() {
  // put your setup code here, to run once:
  
pinMode(LedIntegrado,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

//Va escribir un voltaje de salida en el pin 2
  digitalWrite(LedIntegrado,HIGH);
  delay (1000);
  digitalWrite (LedIntegrado,LOW);
  delay(500);

}
