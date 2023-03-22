
#include <ESP8266WiFi.h> //WiFi
#include <DHT.h> //อุณหภูมิ
#include <Wire.h> //กำหนดPinของ LCD
#include <LiquidCrystal_I2C.h> //LCD
#include <TridentTD_LineNotify.h> //Line Notify
#include <BlynkSimpleEsp8266.h> //แอพBlynk
#define BLYNK_PRINT Serial //แอพBlynk แสดงบน Serial
#define Pump D0 //กำหนดPinของ Relay
#define DHTPIN D5 //กำหนดPinของ เซ็นเซอร์อุอุณหภูมิ
#define DHTTYPE DHT11 //กำหมดรุ่นของ เซ็นเซอร์อุอุณหภูม
#define LINE_TOKEN "Your Line Token" //กำหนด
Linetoken
LiquidCrystal_I2C lcd(0x27, 16, 2); //เซ็ต LCD
DHT dht(DHTPIN, DHTTYPE); //เซ็ต เซ็นเซอร์อุอุณหภูมิ
BlynkTimer timer; //แอพBlynk เวลา
char auth[] = "Token Blynk"; //กำหนด Token Blynk
/////////////Phone Hot sport//////////////
//char ssid[] = "SSID"; //กำหนด ชื่อ WiFi
//char pass[] = "Password"; //กำหนด รหัสผ่าน WiFi
//////////////////////////////////////////
/////////////Home Wifi///////////////////
//char ssid[] = "SSID"; //กำหนด ชื่อ WiFi
//char pass[] = "Password"; //กำหนด รหัสผ่าน WiFi
/////////////////////////////////////////
/////////////Notebook hot sport/////////
char ssid[] = "SSID"; //กำหนด ชื่อ WiFi
char pass[] = "Password"; //กำหนด รหัสผ่าน WiFi
/////////////////////////////////////////
const int AirValue = 620; //แทนที่ค่านี้ด้วย Value_1
const int WaterValue = 310; //แทนที่ค่านี้ด้วย Value_2
float t ; //กำหนดตัวแปร Temp
float h ; //กำหนดตัวแปร Humidity
int soilMoistureValue = 0; //กำหนดตัวแปร soilMoistureValue
int soilmoisturepercent=0-100; //กำหนดตัวแปร soilmoisturepercent
WidgetLED PumpBlynktest(V0); //Blynk แสดงไฟ LED บน Blynk
WidgetLED PumpBlynk(V2); //Blynk กำหนด Button ของ Blynk
void setup()
{
 pinMode(D6,OUTPUT);
 pinMode(D7,OUTPUT);
 pinMode(Pump, OUTPUT);
 Serial.begin(9600);
 ////////////Wifi&Blynk///////////
 Blynk.begin(auth, ssid, pass);
 WiFi.begin(ssid, pass);
 Serial.println(WiFi.localIP());
 Serial.println(LINE.getVersion());
 //////////////Line//////////////
 LINE.setToken(LINE_TOKEN);
 LINE.notify("Device : on");
 Serial.println("Line : Activate");
 /////////////temp///////////////
 dht.begin();
 Serial.println("TempSensor : Activate");
 //////////////LCD///////////////
 lcd.begin();
 lcd.print("Project 101");
 delay(2000);
 lcd.clear();
 lcd.print("Ready");
 lcd.blink();
 delay(2000);
 lcd.clear();
}
BLYNK_CONNECTED() {
 Blynk.syncVirtual(V7);
}
////////////ทำสอบการทำงานของปั๊มน้ำ/////////////////
/*BLYNK_WRITE(V7){
 Serial.println(param.asInt());
 if (param.asInt()){
 digitalWrite(Pump, HIGH);
 PumpBlynktest.off();
 LINE.notify("ทดสอบการทำงานปั้มน้ำ : On");
 } else {
 digitalWrite(Pump, LOW);
 PumpBlynktest.on();
 LINE.notify("ทดสอบการทำงานปั้มน้ำ :Off "); }}*/
//////////////////////////////////////////////////
void soilsenser(){
 /////////////////เซ็นเซอร์ความชื้นในดิน/////////////
 while(1+1==2){
soilMoistureValue = analogRead(A0); //put Sensor insert into soil
Serial.println(" ");
Serial.println(soilMoistureValue);
soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
if(soilmoisturepercent > 100)
{
 Serial.println("100 %");
}
else if(soilmoisturepercent <0)
{
 Serial.println("0 %");
}
else if(soilmoisturepercent >0 && soilmoisturepercent < 100)
{
 Serial.print(soilmoisturepercent);
 Serial.println("%");
}Blynk.virtualWrite(V12,soilMoistureValue);
Blynk.virtualWrite(V13,soilmoisturepercent);
Serial.println("End");
/////////////////////////////////////////////////
////////////////เซ็นเซอร์อุอุณหภูมิ///////////////////
h = dht.readHumidity();
t = dht.readTemperature();
Serial.print("Humidity : ");
Serial.print(h);
Serial.print(" ");
Serial.print("Temperature : ");
Serial.print(t);
 Blynk.virtualWrite(V10,h);
Blynk.virtualWrite(V11,t);
////////////////////////////////////////////////
////ตรวจสอบความชื้นในดินเพื่อสั่งการทำงานของ Relay////
 if( soilMoistureValue > 500 ) {
 digitalWrite(Pump, LOW);
 PumpBlynk.on();
Serial.println("Test Pump on");
 LINE.notify("ปั้มน้ำทำงาน");
 LINE.notify("อุณหภูมิ ");
 LINE.notify(t);
 }
 else{
 digitalWrite(Pump, HIGH);
 PumpBlynk.off();
 Serial.println("Test Pump off");
 }
////////////////////////////////////////////////
///////////แสดงผลอุณหภูมิบนจอLCD/////////////////
Serial.println(" LCD : Run");
 lcd.setCursor(0,0);
 lcd.print("Temp : ");
 lcd.print(t);
 lcd.print(" C");
/////////////////////////////////////////////////
}
delay(2000);
}
void loop()
{
 Blynk.run();
 soilsenser(); }
