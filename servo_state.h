#ifndef _SERVO_STATE_H
#define _SERVO_STATE_H
class ServoState {
	public:
		boolean active;
		int	limit1;
		int	limit2;
		unsigned long travel;
		boolean clockwise;
		int	angle;
		int	flags;
		int	func;
		int	bounce;
};
#endif
