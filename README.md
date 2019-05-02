# ESP8266-SHT30-MQTT

  Simple ESP8266 code that reads temperature and humidity from a SHT30 sensor and publish the values over MQTT.
  Developed solely for the LOLIN D1 mini with a SHT30 addon but should work on any ESP8266 with a SHT30 attached.

  To prevent that the relatively high-powered ESP8266 chip heats up the SHT30 sensor the ESP8266 is put into deep-sleep between cycles.
  The ESP8266 wakes up aprox every 50 seconds, and then reads the SHT30 chip before connecting to wifi/MQTT to further prevent the ESP from affecting the sensor.

  Please note that MQTT is unencrypted so credentials will be sent in the clear over the MQTT TCP connection.

  ## Cofiguration

  All configurable settings can be adjusted in th setup.h file before building.