// Preparação do Arduino, este código irá rodar uma única vez
void setup() 
{
  // Colocamos o pino 12 do Arduino como OUTPUT (saída)
  pinMode(2, OUTPUT); //verde
  pinMode(3, OUTPUT); //amarelo
  pinMode(4, OUTPUT); //vermelho
}

// Este código é chamado automáticamente pelo Arduino, ficará em
// loop até que seu Arduino seja desligado
void loop() 
{
  // Ativamos o pino 12 (colocando 5v nele)  
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  // Aguardamos 1 segundo
  delay(1000);

  // Desligamos o pino 12
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  // Aguardamos mais um segundo
  delay(1000);

  // Este código irá se repetir eternamente
}
