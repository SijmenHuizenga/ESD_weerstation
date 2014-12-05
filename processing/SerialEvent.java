package com.sinius15.test;

public interface SerialEvent {
	
	/**
	 * This event if fired when a serial message is received.
	 * The message has already been checkd for errors with
	 * some checksums. The int[] contains the actual data.
	 * 
	 * @param data an unsigned array with data.
	 */
	public void serialMessageReceived(int[] data);
	
}
