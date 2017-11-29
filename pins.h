typedef struct pins {
	int	enable[2];
	int	servo;
	int	general[4];
} OP_PINS;

typedef struct {
	int	interupt;
	int	signal;
	int	acknowledge;
} DCC_PINS;


#if INVERTED_ACK
#define ACK_OFF   HIGH
#define ACK_ON    LOW
#else
#define ACK_OFF   LOW
#define ACK_ON    HIGH
#endif
