# thermostat_esp8266_web

Arduino code for ESP8266 with wifi and an additional DHT22 sensor for a web thermostat application running on a NODE MCU board. 

A low cost NODE MCU ESP8266 board with a low cost additional DHT22 sensor are used as a thermostat to control the central heating of a house.
A relay is also needed to use the low voltage from the board to control the high voltage which drives the heating system.

The server runs solely on the board and is accessible by any browser. The protocol is http (not https) and a simple username and password authentication is used. After logging in, user can determine if heating is ON or OFF and the desired temperature.

Interface text is in greek, though the comments in the code are in english, so any translation would be quite easy.
