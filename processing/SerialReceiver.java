package com.sinius15.test;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collection;
import java.util.List;

import processing.core.PApplet;
import processing.serial.Serial;

public class SerialReceiver implements Runnable{

	private Serial myPort;
	
	private ArrayList<Integer> buffer = new ArrayList<>();
	private ArrayList<SerialEvent> listeners = new ArrayList<>();
	
	private int nextMsgId = Integer.MIN_VALUE;
	private boolean running = true;

	/**
	 * constructor
	 * @param parent the PApplet parent.
	 */
	public SerialReceiver(PApplet parent){
		myPort = new Serial(parent, Serial.list()[0], 9600);
		new Thread(this).start();
	}
	
	/**
     * As specified by {@link Collection#add}
     */
	public boolean addSerialEventListeners(SerialEvent e){
		return listeners.add(e);
	}
	
	/**
     * As specified by {@link Collection#remove}
     */
	public boolean removeSerialEventListeners(SerialEvent e){
		return listeners.remove(e);
	}
	
	/**
	 * stop this serialReceiver from receiving data.
	 */
	public void stop(){
		running =  false;
		myPort.stop();
	}
	
	@Override
	public void run() {
		while (running) {
			while (myPort.available() > 0) {
				int b = toUnsigned((byte)myPort.read());
				if (b == 255) {
					int[] inputArr = convertIntegers(buffer);
					if(doValidation(inputArr)){
						int[] data = Arrays.copyOfRange(inputArr, 4, inputArr.length);
						for(SerialEvent e : listeners){
							e.serialMessageReceived(data);
						}
					}
					buffer.clear();
				} else {
					buffer.add(b);
				}
			}
			try {
				Thread.sleep(1);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}
	
	/**
	 * Validate the incomming message by using checksums and suff.
	 * @param s non processed data received from the serial port.
	 * @return true if the message is valid, else false.
	 */
	private boolean doValidation(int[] s) {
		System.out.println(nextMsgId);
		if(s.length < 4){
			//only received header or less.
			System.err.println("Comminucation Error 0x00. received:" +s.length);
			return false;
		}
		if (s[0] != s.length) {
			// not everything is received.
			System.err.println("Comminucation Error 0x01. received: " + s[0] + " expected: " + s.length);
			return false;
		}

		byte checkAdd = 0;
		byte checkMul = 0;
		for (int i = 4; i < s[0] - 1; i++) {
			checkAdd += s[i];
			checkMul *= s[i];
		}
		if (toUnsigned(checkAdd) == 255)
			checkAdd = 0;
		if (toUnsigned(checkMul) == 255)
			checkMul = 0;

		if (toUnsigned(checkAdd) != s[1]) {
			// checksum adding failed
			System.err.println("Comminucation Error 0x02. received: " + s[1] + "  expected: " + toUnsigned(checkAdd));
			return false;
		}
		if (toUnsigned(checkMul) != s[2]) {
			// checksum multiply failed
			System.err.println("Comminucation Error 0x03. received: " + s[2] + "  expected: " + toUnsigned(checkMul));
			return false;
		}

		byte checkHead = 0;
		for (int i = 0; i < 4; i++) {
			checkHead += s[i];
		}
		if (toUnsigned(checkHead) == 255)
			checkHead = 0;
		if (toUnsigned(checkHead) != s[s[0] - 1]) {
			// haeder sum
			System.err.println("Comminucation Error 0x04. received: " + s[s[0]-1] + "  expected: " + toUnsigned(checkHead));
			return false;
		}
		if (nextMsgId == Integer.MIN_VALUE) {
			nextMsgId = s[3]+1;
		} else if (nextMsgId != s[3]) {
			System.err.println("Comminucation Error 0x05. received: " + s[3] + "  expected: " + nextMsgId);
			nextMsgId = s[3]+1;
			return false;
		} else {
			nextMsgId++;
			if(nextMsgId == 255)
				nextMsgId = 0;
		}
		return true;
	}
	
	/**
	 * convert a Integer arraylist to a primar int array
	 * @param integers the list to convert
	 * @return the converted list
	 */
	public static int[] convertIntegers(List<Integer> integers) {
		int[] ret = new int[integers.size()];
		for (int i = 0; i < ret.length; i++) {
			ret[i] = integers.get(i).intValue();
		}
		return ret;
	}
	
	/**
	 * convert a int to a unsgined int. This is somtimes neseserry
	 * when a value of a int 
	 * @param b the int to convert
	 * @return the converted unsigned int.
	 */
	public static int toUnsigned(int b) {
		return b & 0xFF;
	}
	
}
