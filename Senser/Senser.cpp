// Do not remove the include below
#include "Senser.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT22.h>

#define DHT22_PIN 7
#define LCD_ADRESS 0x20
#define BUTTON_PIN 2
#define MOTION_DETECTOR_PIN 3
#define SDA_PIN A4
#define SCL_PIN A5

#define NUM_SENSORS 3
#define SENSOR_CHANGE_TRIGGER_TIME 500
#define MAX_ON_TIME 60000
#define INIT_SENSOR 0
#define TEMPERATURE_SENSOR_NUM 0
#define HUMIDITY_SENSOR_NUM 1
#define SOIL_SENSOR_NUM 2

LiquidCrystal_I2C lcd(LCD_ADRESS,16,2);
DHT22 dhtSensor(DHT22_PIN);
volatile unsigned int sensor = INIT_SENSOR;
volatile unsigned long lastTrigger;
volatile unsigned long lastMotionDetected;

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);
	lastMotionDetected = millis();
	lastTrigger = millis();
	pinMode(SDA_PIN, INPUT_PULLUP);
	pinMode(SCL_PIN, INPUT_PULLUP);
	lcd.init();
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), sensorChange, FALLING);
	attachInterrupt(digitalPinToInterrupt(MOTION_DETECTOR_PIN), motionDetected, CHANGE);
}

// The loop function is called in an endless loop
void loop()
{
	delay(SENSOR_CHANGE_TRIGGER_TIME);
	unsigned long unchangedTime = millis() - lastMotionDetected;
	Serial.println(unchangedTime);
	if (unchangedTime < MAX_ON_TIME) {
		lcd.backlight();
		lcd.clear();
		switch(sensor) {
			case TEMPERATURE_SENSOR_NUM:
				setTemperature();
				break;
			case HUMIDITY_SENSOR_NUM:
				setHumidity();
				break;
			case SOIL_SENSOR_NUM:
				setSoilHumidity();
				break;
		}
	} else {
		lcd.noBacklight();
	}
	delay(1000);
}

void motionDetected() {
	Serial.println("Motion detected");
	lastMotionDetected = millis();
}

void sensorChange() {
	unsigned long currentTrigger = millis();
	if ((currentTrigger - lastTrigger) > SENSOR_CHANGE_TRIGGER_TIME) {
		sensor = (sensor + 1) % NUM_SENSORS;
		lastTrigger = currentTrigger;
	}
	lastMotionDetected = millis();
}

void setTemperature() {
	lcd.print("Temperature ");
	lcd.print(dhtSensor.getTemperatureC());
	lcd.print(" celsius");
}
void setHumidity() {
	lcd.print("Humidity ");
	lcd.print(dhtSensor.getHumidity());
	lcd.print(" ");
}
void setSoilHumidity() {
	lcd.print("Soil moisture ");
}
