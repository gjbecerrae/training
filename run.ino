#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char *ssid = "SFR-f1e4";
const char *password = "XSN5F6ECYU4A";
const char *mqttServer = "192.168.0.30";
const int mqttPort = 1883;
const char *mqttUser = "";
const char *mqttPassword = "";
const char *mqttSubscribeTopic = "chips/garbage-bot";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// LEDS
int builtinLED = LED_BUILTIN;
int relayPin = 0; // esp8266:D3
int stopButtonPin = 2; //esp8266:D4

void setup()
{
  Serial.begin(115200);

  // BUILTIN LED
  pinMode(builtinLED, OUTPUT);
  digitalWrite(builtinLED, HIGH);
  // RELAY SIGNAL LED
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  // STOP BUTTON PIN
  pinMode(stopButtonPin, INPUT); 

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("[wifi] Connecting to ");
  Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println(" -> ok");

  mqttClient.setServer(mqttServer,  mqttPort);
  mqttClient.setCallback(callback);

  // Connect mqtt
  Serial.print("[mqtt] Connecting to MQTT");
  while (!mqttClient.connected())
  {
    if (mqttClient.connect("garbage-bot-ESP8266", mqttUser, mqttPassword))
    {
      Serial.println(" -> ok");
      mqttClient.publish("chips/", "hello I am Garabage-bot");
      mqttClient.subscribe(mqttSubscribeTopic);
    }
    else
    {
      Serial.print(".");
      delay(200);
    }
  }
}

void loop()
{
  mqttClient.loop();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("[mqtt] message (topic=");
  Serial.print(topic);
  Serial.println(")");
  while (stopButtonPin == HIGH){
    for (int i = 0; i <= 10; i++) {
      for (int j = 0; i <= 10; i++){
        activateRelay();
      }
      delay(600000);
    }
  }
}

void activateRelay()
{
  digitalWrite(relayPin, HIGH);
  delay(10000);
  digitalWrite(relayPin, LOW);
  delay(10000);
}
