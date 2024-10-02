#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Definições do LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definições do DHT11
#define DHTPIN A1     // Pino onde o DHT11 está conectado
#define DHTTYPE DHT11 // Define o tipo de sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Pino do relé
#define int relePin = 8;  

// Variáveis para controle de tempo e alternância
unsigned long previousMillis = 0;
// Tempo de 5 segundos (5000 ms) 
const long interval = 5000;  // 5 segundos
bool showMessage = false;

void setup() {
  Wire.begin();  
  lcd.init();
  lcd.backlight();

  pinMode(relePin, OUTPUT);
  digitalWrite(relePin, LOW);

  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("Inicializando...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Leitura do sensor DHT11
  float temperatura = dht.readTemperature();
  float umidade = dht.readHumidity();

  // Verificação se as leituras são válidas
  if (isnan(temperatura) || isnan(umidade)) {
    lcd.setCursor(0, 0);
    lcd.print("Erro leitura DHT");
    return;
  }

  // Controle do relé baseado na umidade 
  if (umidade >= 75) {
    digitalWrite(relePin, HIGH);  // Liga o relé
  } else if (umidade <= 70) {
    digitalWrite(relePin, LOW);   // Desliga o relé
  }

  // Controle de tempo para alternância de exibição
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    showMessage = !showMessage;
  }

  // Exibição na tela LCD
  lcd.clear();
  
  if (showMessage) {
    lcd.setCursor(0, 0);
    lcd.print("Status OK!");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperatura);
    lcd.print(" C");
  }

  lcd.setCursor(0, 1);
  lcd.print("Umidade: ");
  lcd.print(umidade);
  lcd.print(" %");
  
  delay(2000);  // Pequeno delay para suavizar as transições na tela
}
