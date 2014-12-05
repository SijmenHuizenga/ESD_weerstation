#include "Controllers.h"
#include "LedControl.h"

//led controller
LedController* ledController;

//input devices
Button* buttons[4];
LightDependentResistor* ldr;
PotentioMeter* potMeter;
Thermometer* tempMeter;

//the new message id.
byte msgId = 0;

//time when the last message was send
unsigned long lastMessage = 0;
//do not send messages faster than this amount of mmilliseconds
const int maxUpdateSpeed = 100;
//do not send messages slower than this amount of milliseconds
const int minUpdateSpeed = 1000;

/**
 * the beginning of the program
 */
void setup() {
	ledController = new LedController();
	ledController->initilize();

	for(int i = 0; i < 4; i++){
		buttons[i] = new Button(13-i);
		buttons[i]->setCallback(valueChangeCallback);
	}

	ldr = new LightDependentResistor(0);
	ldr->setValueChangeCallback(valueChangeCallback);
	tempMeter = new Thermometer(1);
	tempMeter->setValueChangeCallback(valueChangeCallback);
	potMeter = new PotentioMeter(2);
	potMeter->setValueChangeCallback(valueChangeCallback);
	Serial.begin(9600);
}

/**
 * keep spinning
 */
void loop() {
	for(int i = 0; i < 4; i++){
		buttons[i]->update();
	}
	ldr->update();
	tempMeter->update();
	potMeter->update();
	int curMil = millis();
	if(curMil - lastMessage > minUpdateSpeed){
		printDataMessage();
		lastMessage = curMil;
	}
}

/**
 * this funciton is called wenever a value of the
 * input components is changed
 */
void valueChangeCallback(){
	int curMil = millis();
	if(curMil - lastMessage > maxUpdateSpeed){
		printDataMessage();
		lastMessage = curMil;
	}
}

/**
 * send all current wether data over the serial port.
 */
void printDataMessage(){
	unsigned char checkAdd = 0;
	unsigned char checkMul = 0;
	unsigned char checkHead = 0;

	unsigned char buts = 0;
	for(int i = 0; i < 4; i++){
		buts = buts << 1;
		buts += buttons[i]->getState();
	}

	unsigned char out[] = {10, checkAdd, checkMul, msgId,  // <- head
			1,
			(unsigned char)potMeter->getValuePersentage(),
			(unsigned char)ldr->getLuxValue(),
			(unsigned char)(tempMeter->getCurDegrees()+100),
			buts,
			checkHead};

	for(int i = 4; i < out[0]-1; i++){
		checkAdd += out[i];
		checkMul *= out[i];
	}

	if(checkAdd == 255)
		checkAdd = 0;
	if(checkMul == 255)
		checkMul = 0;

	out[1] = checkAdd;
	out[2] = checkMul;

	for(int i = 0; i < 4; i++){
		checkHead += out[i];
	}
	if(checkHead == 255)
		checkHead = 0;

	out[out[0]-1] = checkHead;

	for(int i = 0; i < out[0]; i++){
		Serial.write(out[i]);
	}
	Serial.write(255);

	msgId++;
	if(msgId == 255)
		msgId = 0;
}
