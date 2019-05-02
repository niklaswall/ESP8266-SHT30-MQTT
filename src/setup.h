// SSID and PASSWORD for wifi connection
#define WIFI_SSID "default"
#define WIFI_PASSWORD "password"

//  MQTT Endpoint and credentials
#define MQTT_SERVER "mqtt1.iothingy.com"
#define MQTT_CLIENT_NAME "outdoor-sensor"
#define MQTT_USERNAME "username"
#define MQTT_PASSWORD "password"

// Topics for the temperature and humidity
#define MQTT_TOPIC_TEMP "home/sensor/outdoor/temperature"
#define MQTT_TOPIC_HUMID "home/sensor/outdoor/humidity"

// Publishing rate in seconds
#define PUBLISH_RATE 1 * 50 

// Debug to serial port
#define DEBUG false 