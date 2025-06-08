#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include "DFRobotDFPlayerMini.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
String message1 = "Nurse A,B and C  are to go to ward 012 ,015 and 022 to give the dosage to the patients";
String message2 = " Patient form ward 32A is calling for  assistance .Please check on the patient immediately.";
String message3 = "Train 789 is arriving in 5 minutes Please be ready in platform C.";

// WiFi credentials
const char* ssid = "Xiaomi 11i";
const char* password = "12345678";

// AWS IoT details
const char* mqtt_server = "a1xnali8ha38jo-ats.iot.us-east-1.amazonaws.com";
const char* mqtt_topic = "esp32/flight/audio";

WiFiClientSecure net;
PubSubClient client(net);

// DFPlayer
#define FPSerial Serial1
DFRobotDFPlayerMini myDFPlayer;

// Certificate literals
const char* ca_cert = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

const char* client_cert = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAL9piBamk6kaDA1dB3N0idlkgk9oMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yNTA0MTcwNjAz
NDRaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC3lkLdXahT5mFKZ3SD
ghRCUX5E4YiR6Bi5EQdGJx6hQ1lpfHV6HWsUuThrHwLgyDOH4KZezWE21a07LH4m
AGrwPNHnKecHxAwhivHCNxWgM2DEECABr/4Wfxr7szpOIBgbAV55eS9VPJbsjg5E
p9/EGrxGvXHTvyXbn6DJHs8AdrmbqOzsuWZcNbu+HDJluBU6vDyyyffAv9zFr48W
Mo5wW0L03qfsfz3DWixlBTh6snK0APDvcwtb2/uS6ppwbAFCbqKtCOWpgVt6IYyM
LsuovO72lWBo6fGOQArWHOYmh2WKf8wKWKnc3sJPpinMNNEOPaVGN/G5CHcYODdX
bFJZAgMBAAGjYDBeMB8GA1UdIwQYMBaAFKN7BLnw88KGM9p8A9lGOe9RyihgMB0G
A1UdDgQWBBRqFXWPxZ421EiabVh4008RoCmM0TAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAm7j+dqdLyCxw25WykNnDTxpx
r5wQtevZAyrpIw1QyIv67na9D+IN9+v8nLCkwUk3vXT3eHo+LSkb6NXseQo08SPE
EqFRNwwC9M9BKXyGWM7hEnHwPmERbxVhBokq3W50KLXUhBmJ2ognfoEgBzhtdkyb
AP+RBRkLejHWMmBp7NWnvXHpTJJREJpxvsUH17J36EBpVQyT28B93UKs3s7DUMcE
MO/d5Ti53tbZWotd9H6lj1CwSaGGIh/O2MwjJcJT9yI6adinKUNY73t6ZIy0+Zwk
NPZFrWLii3NITlCQrZJnpkZDMBb2X4TI1uRlQOVbBshc6poW9QZ0oR1SF4oKzw==
-----END CERTIFICATE-----
)KEY";

const char* private_key = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAt5ZC3V2oU+ZhSmd0g4IUQlF+ROGIkegYuREHRiceoUNZaXx1
eh1rFLk4ax8C4Mgzh+CmXs1hNtWtOyx+JgBq8DzR5ynnB8QMIYrxwjcVoDNgxBAg
Aa/+Fn8a+7M6TiAYGwFeeXkvVTyW7I4ORKffxBq8Rr1x078l25+gyR7PAHa5m6js
7LlmXDW7vhwyZbgVOrw8ssn3wL/cxa+PFjKOcFtC9N6n7H89w1osZQU4erJytADw
73MLW9v7kuqacGwBQm6irQjlqYFbeiGMjC7LqLzu9pVgaOnxjkAK1hzmJodlin/M
Clip3N7CT6YpzDTRDj2lRjfxuQh3GDg3V2xSWQIDAQABAoIBAEBb9f7iuYMILa1G
zWrY7D8/kHOXWShFZH1kgddPGYbRNIQtdIWl0IdIU4YCLiEdLiuBTvHHMEBn8j3f
YC30UYj82V0frHVlfhc4PX6njvZFbLPvLzlIOFwsnQoGD/eZernk5fjRyg0om4Q8
XLYm3+RAtyZZfZEM3rm6oURAf7MaU67490HRdzRlbahSV7uzYsoNMBBaJMRLg0kq
6NlVYqEOqfDzziBhmuVUFtaFus7BV7QostIGeLtdrJAyI4p8ij+lDskM+gd/V5fN
lJl100PgxQCekZHgxfdIJbe8t+oNaIPYKsLpj0jFjRER1cKiyGvtKbA6elnmpTeQ
M20CLl0CgYEA51UqQRhM7H5iQcumU3htvAhZba5OKcRYU65929oj/ChEkfFXe2Jf
OG8BEN9ewrT8iTXX6lfL0uZxXvw3cytyRttziD6rE0Cc+EXXIUQUd99JGjy4B/UP
Z9bhTMp9mNjXYdexJydNnMqOvmjEJfpbApoGdrcRsYsuOc0/6/fYR6sCgYEAyynA
PhbY+IOkdEdBVUV7e0nDZvCre3WZSqmYCHjtEPH0GAfo7Tm8DtW2lJnvOnSDYCIQ
CIMR1csQjvOe7r9aajrGZOr3EIB6DZ3i/U3kaVy5HdyC91n3OHmx4H0BA8xz5OLy
UNZ4hj43TYk7GfodKZptj9FvQA1k8MZQL9wtugsCgYEAyixILXPLZrRdsxsEVdbe
MIMrhQvdC+upNhk9rbLL2EhpBRNjXVXv2iZJTDGrY4dBXblM5mWsoLj9RAIxg9no
mbyZD+2VHdJOXTzMcypIqSrffKqsO2gU+q91X+NoOPjp43zaOlnDHtB4SDjPlFFk
0LVECOSd9gipcaLLxrJottkCgYAkblAy0hYGOlT7JOvqRpNmAEwx4WZetS/P26mP
m1+ncju7M2/pWjkGz/v1mcT0ToBgdLs7S4WfONK3T6hoV5f3BKMe5aN4KhG4718M
3TLgUehAGpLBmDhkwC3Ne5z6+Wau3KNnK45hRDbWiI04k0k3dKHA8GVqrvwy0N0A
OBNbEQKBgCRZGlZELCYM8vWbrlzSMqiVKSvh89eapPsWPmWBPtegnNe7mb8WBYmw
8/LontjWoJXIkxqtqsPlomZMoPmx5heIB+Kqjc0Pjzdt9gOOITfnBTqTGgQwjff5
6gVTLYT4MEaR1a04AVZStlcQOybTE9KebEnKdDAgYnLebHITB+YQ
-----END RSA PRIVATE KEY-----
)KEY";

// Connect to WiFi
void connectToWiFi() {
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println(" connected!");
}

// Reconnect to MQTT if disconnected
void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// Callback when message arrives
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.println(topic);
  String message = "";

  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message: ");
  Serial.println(message);

  // Convert to int and play
  int trackNum = message.toInt();
  if (trackNum >= 1 && trackNum <= 300) {  // assuming max 300 tracks
    myDFPlayer.play(trackNum);
    Serial.print("Playing track ");
    Serial.println(trackNum);
  } else {
    Serial.println("Unknown command.");
  }
}
void setup() {
  Serial.begin(115200);
  delay(1000);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
 

  // Initialize DFPlayer
  FPSerial.begin(9600, SERIAL_8N1, 16, 17);
  if (!myDFPlayer.begin(FPSerial)) {
    Serial.println("DFPlayer not responding.");
    while (true);
  }
  myDFPlayer.volume(40);
  Serial.println("DFPlayer initialized.");
  lcd.print("DFPlayer Ready!");
  

  connectToWiFi();

  // Set certs
  net.setCACert(ca_cert);
  net.setCertificate(client_cert);
  net.setPrivateKey(private_key);

  // MQTT setup
  client.setServer(mqtt_server, 8883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (myDFPlayer.available()) {
    uint8_t type = myDFPlayer.readType();
    int value = myDFPlayer.read();
    if (type == DFPlayerPlayFinished) {
      Serial.print("Finished playing track ");
      Serial.println(value);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Finished Track  ");
      lcd.setCursor(0, 1);
      lcd.print(value);
    if(value==1){
      scrollMessage(message1);
    }
    if(value==2){
      scrollMessage(message2);
    }
    if(value==3){
      scrollMessage(message3);
    }
    
      
    }
  }
}

void scrollMessage(String message) {
  int messageLength = message.length();
  
  for (int i = 0; i < messageLength - 16; i++) {
    lcd.clear();               // Clear the LCD screen
    lcd.setCursor(0, 0);       // Set cursor to the first position

    // Print a substring of the message (16 characters at a time)
    lcd.print(message.substring(i, i + 16));
    
    delay(300); // Adjust this delay for the scrolling speed
  }
}
