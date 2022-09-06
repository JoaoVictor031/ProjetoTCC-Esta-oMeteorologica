#include <SPI.h>
#include <SD.h>
#include <DHT.h>
#include <RTClib.h>

//definir pino DHT
#define DHTPIN A0

//descomente qualquer tipo que você estiver usando
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22 (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// inicialize o sensor DHT para Arduino normal de 16 mhz
DHT dht(DHTPIN, DHTTYPE);

// mude isso para combinar com seu escudo ou módulo SD;
// Blindagem e módulos Arduino Ethernet: pino 4
// Blocos e módulos SD do log de dados: pino 10
// Blindagem Sparkfun SD: pino 8
const int chipSelect = 4; 

// Crie um arquivo para armazenar os dados
File myFile;

// RTC
RTC_DS1307 rtc;

void setup() {
  pinMode (7, OUTPUT);
  

  //inicializando o sensor DHT
  dht.begin();

  //inicializando o monitor serial
 // Serial.begin(9600);
  
  // configuração para o RTC
  while(!Serial); //para Leonardo/Micro/ Zero
    if(! rtc.begin()) {
      Serial.println("Não foi possível encontrar o RTC");
      while (1);
    }
    else {
      // A linha a seguir define o RTC para a data e hora em que este esboço foi compilado
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    if(! rtc.isrunning()) {
      Serial.println("O RTC NÃO está sendo executado!");
    }
    
  //configuração para o cartão SD
  Serial.print("Inicializando o SD Card...");

  if(!SD.begin(chipSelect)) {
    Serial.println("falha na inicialização!");
    return;
  }
  Serial.println("inicialização feita.");
    
  //abrir arquivo
  myFile=SD.open("DATA.csv", FILE_WRITE);

  // se o arquivo foi aberto, escreva para ele:
  if (myFile) {
    Serial.println("Arquivo aberto ok");
    // imprima os títulos dos nossos dados
    myFile.println("Date,Time,Temperatura ºC");
  }
  myFile.close();
}

void loggingTime() {
  DateTime now = rtc.now();
  myFile = SD.open("DATA.csv", FILE_WRITE);
  if (myFile) {
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(',');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(";");
  }
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.println(now.day(), DEC);
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  myFile.close();
  delay(100);  
}

void loggingTemperature() {
  //A leitura da temperatura ou umidade leva cerca de 250 milissegundos!
  //As leituras do sensor também podem ter até 2 segundos de idade (é um sensor muito lento)
  //Ler temperatura como Celsius
  float t = dht.readTemperature();
  //Leia a temperatura como Fahrenheit
  //float f = dht.readTemperature(true);
  
  //Verifique se alguma leitura falhou e saia mais cedo (para tentar novamente).
  if  (isnan(t) /*|| isnan(f)*/) {
    Serial.println("Falha ao ler o sensor DHT!");
    return;
  }
  
  //fins de depuração
  Serial.print("Temperatura: "); 
  Serial.print(t);
  Serial.println(" *C");
  //Serial.print(f);
  //Serial.println(" *F\t"); 
  
  myFile = SD.open("DATA.csv", FILE_WRITE);
  if (myFile) {
    myFile.print(t);
    myFile.print(";");
  }
  myFile.close();
}


//Umidade

void loggingHumidity() {
  //A leitura da temperatura ou umidade leva cerca de 250 milissegundos!
  //As leituras do sensor também podem ter até 2 segundos de idade (é um sensor muito lento)
  //Ler temperatura como Celsius
  float h = dht.readHumidity();
  //Leia a temperatura como Fahrenheit
  //float f = dht.readTemperature(true);
  
  //Verifique se alguma leitura falhou e saia mais cedo (para tentar novamente).
  if  (isnan(h) /*|| isnan(f)*/) {
    Serial.println("Falha ao ler o sensor DHT!");
    return;
  }
  
  //fins de depuração
  Serial.print("Umidade: "); 
  Serial.print(h);
  Serial.println(" % ");
  //Serial.print(f);
  //Serial.println(" *F\t"); 
  
  myFile = SD.open("DATA.csv", FILE_WRITE);
  if (myFile) {
    myFile.print(h);
    myFile.println("");
  }
  myFile.close();
}
void loop() {
  digitalWrite (10,1);
  delay(1000);
  digitalWrite(10,0);
  loggingTime();
  loggingTemperature();
  loggingHumidity();
  Serial.println("------------------");
  delay(10000);
}
