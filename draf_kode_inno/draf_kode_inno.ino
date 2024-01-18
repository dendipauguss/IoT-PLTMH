/* ==== TORSI GENETIK (MONITORING SISTEM GENERATOR PEMBANGKIT LISTRIK)==== */
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPLqozpW9dj"                    //
#define BLYNK_TEMPLATE_NAME "Data Dummy"                    // Diambil dari cloud server Blynk.console yang kita buat
#define BLYNK_AUTH_TOKEN "R9uufAttN5dJUgE4MjooZt_tva8rBppT" //

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <PZEM004Tv30.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Mau ? Nanya aja"; // Sesuaikan dengan SSID Wifi yang digunakan
char pass[] = "sipapasipaga123"; // Sesuaikan dengan Password Wifi yang digunakan

LiquidCrystal_I2C lcd(0x27, 16, 2);// Sesuaikan jumlah kolom, baris dengan jenis LCD I2C yang dipakai

#if defined(ESP32)
PZEM004Tv30 pzem(Serial2, 16, 17); // Sesuaikan dengan pin yang dipakai (TX, RX), disini saya memakai 16 = TX, 17 = RX
#else
PZEM004Tv30 pzem(Serial2);
#endif

#define pinSSR 12 // Sesuaikan dengan pin yang dipakai pada Solid-state Relay
void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);  //memulai Blynk
  lcd.init();                      // initialize the lcd
  // Print a message to the LCD.
  lcd.backlight();
  //  lcd.setCursor(2, 0);
  //  lcd.print("TORSI GENETIK");
  // Uncomment in order to reset the internal energy counter
  // pzem.resetEnergy();
  pinMode(pinSSR, OUTPUT);
}

void loop() {

  Serial.print("Custom Address:");
  Serial.println(pzem.readAddress(), HEX);
  
  // Kondisi Solid-state Relay
  // digitalWrite(pinSSR, HIGH);
  
  // Membaca data dari sensor PZEM-004T
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();
  
  // Mengecek jika data terdeteksi dengan sensor
  if (isnan(voltage)) {
    Serial.println("Error reading voltage");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Error Baca  Volt");
  } else if (isnan(current)) {
    Serial.println("Error reading current");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Error Baca A");
  } else if (isnan(power)) {
    Serial.println("Error reading power");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Error Baca W");
  } else if (isnan(energy)) {
    Serial.println("Error reading energy");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Error Baca kWh");
  } else if (isnan(frequency)) {
    Serial.println("Error reading frequency");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Error Baca Hz");
  } else if (isnan(pf)) {
    Serial.println("Error reading power factor");
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Error Baca Pf");
  } else {
    lcd.clear();
    
    // Print nilai yang terdeteksi pada Serial Monitor
    Serial.print("Voltage: ");      Serial.print(voltage);      Serial.println("V");
    Serial.print("Current: ");      Serial.print(current);      Serial.println("A");
    Serial.print("Power: ");        Serial.print(power);        Serial.println("W");
    Serial.print("Energy: ");       Serial.print(energy, 3);     Serial.println("kWh");
    Serial.print("Frequency: ");    Serial.print(frequency, 1); Serial.println("Hz");
    Serial.print("PF: ");           Serial.println(pf);

    lcd.setCursor(0, 0);            lcd.print(voltage);             lcd.print(" V");
    lcd.setCursor(10, 0);            lcd.print(current);             lcd.print(" A");
    lcd.setCursor(10, 1);            lcd.print(power);               lcd.print(" W");
    lcd.setCursor(1, 1);            lcd.print(frequency, 1);        lcd.print(" Hz");
    //    lcd.setCursor(3, 1);            lcd.print(energy);              lcd.print(" kWh");
    //    lcd.setCursor(5, 1);            lcd.print(pf);

    Blynk.virtualWrite(V1, voltage);
    Blynk.virtualWrite(V2, current);
    Blynk.virtualWrite(V3, power);
    Blynk.virtualWrite(V4, energy);
    Blynk.virtualWrite(V5, frequency);
    Blynk.virtualWrite(V6, pf);
  }

  Serial.println();
  Blynk.run();
  delay(2000);
}

BLYNK_WRITE(V0) {  // Mendeteksi perubahan status widget LED di pin virtual V1
  int relayStatus = param.asInt();
  digitalWrite(pinSSR, relayStatus);
}
