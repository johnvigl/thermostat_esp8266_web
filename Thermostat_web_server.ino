#include "ESP8266WiFi.h" // Import required libraries
#include "DHT.h"
#include "ESP8266WebServer.h"

#define DHTPIN 13 // DHT22 sensor pin (pin D7)
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE, 15);

const char* ssid = "wifi SSID"; // WiFi parameters and login credentials
const char* password = "wifi password";
const char* username = "webpage login name";
const char* pass = "webpage login password";

#define LISTEN_PORT 80 // The port to listen for incoming TCP connections 

int ledPin = 2; // The pin assigned to an onboard LED (inversed polarity) 
int powerPin = 0; // The pin connected to the relay at the boiler (pin D3)
int led_startPin = 16; // The pin assigned to another onboard LED (inversed polarity) (pin D0)

float temperature; // Variables to be exposed to the API
float temp_high;
float humidity;
unsigned int variable = 20; // The variable you want to control (default desired temperature 20 °C)
bool power = 0; // start ESP8266 with the central heating system at "off"
String check_status = digitalRead(ledPin) == LOW ? "checked" : "";
String powerstatus = "Όχι";

ESP8266WebServer server(LISTEN_PORT); //Start an instance of the Server

void setup(void) { 
  pinMode(ledPin, OUTPUT); // led and relay pins are set as OUTPUT pins
  pinMode(powerPin, OUTPUT);
  pinMode(led_startPin, OUTPUT);
  dht.begin();  // Initialize DHT sensor 
  WiFi.begin(ssid, password);  // Connect to WiFi
  server.on("/", handleRoot);
  server.begin();
  digitalWrite(led_startPin, HIGH); // HIGH because polarity is inversed
  digitalWrite(ledPin, HIGH); // HIGH because polarity is inversed
  digitalWrite(powerPin, LOW); // let's wait to check the temperature before power on
}        

void handleRoot() {
  if(!server.authenticate(username, pass)){ // http (not https), any improvements are welcome! 
    return server.requestAuthentication();
  }
    String html = "<html><head>"; 
    html += "<meta charset='UTF-8'>";
    //styles
    html += "<style>h1{text-align:center;}</style>";
    html += "<style>h2{text-align:center;}h3{font-size:40%;}</style>";
    html += "<style>.temp{font-size:150%;color:#FF0000;}</style>";
    html += "<style>.hum{font-size:150%;color:#0000FF;}</style>";
    html += "<style>.btn{width:350px;font-size:30px}</style>";
    html += "<style>.open{font-size:150%;color:#006d70;}</style>";
    html += "<style>input.chkbox{height:100px;width:100px;box-sizing:border-box;background:#006d70;}</style>";
    html += "<style>input.num{height:100px; width:140px;color:#FF6666;font-size:400%;}</style>";
    html += "</head>";
    html += "<body>";
    html += "<h3><br></h3>";
    html += "<h1>Ρύθμιση θερμοστάτη και λειτουργίας</h1>"; // adsust thermostat and function
    html += "<form method='post'>";
    html += "<h3><br></h3>";
    html += "<h2>Τρέχουσες συνθήκες:</h2>"; // current conditions
    html += "<h2 class=temp>Θερμοκρασία : " + String(temperature).substring(0,4) + " °C</h2>"; //temperature
    html += "<h2 class=hum>Υγρασία: " + String(humidity).substring(0,2) + " %</h2>"; //humidity
    html += "<h3><br></h3>";
    html += "<h2><label for='variable'>Επιθυμητή θερμοκρασία: </label></h2>"; //desired temperature

    if (server.method() == HTTP_POST) { // run the same code again asif button was pressed twice
      if (server.hasArg("variable")) {
        variable = server.arg("variable").toInt();
        Serial.print("variable=");
        Serial.println(variable);
      }
      if (server.arg("led") == "on") {
        power=1;
        digitalWrite(ledPin, LOW);
        } else {
        power=0;
        digitalWrite(ledPin, HIGH);
        }
      Serial.print("power=");
      Serial.println(power);
      if ((power==0) || (variable < temperature)) {
        powerstatus = "ΟΧΙ"; // power is OFF ("NO")
        digitalWrite(led_startPin, HIGH); 
        digitalWrite(powerPin, LOW);
      }
      else if ((power==1) && (variable >= temp_high)) {
        powerstatus = "ΝΑΙ"; // power is ON ("YES")
        digitalWrite(led_startPin, LOW);
        digitalWrite(powerPin, HIGH);
      }
    }
    check_status = digitalRead(ledPin) == LOW ? "checked" : ""; // update the check status variable
    html += "<h1 class=temp><input class=num type='number' min='15' max='25' name='variable' value='" + String(variable) + "'></h1>";
    html += "<h1><label class=open for='led'>Αναμμένο:</label></h1>"; //select to start
    html += "<h1><input type='checkbox' class=chkbox name='led' value='on' " + check_status + "></h1>";
    html += "<h2><label for='power'>Καυστήρας σε λειτουργία: </label>"+ String(powerstatus)+"</h2>"; // is boiler "on"? (yes/no)
    html += "<h3><br></h3>";
    html += "<h2><input type='submit' class=btn value='Αποστολή/Ανανέωση'></h2>"; // send and refresh (automatic refresh would be better)
    html += "</form>";
    html += "</body></html>";
    server.send(200, "text/html", html);
}

void loop() {
  temperature = dht.readTemperature();
  temp_high = temperature + 0.5; // 0.5 °C threshold to start and stop boiler
  humidity = dht.readHumidity(); // humidity is being read, a more sophisticated algorithm would use it to determine if heating is needed
  server.handleClient();
  if ((power==0) || (variable < temperature)) {
    powerstatus = "ΟΧΙ";
    digitalWrite(led_startPin, HIGH);
    digitalWrite(powerPin, LOW);
  }
  else if ((power==1) && (variable >= temp_high)) {
    powerstatus = "ΝΑΙ";
    digitalWrite(led_startPin, LOW);
    digitalWrite(powerPin, HIGH);
  }
}
