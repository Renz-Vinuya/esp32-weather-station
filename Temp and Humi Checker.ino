#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define DHTPIN 2
#define DHTTYPE DHT22

#define RED_PIN 13    // Top wire in your photo
#define GREEN_PIN 12  // Third wire from top
#define BLUE_PIN 14   // Fourth wire from top

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

void setup(){
  Serial.begin(115200);

  dht.begin();

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
      Serial.println("OLED ALLOCATION FAILED!");
      for(;;);
  }

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

}

void setRGB(int r, int g, int b){
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

void loop(){
  delay(2000);

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  float coldTemp = 22;
  float hotTemp = 30;

  if(isnan(humidity) || isnan(temperature))
  {
    Serial.println("Failed to read form DHT sensor");
    return;
  }

  if(temperature < coldTemp){
    setRGB(0, 0, 255);
  } else if(temperature > hotTemp){
    setRGB(255, 0, 0);
  } else {
    setRGB(0, 255, 0);
  }

  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.print(F("%  Temperature: "));
  Serial.print(temperature);
  Serial.println(F("°C "));

  display.clearDisplay();     
  
  display.setTextSize(1);       
  display.setCursor(0,0);       
  display.println(F("ROOM STATUS"));
  
  display.setTextSize(2);       
  display.setCursor(0, 20);
  display.print(temperature, 1);          
  display.print(" C");

  display.setCursor(0, 45);
  display.print(humidity, 0);
  display.print(" % Hum");

  display.display();
}