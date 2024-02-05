
#include <ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "MKashirK"
#define AIO_KEY         "aio_inqg8626uoyawe1NbWHAC4UKBpH1"

#define ledPin 5 // Red LED
#define pirPin 4 // Input for HC-SR501

int pirValue = 0; // variable to store read PIR Value

// Set up the Wi-Fi client
WiFiClient client;

// Set up the Adafruit MQTT client
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Set up the MQTT topic for publishing
Adafruit_MQTT_Publish pirTopic = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/pir");

void setup() 
{
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(pirPin, INPUT);
  digitalWrite(ledPin, LOW);

  // Connect to Wi-Fi
  WiFi.begin("andAP", "Qwerty1024");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to Adafruit IO MQTT
  connectToAdafruitIO();
}

void loop() 
{
  pirValue = digitalRead(pirPin);
  Serial.println(pirValue);

  // Publish PIR sensor value to Adafruit IO with delay
  publishPIRValueWithDelay();

  if (pirValue == 1)
  {
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    digitalWrite(ledPin, LOW);
  }
}

void connectToAdafruitIO()
{
  int8_t ret;

  while ((ret = mqtt.connect()) != 0)
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying Adafruit IO connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
  }
  Serial.println("Connected to Adafruit IO MQTT");
}

void publishPIRValueWithDelay()
{
  // Add a delay before each data upload to comply with Adafruit IO's rate limits
  delay(2500); // Adjust the delay period as needed

  if (pirTopic.publish(pirValue == 1 ? "1" : "0"))
  {
    Serial.println("Successfully published to Adafruit IO");
  }
  else
  {
    Serial.println("Failed to publish to Adafruit IO");
  }
}
