/*
 * ControllerHeader.h
 *
 *  Created on: 1 dec. 2014
 *      Author: Sijmen
 */

#ifndef LIB_CONTROLLERS_H_
#define LIB_CONTROLLERS_H_

class PotentioMeter {
	private:
		unsigned int pin;
		void ( *callback )( void );
		int lastVal;
	public:
		PotentioMeter(unsigned int);
		void setValueChangeCallback(void( * )( void ));
		void update();
		int getValuePersentage();
		int getRawValue();
};

class LightDependentResistor {
	private:
		static const int max = 1020;
		static const int min = 1;
		int lastVal;
		unsigned int pin;
		void ( *callback )( void );
	public:
		LightDependentResistor(unsigned int);
		void setValueChangeCallback(void( * )( void ));
		void update();
		int getLuxValue();
		int getRawValue();
};

class Thermometer{
	private:
		unsigned int pin;
		int lastVal;
		static const int deg0 = 560;
		static const int deg22 = 369;
		void ( *callback )( void );
	public:
		Thermometer(unsigned int);
		void setValueChangeCallback(void( * )( void ));
		void update();
		int getCurDegrees();
		int getRawValue();
};

class Button{
	private:
		unsigned int pin;  //digital
		bool state;
		void ( *callback )( void );
	public:
		Button(int);
		void setCallback(void ( * )( void ));
		void update();
		bool getState();

};
#endif
