#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

char isCelsius = true;

#define DHT_PIN 15
#define DHTTYPE DHT22

const int pins[] = {0, 2, 6, 7, 8, 9, 10, 11, 12, 14};
int cycles = 1;

DHT dht(DHT_PIN, DHTTYPE);

void barGraph(int nums) {
  for (int i = 0; i < 10; i++) {
    digitalWrite(pins[i], LOW);
  }

  for (int i = 0; i < nums; i++) {
    digitalWrite(pins[i], HIGH);
  }
}

float convertToF(float c) {
  return (c * 9) / 5 + 32;
}

void displayInfo(float t, float h) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);

  if (isCelsius) {
    display.println("Temperature: "+String(t)+" C");
  } else {
    display.println("Temperature: "+String(convertToF(t))+" F");
  }

  display.println("\nHumidity: "+String(h)+"%");

  display.display();

  int scaledValue = (t + 40) / 12 + 1;
  barGraph(scaledValue);
}

void setup() {
  Serial1.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial1.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  dht.begin();

  for (int i = 0; i < 10; i++) {
    pinMode(pins[i], OUTPUT);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.println("Bar Graph = Hot-o-meter. Just a basic temperature station. Made by pizzalover125.");
  display.display();

  delay(3000);
}

void loop() {
  delay(500);
  
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();

  if (cycles % 2 == 0) {
    isCelsius = false;
    displayInfo(temp, humidity);
  } else {
    isCelsius = true;
    displayInfo(temp, humidity);
  }

  cycles++;
}