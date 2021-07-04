#define BLYNK_PRINT Serial       // DEFINE  TO PRINT IN BLYNK               
#include <WiFi.h> //HEADER FILE FOR WIFI CONNECTIVITY
#include <WiFiClient.h> // HEADER FILE FOR WIFI SUPPORT
#include <BlynkSimpleEsp32.h> // HEADER FILE TO CONNECT BlYNK TO ESP BOARD

char auth[] = "ZnQdISFa9NxDt3T4zkFmybzbibjJe0IF";// authentication id given by BLYNK 
char ssid[] = "Mi 10i likhith";//USER NAME OF WIFI TO BE CONNECTED FOR THE ESP - 32 BOARD  
char pass[] = "likhith28"; // PASSWORD FOR THE WIFI 
volatile int flow_frequency; // Measures flow sensor pulses
// Calculated litres/hour
 float vol = 0.0,l_minute;
unsigned char flowsensor = 27;// Sensor Input
const int relayPin = 25;// relay output
unsigned long currentTime;
unsigned long cloopTime;
void flow () // Interrupt function
{
   flow_frequency++;
}
void setup()
{
   WiFi.begin(ssid, pass);
   Serial.begin(115200);
   pinMode(flowsensor, INPUT);
   pinMode(relayPin, OUTPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   digitalWrite(relayPin, HIGH);
   
   attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
   sei();//enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
   Blynk.begin(auth, ssid, pass);
}
void loop ()
{
  
  
   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0)
    {
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      
      Serial.print("Rate: ");
      Serial.print(l_minute);
      Serial.print(" L/M");
      Blynk.virtualWrite(V4,Rate);
      l_minute = l_minute/60;
  
      vol = vol +l_minute;
      Serial.print("Vol:");
      Serial.print(vol);
      Serial.print(" L");
      flow_frequency = 0; // Reset Counter
      Serial.print(l_minute, DEC); // Print litres/hour
      Serial.println(" L/Sec");
      
      Blynk.virtualWrite(V6,vol);
      if (vol >= 3 ) 
      {
      digitalWrite(relayPin, LOW);
      Serial.print("RELAY OFF");
      Blynk.notify("DEAR CUSTOMER TODAYS WATER LIMIT IS  EXHAUSTED");
      Blynk.email("17f21a0452@gmail.com","WATER","DEAR CUSTOMER TODAYS WATER LIMIT IS  EXHAUSTED");
      
      }
      else 
      {
      digitalWrite(relayPin, HIGH);
      Serial.print("RELAY ON");
      }
    }
   }
   
   for(int i=1; i<=10; i++) {
        String phdata = Serial.readStringUntil(':');
        Serial.println(phdata);
        if(phdata != "")
        {
            String ph = Serial.readStringUntil('$');
           
            Serial.println(ph);
            float phvalue=ph.toFloat();
             Serial.println();
            Serial.println("PH Value");
            Serial.println(phvalue);
            Blynk.virtualWrite(V4,phvalue);
            if(phvalue <= 7){
              Serial.println("ACIDIC");  
            }
            if(phvalue >= 7 && phvalue <=8 ){
              Serial.println("****PURE WATER*****");  
            }
            if(phvalue >=8 ){
              Serial.println("****BASE*****");  
            }
        }
    }
    Blynk.run();
 //delay(1000);
}  
