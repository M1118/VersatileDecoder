#include <Arduino.h>
#include "pins.h"
#include "socket.h"
#include "setpiece.h"
#include "servo_state.h"
#include <DCCServo.h>
#include <NmraDcc.h>
#include "cv.h"

extern NmraDcc Dcc;

/**
 * Construct a socket object and disable everything by default
 */
Socket::Socket(OP_PINS *pins, int socketNum)
{
	this->pins = pins;
	this->socketNum = socketNum;
	pinMode(pins->enable[0], OUTPUT);
	digitalWrite(pins->enable[0], LOW);
	pinMode(pins->enable[1], OUTPUT);
	digitalWrite(pins->enable[0], LOW);
	for (int i = 0; i < 4; i++)
	{
		pinMode(pins->general[i], OUTPUT);
		digitalWrite(pins->general[i], HIGH);
	}
	servo = NULL;
	stepper = NULL;
}

/**
 * Configure the socket
 */
void Socket::configure(SOCKET_CONFIG config)
{
	switch (config)
	{
	case FOUR_SERVOS:
		this->servo = new DCCServo(pins->servo, 0, 180, 10, SERVO_INITMID);
		digitalWrite(pins->enable[0], HIGH);
		digitalWrite(pins->enable[1], HIGH);
		servo->setActive(true);
		this->servoStates = new ServoState[4];
		initServos(4);
		break;
	case STEPPER:
		{
		unsigned int maxSteps = ((unsigned int)Dcc.getCV(CV_S0_MAXMSB) * 256) + Dcc.getCV(CV_S0_MAXLSB);
  		stepper = new DCCStepper(Dcc.getCV(CV_S0_STEPMODE)|STEPPER_INVERTED, maxSteps,
                             Dcc.getCV(CV_S0_RATIO) * Dcc.getCV(CV_S0_STEPS),
                             Dcc.getCV(CV_S0_MAXRPM),
		pins->general[0], pins->general[1], pins->general[2], pins->general[3]);
		digitalWrite(pins->enable[0], HIGH);
		digitalWrite(pins->enable[1], HIGH);
		break;
		}
	case STEPPER_2PHASE:
		break;
	case TWO_MOTORS:
		break;
	case TWO_SERVOS_MOTOR:
		break;
	}
}

/**
 * Set the function state
 */
void Socket::setFuncState(int fn, boolean state, boolean setPieceFn)
{
	if (setPiece.isActive() && setPieceFn == false)
	{
		return;
	}
	if (servo)
	{
		int pinNo = fn - (socketNum * 4);
		switch (pinNo)
		{
		case 0:
		case 1:
		case 2:
		case 3:
			if (state)
			{
				if (servoStates[pinNo].active)
				{
					return;
				}
Serial.print("Servo ");
Serial.print(pinNo);
Serial.print(" start ");
Serial.print(servoStates[pinNo].limit1);
Serial.print(" end ");
Serial.print(servoStates[pinNo].limit2);
Serial.print(" time ");
Serial.println(servoStates[pinNo].travel);
				servo->setAngle(servoStates[pinNo].angle);
				servo->setStart(servoStates[pinNo].limit1);
				servo->setEnd(servoStates[pinNo].limit2);
				servo->setTravelTime(servoStates[pinNo].travel);
				digitalWrite(pins->general[pinNo], LOW);
				servoStates[pinNo].active = true;
			}
			else
			{
				digitalWrite(pins->general[pinNo], HIGH);
				servoStates[pinNo].angle = servo->getAngle();
				servoStates[pinNo].active = false;
			}
			break;
		}
	}
	if (stepper)
	{
		switch (fn)
		{
		case 4:
			stepper->setActive(state);
		}
	}
}

void Socket::setSpeed(int speed, boolean forwards)
{
	if (servo)
	{
		servo->setSpeed(speed, forwards);
	}
	if (stepper)
	{
		stepper->setSpeed(speed, forwards);
	}
}

void Socket::initServos(int nServos)
{

	for (int i = 0; i < nServos; i++)
	{
		int cvbase = CV_SERVO(i);
		servoStates[i].limit1 = Dcc.getCV(cvbase + SERVO_LIMIT0);
		servoStates[i].limit2 = Dcc.getCV(cvbase + SERVO_LIMIT1);
		servoStates[i].travel = Dcc.getCV(cvbase + SERVO_TRAVEL);
Serial.print("Init Servo ");
Serial.print(i);
Serial.print(" start ");
Serial.print(servoStates[i].limit1);
Serial.print(" end ");
Serial.print(servoStates[i].limit2);
Serial.print(" time ");
Serial.println(servoStates[i].travel);
	}
}

void Socket::updateCV(uint16_t CV, uint8_t value)
{
	if (servo)
	{
		int servo_base = CV_SERVO(socketNum * 4);
		if (CV >= servo_base && CV <= servo_base + (4 * 6))
		{
			int servoNo = (CV - servo_base) / 6;
			int cvOffset = (CV - servo_base) % 6;
			switch (cvOffset)
			{
			case 0:
				servoStates[servoNo].limit1 = value;
				if (servoStates[servoNo].active)
				{
					servo->setStart(value);
				}
				break;
			case 1:
				servoStates[servoNo].limit2 = value;
				if (servoStates[servoNo].active)
				{
					servo->setEnd(value);
				}
				break;
			case 2:
				servoStates[servoNo].travel = value;
				if (servoStates[servoNo].active)
				{
					servo->setTravelTime(value);
				}
				break;
			case 3:
				servoStates[servoNo].flags = value;
				if (servoStates[servoNo].active)
				{
					servo->setFlags(value);
				}
				break;
			case 4:
				servoStates[servoNo].func = value;
				break;
			case 5:
				servoStates[servoNo].bounce = value;
				if (servoStates[servoNo].active)
				{
					servo->setBounceAngle(value);
				}
				break;
			}
		}
	}
	if (stepper)
	{
		switch (CV)
		{
		case CV_S0_STEPS:
		case CV_S0_RATIO:
			break;
		case CV_S0_MAXRPM:
			stepper->setRPM(value);
			break;
		case CV_S0_STEPMODE:
			stepper->setMode(value);
			break;
		case CV_S0_MAXLSB:
			stepper->setMaxStepsLSB(value);
			break;
		case CV_S0_MAXMSB:
			stepper->setMaxStepsMSB(value);
			break;
		case CV_S0_STEPDELAY:
			stepper->setReverseDelay(value);
			break;
		}
	}
}

void Socket::loop()
{
	if (servo)
	{
		servo->loop();
	}
	if (stepper)
	{
		stepper->loop();
	}
}
