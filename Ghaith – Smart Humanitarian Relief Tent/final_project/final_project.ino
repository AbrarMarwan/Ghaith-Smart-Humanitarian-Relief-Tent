#include <OneWire.h>
#include <DallasTemperature.h>
#include <Servo.h>

// ====== Pins ======
#define TEMP_PIN 3
#define RELAY_PIN 2
#define SERVO_PIN 4
#define RAIN_PIN 5

// ====== Objects ======
OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);
Servo roofServo;

// ====== Variables ======
float temperature;

void setup() {
  Serial.begin(9600);

  sensors.begin();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN,LOW);   // الريليه مطفي بالبداية (Active  HIGH)

  pinMode(RAIN_PIN, INPUT);

  roofServo.attach(SERVO_PIN);
  roofServo.write(0);  // الغطاء مغلق بالبداية
}

void loop() {
  // ===== قراءة الحرارة =====
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);

  Serial.print("Temperature: ");
  Serial.println(temperature);

  // ===== التحكم بالسخان =====
  if (temperature < 30) {
    digitalWrite(RELAY_PIN, HIGH);   // تشغيل السخان
    Serial.println("Heater ON");
  }
  else if (temperature >= 50) {
    digitalWrite(RELAY_PIN, LOW );  // إطفاء السخان
    Serial.println("Heater OFF");
  }

  // ===== حساس المطر =====
  int rainState = digitalRead(RAIN_PIN);

  if (rainState == LOW) {   // في مطر
    roofServo.write(90);    // فتح الغطاء
    Serial.println("Rain detected - Roof Open");
  } else {
    roofServo.write(0);     // غلق الغطاء
  }

  delay(2000);
}
