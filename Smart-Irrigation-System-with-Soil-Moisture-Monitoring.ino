#include <avr/wdt.h>

#define analogIn1 A5
#define analogIn2 A4
#define analogIn3 A3
#define analogIn4 A2
#define Power 7
#define Relay 8
#define ledR 3
#define ledG 4
#define ledB 5
float avgmoisture = 0;
float avg = 0;
float moisture1 = 0;
float moisture2 = 0;
float moisture3 = 0;
float moisture4 = 0;
bool state = false;
unsigned long lastCheck = 0;
const unsigned long interval = 20000; // 20s

void setup() {
  // put your setup code here, to run once:
  pinMode(analogIn1, INPUT);
  pinMode(analogIn2, INPUT);
  pinMode(analogIn3, INPUT);
  pinMode(analogIn4, INPUT);
  pinMode(Power, OUTPUT);
  pinMode(Relay, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);
  wdt_enable(WDTO_8S);
  Serial.begin(9600);
  digitalWrite(Power, LOW);
  digitalWrite(Relay, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  wdt_reset();
  if(millis() - lastCheck >= interval){   // Run the main program every 20s
    lastCheck = millis();
    // main code
    
    avg = baca();
    Serial.print("\nAverage Sensor  : ");
    Serial.println(avg);
    if (avg >= 500){
      Serial.println("Tanah kering...");  //Dry Soil, valve state = on
      state = true;
      digitalWrite(ledR, HIGH);
      digitalWrite(ledG, LOW);
      digitalWrite(ledB, LOW);
    }
    
    else if(avg <= 370){          
      Serial.println("Tanah basah...");   //Wet soil, valve state = off
      state = false;
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, LOW);
      digitalWrite(ledB, HIGH);
    }

    else{
      Serial.println("Tanah perfect..."); //Perfect soil
      digitalWrite(ledR, LOW);
      digitalWrite(ledG, HIGH);
      digitalWrite(ledB, LOW);
    }
    delay(500);
    digitalWrite(Relay, state);           //Switch the relay state to open/close the water valve
  }
  delay(500);
}

float baca(){
  digitalWrite(Power, HIGH);  // Turn on the moisture sensor
  delay(1000);
  avgmoisture = 0;
  moisture1 = 0;
  moisture2 = 0;
  moisture3 = 0;
  moisture4 = 0;
  for(int i = 0; i < 10; i++){
    wdt_reset();
    moisture1 += analogRead(analogIn1);
    Serial.print("\nSensor 1  : ");
    Serial.println(analogRead(analogIn1));
    moisture2 += analogRead(analogIn2);
    Serial.print("Sensor 2  : ");
    Serial.println(analogRead(analogIn2));
    moisture3 += analogRead(analogIn3);
    Serial.print("Sensor 3  : ");
    Serial.println(analogRead(analogIn3));
    moisture4 += analogRead(analogIn4);
    Serial.print("Sensor 4  : ");
    Serial.println(analogRead(analogIn4));
    delay(1000); 
  }
  avgmoisture = ((moisture1 / 10) + (moisture2 / 10) + (moisture3 / 10) + (moisture4 / 10)) / 4;
  digitalWrite(Power, LOW);   // Turn the sensor OFF
  delay(500);
  return avgmoisture;         // Return analog moisture value
}