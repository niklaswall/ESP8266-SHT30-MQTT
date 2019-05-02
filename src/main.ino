/*
  Simple ESP8266 code that reads temperature and humidity from a SHT30 sensor and publish the values over MQTT.
  Developed solely for the LOLIN D1 mini with a SHT30 addon but should work on any ESP8266 with a SHT30 attached.

  To prevent that the relatively high-powered ESP8266 chip heats up the SHT30 sensor the ESP8266 is put into deep-sleep between cycles.
  The ESP8266 wakes up aprox every 50 seconds, and then reads the SHT30 chip before connecting to wifi/MQTT to further prevent the ESP from affecting the sensor.

  Please note that MQTT is unencrypted so credentials will be sent in the clear over the MQTT TCP connection.
*/
#include <ESP8266WiFi.h>
#include <WEMOS_SHT3X.h>
#include <PubSubClient.h>
#include "setup.h"

extern "C" {
#include "user_interface.h"
}


WiFiClient wifi;
PubSubClient mqtt(wifi);
SHT3X sht30(0x45);

void doDeepSleep()
{
#ifdef DEBUG
  Serial.print("Going into deep sleep for ");
  Serial.print(PUBLISH_RATE);
  Serial.println(" seconds");
#endif
  ESP.deepSleep(PUBLISH_RATE * 1e6);
}

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
#endif
  //TODO: Make sure that wifi is really off (the ESP8266 defaults to the last state)

  // reading SHT30 sensor first, to lower risk of ESP8266 heatup affecting the sensor.
  sht30.get();
  float t = sht30.cTemp;
  float h = sht30.humidity;

#ifdef DEBUG
    Serial.println();
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.print(" | Humidity: ");
    Serial.println(h);
#endif

  // Try to connecto to wifi for ~10 seconds, then enter deep sleep if not succeeding
  int retryCountWIFI = 0;
  WiFi.hostname(MQTT_CLIENT_NAME);
  wifi_station_set_hostname(MQTT_CLIENT_NAME);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
#ifdef DEBUG
    Serial.println("connecting Wifi");
#endif
    delay(500);

    retryCountWIFI++;
    if (retryCountWIFI > 20)
    {
#ifdef DEBUG
      Serial.println("Unable to connect to WIFI, sleeping...");
#endif
      ESP.deepSleep(PUBLISH_RATE * 1e6);
    }
  }



  #ifdef DEBUG
    Serial.print("Connected to WIFI, IP address: ");
    Serial.println(WiFi.localIP());
    
    Serial.print("Host Name: ");
    Serial.println(WiFi.hostname());
  #endif
  
  delay(2000);
  yield();


  // Connect to MQTT server, bail if taking more than ~10 seconds.
  int retryCountMQTT = 0;
  mqtt.setServer(MQTT_SERVER, 1883);
  while (!mqtt.connect(MQTT_CLIENT_NAME, MQTT_USERNAME, MQTT_PASSWORD))
  {

#ifdef DEBUG
      Serial.println("connecting MQTT");
#endif

    delay(500);
    mqtt.loop();
    retryCountMQTT++;
    if (retryCountMQTT > 20)
    {
#ifdef DEBUG
      Serial.println("Unable to connect to MQTT!");
#endif
      doDeepSleep();
    }
  }


  // Publish sensor readings to MQTT
  mqtt.publish(MQTT_TOPIC_TEMP, String(t).c_str(), false);
  mqtt.publish(MQTT_TOPIC_HUMID, String(h).c_str(), false);

  // Make sure that we flush out the MQTT publish messages before entering deep sleep.
  for (int i = 0; i < 20; i++)
  {
    mqtt.loop();
    delay(10);
  }

  doDeepSleep();
}

void loop()
{
  // Normal operation is going into deep sleep before entering the main loop.
}
