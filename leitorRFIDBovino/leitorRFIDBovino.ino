#include <SoftwareSerial.h> //Biblioteca SoftwareSerial
#include <Wire.h>
#include <LiquidCrystal_I2C.h> 
#include <DS1307.h> //Biblioteca RTC DS1307 

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27, 16, 3);
// Inicializa o leitor de RFID
SoftwareSerial RFID(3, 2); // RX e TX
//Modulo RTC DS1307 ligado as portas A4 e A5 do Arduino
DS1307 rtc(A2, A3);

int chipSelect = 4; //Pino 4 para o adaptador MicroSD

//o código gravado no brinco é em char, então não existe necessidade de usar inteiro.
char c; 
String nome = "";
int bip = 8; 

void setup(){
  Serial.begin(9600);
  lcd.begin(); // Inicia o LCD
  lcd.setBacklight(HIGH);
  lcd.print("APROXIME O");
  lcd.setCursor(0, 1);
  lcd.print("BRINCO DO LEITOR");
  RFID.begin(9600); // Seta o DataRate do leitor de RFID

  pinMode (bip, OUTPUT);
                                          
  rtc.halt(true);
  //As linhas abaixo setam a data e hora do modulo, após a primeira utilização podem ser comentadas.
  rtc.setDOW(THURSDAY);      //Define o dia da semana
  rtc.setTime(00, 16, 0);     //Define  o horario
  rtc.setDate(14, 10, 2021);   //Define o dia, mes e ano

  //Definicoes do pino SQW/Out
  rtc.setSQWRate(SQW_RATE_1);
  rtc.enableSQW(true);

  Serial.begin(9600);
}

void loop()
{

  if (RFID.available() > 0) //Caso o Leitor de RFID esteja disponível, realiza o loop interno. Cada vez que o loop interno terminar, retorna ao loop principal e limpa a tela
  {
    delay(100);
    lcd.clear();
    lcd.print("NUMERO DO BRINCO");
     lcd.setCursor(0, 1);
    while (RFID.available() > 0) { //Enquanto o leitor estiver disponível, realiza a leitura
      c = RFID.read(); // C recebe os dados do Leitor
      Serial.write(c);
      nome = String(nome + c); //
      if (c == '\r')  Serial.write('\n');
    } 
    lcd.print(nome);  
    digitalWrite(bip, HIGH);
    delay(300);
    digitalWrite(bip, LOW);
    delay(100); 
    nome = "";
  }
}
