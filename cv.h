#ifndef _CV_H
#define _CV_H
#include <NmraDcc.h>


#define DCC_VERSION_ID    5

// CV Storage structure
struct CVPair
{
  uint16_t  CV;
  uint8_t   Value;
};

#define CV_OP1		30
#define CV_OP2		31
#define CV_SERVO(x)	(32 + ((x) * 6))	// Servo from 32 to 74 
#define SERVO_LIMIT0	0
#define SERVO_LIMIT1	1
#define SERVO_TRAVEL	2
#define SERVO_FLAGS	3
#define SERVO_FUNC	4
#define SERVO_BOUNCE 	5
#define CV_S0_STEPS       80
#define CV_S0_RATIO       81
#define CV_S0_MAXRPM      82
#define CV_S0_STEPFUNC    83
#define CV_S0_STEPMODE    84
#define CV_S0_MAXLSB      85
#define CV_S0_MAXMSB      86
#define CV_S0_STEPDELAY   87
#define CV_S0_CURLSB      88
#define CV_S0_CURMSB      89
#define CV_S1_STEPS       90
#define CV_S1_RATIO       91
#define CV_S1_MAXRPM      92
#define CV_S1_STEPFUNC    93
#define CV_S1_STEPMODE    94
#define CV_S1_MAXLSB      95
#define CV_S1_MAXMSB      96
#define CV_S1_STEPDELAY   97
#define CV_S1_CURLSB      98
#define CV_S1_CURMSB      99
#endif
