#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Public Mosquitto broker
const char* mqtt_server = "test.mosquitto.org";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

// Relay pins
#define RELAY1 D1
#define RELAY2 D2
#define RELAY3 D5
#define RELAY4 D6

void setup_relays() {
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);

  digitalWrite(RELAY1, HIGH);
  digitalWrite(RELAY2, HIGH);
  digitalWrite(RELAY3, HIGH);
  digitalWrite(RELAY4, HIGH);
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  String cmd = String((char*)payload);
  String topicStr = String(topic);

  if (topicStr.endsWith("1")) digitalWrite(RELAY1, cmd == "ON" ? LOW : HIGH);
  else if (topicStr.endsWith("2")) digitalWrite(RELAY2, cmd == "ON" ? LOW : HIGH);
  else if (topicStr.endsWith("3")) digitalWrite(RELAY3, cmd == "ON" ? LOW : HIGH);
  else if (topicStr.endsWith("4")) digitalWrite(RELAY4, cmd == "ON" ? LOW : HIGH);
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("NodeMCUClient123")) {
      client.subscribe("lucifer123/home/relay1");
      client.subscribe("lucifer123/home/relay2");
      client.subscribe("lucifer123/home/relay3");
      client.subscribe("lucifer123/home/relay4");
    } else {
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_relays();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
}
