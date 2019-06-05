int ledVerde =  2; 
int ledAmarelo = 3;
int dado; //variável que receberá os dados da porta serial
 
void setup(){
  Serial.begin(9600);//frequência da porta serial
  pinMode(ledVerde,OUTPUT);
  pinMode(ledAmarelo,OUTPUT);
}
 
void loop(){
  if(Serial.available() > 0){ //verifica se existe comunicação com a porta serial
      dado = Serial.read();//lê os dados da porta serial
      switch(dado){
        case 1:
           digitalWrite(ledVerde,HIGH);
           digitalWrite(ledAmarelo,LOW);
           delay(1000);
           digitalWrite(ledVerde,LOW);
        break;
        case 2:
           digitalWrite(ledAmarelo,HIGH);
           digitalWrite(ledVerde,LOW);
           delay(1000);
           digitalWrite(ledAmarelo,LOW);
        break;
    }
  }
}
