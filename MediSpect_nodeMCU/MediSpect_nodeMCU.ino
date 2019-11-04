#include <SoftwareSerial.h>
#include <dht11.h>
SoftwareSerial s(D6, D5);
dht11 DHT11;
#define DHT11PIN 0
int myBPM;
int sensorValue;
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "By9BSWe58Yf0bjAzN837U9rgHhGlTO7N";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Trikuldham";
char pass[] = "jaihanuman1981";

// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V1);
BlynkTimer timer;
// You can send commands from Terminal to your hardware. Just use
// the same Virtual Pin as your Terminal Widget
BLYNK_WRITE(V1)
{

  // if you type "Marco" into Terminal Widget - it will respond: "Polo:"
  if (String("Marco") == param.asStr()) {
    terminal.println("You said: 'Marco'") ;
    terminal.println("I said: 'Polo'") ;
  } else {

    // Send it back
    terminal.print("You said:");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }

  // Ensure everything is sent
  terminal.flush();
}
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.

}
void setup() {
  pinMode( 3, INPUT);
  s.begin(9600);
  Serial.begin(9600);
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // This will print Blynk Software version to the Terminal Widget when
  // your hardware gets connected to Blynk Server
  terminal.println(F("Blynk v" BLYNK_VERSION ": Device started"));
  terminal.println(F("-------------"));
  terminal.println(F("Type 'Marco' and get a reply, or type"));
  terminal.println(F("anything else and get it printed back."));
  terminal.flush();
  timer.setInterval(1000L, myTimerEvent);
}

void loop() {
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
  s.write("s");
  if (s.available() > 0)
  {
    myBPM = s.read();
    Serial.println(myBPM);
    terminal.print("BPM: ");
    terminal.println(myBPM / 4);
  }
  Blynk.virtualWrite(V1, myBPM / 4);
  sensorValue = analogRead(0); // read analog input pin 0
  Serial.print("Air Quality: ");
  Serial.println(sensorValue, DEC); // prints the value read
  Blynk.virtualWrite(V2, sensorValue);
  int chk = DHT11.read(DHT11PIN);
  Serial.print("Temperature: ");
  Serial.println(DHT11.temperature);
  Blynk.virtualWrite(V3, DHT11.temperature);
  Serial.print("Humidity: ");
  Serial.println(DHT11.humidity);
  Blynk.virtualWrite(V4, DHT11.humidity);
}
