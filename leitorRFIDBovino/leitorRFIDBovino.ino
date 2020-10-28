#include <SoftwareSerial.h> //Biblioteca SoftwareSerial
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS1307.h> //Biblioteca RTC DS1307
#include <SD.h> //Biblioteca do cartão SD 

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
int QTDLinhas = 0;


//Função para contar as linhas no arquivo 
int ContarLinhas(){
  File Arquivo = SD.open("brincos.txt"); //Arquivo contendo os dados de alerta, no formato: númeroBrinco : alerta
  if(Arquivo){
    while(Arquivo.available()){
        byte B = Arquivo.read();
        if(B == '\n'){
          QTDLinhas++;   
        }
    }      
    Arquivo.close();  
  }
  return QTDLinhas;
}

//Função para buscar as linhas no arquivo.
void BuscarLinhas(int QTDLinhas, String* Linhas){
  int cont = 0;
  File Arquivo = SD.open("brincos.txt"); //Arquivo contendo os dados de alerta, no formato: númeroBrinco : alerta
  if(Arquivo){
    while(Arquivo.available()){
      char crtr = (char)Arquivo.read();
      if(crtr == '\n'){
        cont++;
      }else Linhas[cont] += crtr;
    }
    Arquivo.close();
  }
  return Linhas;
}


//Função para verificar a presença de um brinco com status ativo no arquivo
bool TestarPresenca(String Entrada, String *Status, String Linhas[]){
  int cont = 0;
  for(cont = 0; cont < QTDLinhas; cont++){
    if(Entrada.substring(0,16) == Linhas[cont].substring(0,16)){
      *Status = Linhas[cont].substring(17);
      return true;
    }
  }
return false;
}

void setup(){
  Serial.begin(9600);
  lcd.begin(); // Inicia o LCD
  lcd.setBacklight(HIGH);
  lcd.print("APROXIME O");
  lcd.setCursor(0, 1);
  lcd.print("BRINCO DO LEITOR");
  RFID.begin(9600); // Seta o DataRate do leitor de RFID

  pinMode (bip, OUTPUT);

  pinMode(chipSelect, OUTPUT);// O MicroSD no pino 4 precisa estar como OUTPUT
  if (!SD.begin(chipSelect)) {
    Serial.println("Não foi possível iniciar o cartão SD"); //Caso de o cartão não esteja disponível
  }
  rtc.halt(true);
  //As linhas abaixo setam a data e hora do modulo, após a primeira utilização podem ser comentadas.
  rtc.setDOW(WEDNESDAY);      //Define o dia da semana
  rtc.setTime(21, 30, 0);     //Define o horario
  rtc.setDate(07, 11, 2018);   //Define o dia, mes e ano

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
    digitalWrite(bip, HIGH);
    delay(300);
    digitalWrite(bip, LOW);
    delay(100);
    QTDLinhas = ContarLinhas() + 1;
    String Linhas[QTDLinhas];
    BuscarLinhas(QTDLinhas,Linhas);
    String Status;
    if(TestarPresenca(nome, &Status, Linhas)){
      Status.toUpperCase();
      Serial.println(Status);
      lcd.clear();
      lcd.print(nome);
      lcd.setCursor(0, 1);
      lcd.print(Status);
      delay(100);
      digitalWrite(bip, HIGH);
      delay(300);
      digitalWrite(bip, LOW);
      delay(300);
      digitalWrite(bip, HIGH);
      delay(300);
      digitalWrite(bip, LOW);  
    }else 
    {
      Serial.println("SEM ALERTA");
      lcd.print(nome);    
    }
    nome = String(nome + " DATA " + rtc.getDateStr());
    File arquivo = SD.open("arquivo.txt", FILE_WRITE);
    arquivo.println(nome);
    arquivo.close();
    nome = "";
  }
}
