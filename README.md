# thermostat_esp8266_web

Arduino code for ESP8266 with wifi and an additional DHT22 sensor for a web thermostat application running on a NODE MCU board. 

A low cost NODE MCU ESP8266 board with a low cost additional DHT22 sensor are used as a thermostat to control the central heating of a house.
A relay is also needed to use the low voltage from the board to control the high voltage which drives the heating system.

The server runs solely on the board and is accessible by any browser. The protocol is http (not https) and a simple username and password authentication is used. After logging in, user can determine if heating is ON or OFF and the desired temperature.

Selecting the desired temperature, the progrm compares this value with the readings of the sensor. If the sensor's temprerature is higher than selected or if the user has switched off the heating system, then the boiler is set to OFF. If the system is powered on (the tick is selected and the submit button is clicked) then if the desired temperature is more than o.5 degrees Celsius lower than desired, then the boiler starts. when the sensor's temperature exceeds the desired temperature, the boiler stops.

For example, if you set the desired temp to 20 °C then the heating starts at 19.5 °C and ends at 20 °C.
If you also consider that the temperature also rises for some minutes after stopping the consumption of gas/oil etc, then the margin could be more than 0.5 °C (depending of the heating system or the house's thermal insulation, external conditions, etc).

Interface text is in greek, though the comments in the code are in english, so any translation would be quite easy.

