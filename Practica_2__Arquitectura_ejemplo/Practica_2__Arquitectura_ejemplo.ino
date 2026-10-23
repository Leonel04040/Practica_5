#include "DHT.h"

#define DHTPIN 2       // Pin digital para el DHT11
#define DHTTYPE DHT11  // Tipo de sensor

DHT dht(DHTPIN, DHTTYPE);

const int pinTrig = 9;
const int pinEcho = 10;
const int pinMotor = 13; 

const float limiteTemp = 30.0;     // °C
const float limiteDistancia = 5.0; // cm

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando sistema con DHT11...");
  
  pinMode(pinMotor, OUTPUT);
  pinMode(pinTrig, OUTPUT);
  pinMode(pinEcho, INPUT);
  
  dht.begin(); // Inicializa la comunicación con el DHT11
}

void loop() {
  // 1. Lectura digital de temperatura
  float temperatura = dht.readTemperature(); 
  
  // Verificación de fallo de lectura
  if (isnan(temperatura)) {
    Serial.println("Error al leer el DHT11. Revisa las conexiones.");
    delay(2000);
    return;
  }

  // 2. Lectura de distancia (HC-SR04)
  digitalWrite(pinTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinTrig, LOW);
  
  long duracion = pulseIn(pinEcho, HIGH, 30000); 
  float distancia = (duracion == 0) ? 999.0 : (duracion * 0.034 / 2);

  // 3. Monitor Serial
  Serial.print("Temp: ");
  Serial.print(temperatura);
  Serial.print(" C | Distancia: ");
  Serial.print(distancia);
  Serial.print(" cm -> ");

  // 4. Lógica de control
  if (temperatura > limiteTemp || distancia < limiteDistancia) {
    digitalWrite(pinMotor, HIGH);
    Serial.println("Estado: ENCENDIDO");
  } else {
    digitalWrite(pinMotor, LOW);
    Serial.println("Estado: APAGADO");
  }

  // El DHT11 requiere de 1 a 2 segundos entre lecturas para no saturarse
  delay(1500); 
}
