package com.sinius15.test;

import processing.core.PApplet;

public class ArduinoReceiver extends PApplet implements SerialEvent{

	private static final long serialVersionUID = -6035895160696015165L;
	
	SerialReceiver receiver;
	
	int potMeterVal = -1;
	int lightMeterVal = -1;
	int tempMeterVal = -1;
	boolean[] buttonStates = new boolean[4];

	@Override
	public void setup() {
		size(400, 200);
		receiver = new SerialReceiver(this);
		receiver.addSerialEventListeners(this);
	}

	@Override
	public void draw() {
		background(0);
		textSize(20);
		text("potMeterVal: " + potMeterVal, 10, 50);
		text("lightSensor: " + lightMeterVal, 10, 70);
		text("tempMeterVal: " + tempMeterVal, 10, 90);
		for(int i = 0; i < buttonStates.length; i++){
			text("button " + i + ": " + (buttonStates[i] ? "pressed" : "not pressed"), 10, 110+i*20);
		}
	}

	@Override
	public void serialMessageReceived(int[] data) {
		potMeterVal = data[1];
		lightMeterVal = data[2];
		tempMeterVal = data[3]-100;
		int rawBut = data[4];
		for(int i = 0; i < buttonStates.length; i++){
			buttonStates[i] = rawBut % 2 != 0;
			rawBut = rawBut >> 1;
		}
	}	

}
