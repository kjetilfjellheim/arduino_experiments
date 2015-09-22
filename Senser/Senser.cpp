// Do not remove the include below
#include "Senser.h"
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHT_PIN 8
#define LCD_ADRESS 0x20
#define BUTTON_PIN 2
#define MOTION_DETECTOR_PIN 3
#define SDA_PIN A4
#define SCL_PIN A5

#define NUM_SENSORS 4
#define SENSOR_CHANGE_TRIGGER_TIME 100
#define MAX_ON_TIME 60000
#define INIT_SENSOR 0

#define TEMPERATURE_SENSOR_NUM 0
#define HUMIDITY_SENSOR_NUM 1
#define SOIL_SENSOR_NUM 2
#define PRESSURE_SENSOR_NUM 3

LiquidCrystal_I2C lcd(LCD_ADRESS, 16, 2);
DHT dht(DHT_PIN, DHT22);

volatile unsigned int sensor = INIT_SENSOR;
volatile unsigned long lastTrigger;
volatile unsigned long lastMotionDetected;

//The setup function is called once at startup of the sketch
void setup() {
	Serial.begin(9600);
	lastMotionDetected = millis();
	lastTrigger = millis();
	pinMode(SDA_PIN, INPUT_PULLUP);
	pinMode(SCL_PIN, INPUT_PULLUP);
	lcd.init();
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), sensorChange, FALLING);
	attachInterrupt(digitalPinToInterrupt(MOTION_DETECTOR_PIN), motionDetected,
			CHANGE);
	dht.begin();
	delay(3000);
}

// The loop function is called in an endless loop
void loop() {
	delay(SENSOR_CHANGE_TRIGGER_TIME);
	unsigned long unchangedTime = millis() - lastMotionDetected;
	if (unchangedTime < MAX_ON_TIME) {
		lcd.display();
		lcd.backlight();
		switch (sensor) {
		case TEMPERATURE_SENSOR_NUM:
			setTemperature();
			break;
		case HUMIDITY_SENSOR_NUM:
			setHumidity();
			break;
		case SOIL_SENSOR_NUM:
			setSoilHumidity();
			break;
		case PRESSURE_SENSOR_NUM:
			setPressure();
			break;
		}
	} else {
		lcd.noDisplay();
		lcd.noBacklight();
	}
	delay(500);
}

void motionDetected() {
	lastMotionDetected = millis();
}

void sensorChange() {
	unsigned long currentTrigger = millis();
	if ((currentTrigger - lastTrigger) > SENSOR_CHANGE_TRIGGER_TIME) {
		sensor = (sensor + 1) % NUM_SENSORS;
		lastTrigger = currentTrigger;
	}
	lastMotionDetected = millis();
	Serial.println(sensor);
}

void setTemperature() {
	float newTemperature = dht.readTemperature(false);
	lcd.clear();
	lcd.print("Temperature");
	lcd.setCursor(0, 1);
	lcd.print(newTemperature);
	lcd.print(" C");
}
void setHumidity() {
	float newHumidity = dht.readHumidity();
	lcd.clear();
	lcd.print("Humidity");
	lcd.setCursor(0, 1);
	lcd.print(newHumidity);
	lcd.print(" %");
}
void setSoilHumidity() {
	lcd.clear();
	lcd.print("Soil moisture");
	lcd.setCursor(0, 1);
	lcd.print("Soil moisture");
}
void setPressure() {
	lcd.clear();
	lcd.print("Baromethric");
	lcd.setCursor(0, 1);
	lcd.print("10000");
	lcd.print(" kPa");
}
