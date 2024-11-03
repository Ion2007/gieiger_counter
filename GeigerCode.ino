
//import libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//define screen demensions
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 

//create display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);



volatile unsigned long counts = 0; // every time gieger count clicks
unsigned long cpm = 0;             //Counts per minute (CPM)
unsigned long previousMillis;                            // Time measurement
const int inputPin = 2;                                  // geiger data (on arduino uno use ports 2 or 3)

unsigned long minutes = 1;
unsigned long start = 0;

#define LOG_PERIOD 2000 //Logging period in milliseconds
#define MINUTE_PERIOD 60000

void ISR_impulse() { //runs on ever click
  counts++;
  digitalWrite(3,HIGH);
  delay(5);
  digitalWrite(3,LOW);
  
}

void setup() {
  //display set up code
  display.clearDisplay();
  int (13, OUTPUT);

  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  

  pinMode(inputPin, INPUT);                                                // Set pin for capturing Tube events
  interrupts();                                                            // Enable interrupts
  attachInterrupt(digitalPinToInterrupt(inputPin), ISR_impulse, FALLING); // Define interrupt on falling edge
  unsigned long clock1 = millis();
  start = clock1;
}

void loop() {

  unsigned long currentMillis = millis();
  
  
  
  if (currentMillis - previousMillis > LOG_PERIOD) {
    
    previousMillis = currentMillis;
    minutes = (previousMillis - start) / MINUTE_PERIOD; //determines minutes have passed
    if (minutes < 1) {
      minutes = 1;
  }


  

    cpm = counts / minutes; //cpm math
    // conversion to sieverts and Gray (micro/hour)
    float sieverts = cpm / 151.0f; 
    float Gray = sieverts;

    display.clearDisplay();

    display.setTextSize(1.5);
    display.setTextColor(WHITE);
    display.setCursor(0, 20);
    //make lines to display
    String line0 = "Counts: "+ String(counts);
    String line1 = "CPM: " + String(cpm);
    String line2 = "uSv/h: " + String(sieverts);
    String line3 = "uGy/h: " + String(Gray);
    //display lines
    display.println(line1);
    display.println(line2);
    display.println(line3);
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println(line0);
    display.display();
    
  }
}