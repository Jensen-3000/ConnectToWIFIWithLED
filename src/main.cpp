#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const char *ssid = "e307";
const char *password = "rockyCartoon544";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define OLED_ADDRESS 0x3C
#define PING_INTERVAL 2000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
IPAddress routerIP;

int pingCount = 0;

// Initialize the SSD1306 / OLED-091
void initializeDisplay()
{
    display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
    display.clearDisplay();
}

// Connect to the WiFi network
void connectToWiFi()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(100);
    }
    routerIP = WiFi.gatewayIP();
    Serial.print("Connected to WiFi. Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Router IP: ");
    Serial.println(routerIP);
}

// Update the display text with the local IP, router IP, ping result, and ping count
void updateDisplayText(int pingResult, int pingCount)
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // Shows local IP
    display.setCursor(0, 0);
    display.print("IP: ");
    display.println(WiFi.localIP());

    // Shows router IP
    display.setCursor(0, 10);
    display.print("R-IP: ");
    display.println(routerIP);

    // Shows ping result and ping count on the same line
    display.setCursor(0, 20);
    display.print("Ping: ");
    display.print(pingResult);
    display.print(" ms Count: ");
    display.println(pingCount);

    display.display();
}

// Perform a ping to the router
int performPing()
{
    bool pingSuccess = Ping.ping(routerIP);
    if (pingSuccess)
    {
        int pingTime = Ping.averageTime();
        Serial.print("Ping successful. Time: ");
        Serial.print(pingTime);
        Serial.println(" ms");
        return pingTime;
    }
    else
    {
        Serial.println("Ping failed.");
        return -1;
    }
}

void setup()
{
    Serial.begin(115200);

    initializeDisplay();

    connectToWiFi();

    updateDisplayText(0, pingCount);

    display.display();
}

void loop()
{
    int pingResult = performPing();

    pingCount++;

    updateDisplayText(pingResult, pingCount);

    delay(PING_INTERVAL);
}