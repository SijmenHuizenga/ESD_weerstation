/*
 * Controllers.cpp
 *
 *  Created on: 1 dec. 2014
 *      Author: Sijmen
 */

#include "Controllers.h"
#include "arduino.h"

//PotentioMeter
PotentioMeter::PotentioMeter(unsigned int analogPin) {
	this->pin = analogPin;
	this->callback = 0;
	this->lastVal = -1;
}

/**
 * value is tussen 0 en 99
 */
int PotentioMeter::getValuePersentage() {
	return map(analogRead(pin), 0, 1023, 0, 99);
}

int PotentioMeter::getRawValue() {
	return analogRead(pin);
}
void PotentioMeter::setValueChangeCallback(void(*call)(void)){
	this->callback = call;
}
void PotentioMeter::update(){
	if(this->lastVal != this->getValuePersentage()){
		this->lastVal = this->getValuePersentage();
		this->callback();
	}
}

//LightDependentResistor
LightDependentResistor::LightDependentResistor(unsigned int analogPin) {
	this->pin = analogPin;
	this->callback = 0;
	this->lastVal = -1;
}
void LightDependentResistor::setValueChangeCallback(void(*call)(void)){
	this->callback = call;
}

/**
 * We gaan er van uit dat lux tussen 0 en 100 gaat.
 */
int LightDependentResistor::getLuxValue() {
	return map(constrain(analogRead(this->pin),this->min, this->max), this->min, this->max, 100, 0);
}

int LightDependentResistor::getRawValue() {
	return analogRead(this->pin);
}

void LightDependentResistor::update(){
	if(this->lastVal != getLuxValue()){
		this->lastVal = getLuxValue();
		this->callback();
	}
}

//Thermometer
Thermometer::Thermometer(unsigned int analogPin) {
	this->pin = analogPin;
	this->callback = 0;
	this->lastVal = -1;
}

void Thermometer::update(){
	if(this->lastVal != getCurDegrees()){
		this->lastVal = getCurDegrees();
		this->callback();
	}
}

void Thermometer::setValueChangeCallback(void(*call)(void)){
	this->callback = call;
}

int Thermometer::getCurDegrees() {
	return map(analogRead(this->pin), this->deg0, this->deg22, 0, 22);
}

int Thermometer::getRawValue() {
	return analogRead(this->pin);
}

//button
Button::Button(int digitalPin) {
	this->pin = digitalPin;
	this->state = false;
	this->callback = 0;
	pinMode(pin, INPUT);
}
void Button::update() {
	if(this->callback == 0)
		return;
	int s = digitalRead(pin);
	if (s == HIGH && this->state == false) {
		this->state = true;
		this->callback();
	} else if (s == LOW && this->state == true) {
		this->state = false;
		this->callback();
	}
}
void Button::setCallback(void ( *callback )( void )) {
	this->callback = callback;
}

bool Button::getState(){
	return state;
}
