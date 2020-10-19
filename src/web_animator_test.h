#include <Arduino.h>
#include <WebAnimator.h>

#define SDA_PIN 4
#define SCL_PIN 15

//U8G2_SSD1306_64X48_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, SCL_PIN, SDA_PIN);   // EastRising 0.66" OLED breakout board, Uno: A4=SDA, A5=SCL, 5V powered
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ 16, /* clock=*/ SCL_PIN, /* data=*/ SDA_PIN);

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;

const int FONT_WIDTH = 7;
const int FONT_HEIGHT = 14;

const int HTTP_TIMEOUT = 10000;


// Include required libraries to get data from your data panel connection page.
#include <WiFi.h>
//#include <ArduinoHttpClient.h>

//#include <ESP8266WebServer.h>
//#include <ESP8266HTTPClient.h>

#define HTTP 1
#define HTTPS 2

#define HTTP_PROTOCOL HTTP
#define HTTP_HOST "192.168.0.16"
#define HTTP_PORT 3000
#define HTTP_ENDPOINT "/tao"

#if HTTP_PROTOCOL == HTTP
#include <WiFiClient.h>
#include <HTTPClient.h>
const char *httpProtocol = "http";
#elif HTTP_PROTOCOL == HTTPS
#include <WiFiClientSecure.h>
const char *httpProtocol = "https";
#else
    #error Not supported http protocol !
#endif


// Define the WiFi settings.
#include <privateCredentials.h>
const char *ssid = WIFI_PRIVATE_SSID;
const char *password = WIFI_PRIVATE_PASSWORD;

const char *httpHost = HTTP_HOST;
const int httpPort = HTTP_PORT;
const char *httpEndpoint = HTTP_ENDPOINT;

const String webpage = String(httpProtocol) + "://" + String(httpHost) + ":" +  String(httpPort) + String(httpEndpoint);

WebAnimator webAnimator;

void setupWiFiBlocking(void) {
    //WiFi.mode(WIFI_OFF);
    //delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, password);

    Serial.print(F("Attempting to connect to Wi-Fi network named: "));
    Serial.println(ssid);

    // Halt the code until connected to WiFi.
    int k = 0;
    while (WiFi.status() != WL_CONNECTED) {
        k++;
        delay(1000);
        Serial.print(".");
        if (k % 10 == 0) {
            Serial.print("Wi-Fi current status: ");
            Serial.println(WiFi.status());
        }
    }
}

WiFiClient buildWiFiClient(const char* protocol) {
    #if HTTP_PROTOCOL == HTTP
    WiFiClient client;
    return client;
    #elif HTTP_PROTOCOL == HTTPS
    WiFiClientSecure client;
    client.setInsecure();
    return client;
    #else
        #error Not supported http protocol !
    #endif
}

bool connectWiFiClient(WiFiClient* client, const char* protocol, const char* host, int port) {
    Serial.print("Attempting to connect to Host host: ");
    Serial.print(host);
    Serial.print(" on port: ");
    Serial.print(port);
    Serial.print(" with protocol: ");
    Serial.print(protocol);
    Serial.println();

    client->setTimeout(HTTP_TIMEOUT);

    if(!client->connect(host, port)) {
        Serial.println("Connection Failed!");
        return false;
    }
    
    Serial.println("Connected.");

    return true;
}

void setup(void) {
    Serial.begin(115200);
    Serial.println();

    HTTPClient client;
    webAnimator.begin(client, u8g2);

    String message = "Setting up WiFi on " + String(ssid) + " ...";
    webAnimator.displayText(message);

    Serial.println(F(""));

    // print the SSID of the network you're attached to:
    Serial.print(F("SSID: "));
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print(F("IP Address: "));
    Serial.println(ip);
}

void loop(void) {
    webAnimator.displayText(F("HTTP query ..."));

    webAnimator.requestAnimation(webpage); //Specify the URL

    webAnimator.displayAnimationOnce();

    // deley between each page
    delay(20000);

}
