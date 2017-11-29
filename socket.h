#include <DCCStepper.h>
#include <DCCServo.h>
#include "servo_state.h"

typedef enum {
	FOUR_SERVOS = 0,
	STEPPER,
	STEPPER_2PHASE,
	TWO_MOTORS,
	TWO_SERVOS_MOTOR
} SOCKET_CONFIG;

class Socket {
	public:
		Socket(OP_PINS *, int);
		void configure(SOCKET_CONFIG);
		void setFuncState(int, boolean, boolean);
		void setSpeed(int, boolean);
		void updateCV(uint16_t, uint8_t);
		void loop();
	private:
		void		initServos(int);
		int		socketNum;
		SOCKET_CONFIG	config;
		OP_PINS		*pins;
		DCCServo	*servo;
		DCCStepper	*stepper;
		ServoState	*servoStates;
};
