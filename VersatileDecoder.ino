#include "pins.h"
#include "socket.h"
#include "cv.h"
#include "cvdefaults.h"
#include "setpiece.h"
#include <NmraDcc.h>


OP_PINS socket1 = {
  { A4, A5 },   // Enable on pin A4 and A5
  5,    // Servo Control on D5
  { A3, A0, A1, A2 }  // 4 H Bridge control pins
},
socket2 = {
  { 12, 9 },   // Enable on pin D12 and D9
  4,    // Servo Control on D4
  { 11, 13, 8, 10 } // 4 H bridge outputs
};

DCC_PINS dcc = { 1, 3, 2 };

NmraDcc Dcc;

#define FACTORY_RESET_AT_STARTUP  0

#if FACTORY_RESET_AT_STARTUP
static uint8_t FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs) / sizeof(CVPair);
#else
static uint8_t FactoryDefaultCVIndex = 0;
#endif

Socket *sockets[2];

SetPiece setPiece;

/*
   Set the current speed as a percentage
*/
void setSpeed(int percentage, boolean forward)
{
  sockets[0]->setSpeed(percentage, forward);
  sockets[1]->setSpeed(percentage, forward);
}

/*
   Called for the state of each of the functions, use the
   the current function mapping to set of the state for each
   of the devices we control.
*/
void setFuncState(int function, boolean state, boolean sp)
{
  sockets[0]->setFuncState(function, state, sp);
  sockets[1]->setFuncState(function, state, sp);
}

void updateCV(uint16_t CV, uint8_t value)
{
  sockets[0]->updateCV(CV, value);
  sockets[1]->updateCV(CV, value);
}

void setup() {
  Serial.begin(9600);
  // Configure the DCC CV Programing ACK pin for an output
  pinMode(dcc.acknowledge, OUTPUT);
  digitalWrite(dcc.acknowledge, ACK_OFF);
  // Setup which External Interupt, the Pin it's associated with that we're using and enable the Pull-Up
  Dcc.pin(dcc.interupt, dcc.signal, 1);

  // If the saved CV value for the decoder does not match this version of
  // the code force a factry reset of the CV values
  if (Dcc.getCV(CV_VERSION_ID) != DCC_VERSION_ID)
  {
    FactoryDefaultCVIndex = sizeof(FactoryDefaultCVs) / sizeof(CVPair);
  }


  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init(MAN_ID_DIY, DCC_VERSION_ID, FLAGS_MY_ADDRESS_ONLY | FLAGS_OUTPUT_ADDRESS_MODE, 0);

  sockets[0] = new Socket(&socket1, 0);
  sockets[1] = new Socket(&socket2, 1);

  switch (Dcc.getCV(CV_OP1))
  {
    case 0:
      sockets[0]->configure(FOUR_SERVOS);
      break;
    case 1:
      sockets[0]->configure(STEPPER);
      break;
    case 2:
      sockets[0]->configure(STEPPER_2PHASE);
      break;
    case 3:
      sockets[0]->configure(TWO_MOTORS);
      break;
    case 4:
      sockets[0]->configure(TWO_SERVOS_MOTOR);
      break;
  }
  switch (Dcc.getCV(CV_OP2))
  {
    case 0:
      sockets[1]->configure(FOUR_SERVOS);
      break;
    case 1:
      sockets[1]->configure(STEPPER);
      break;
    case 2:
      sockets[1]->configure(STEPPER_2PHASE);
      break;
    case 3:
      sockets[1]->configure(TWO_MOTORS);
      break;
    case 4:
      sockets[1]->configure(TWO_SERVOS_MOTOR);
      break;
  }
}

void loop() {
  // Execute the DCC process frequently in order to ensure
  // the DCC signal processing occurs
  Dcc.process();
  if (FactoryDefaultCVIndex && Dcc.isSetCVReady())
  {
    FactoryDefaultCVIndex--; // Decrement first as initially it is the size of the array
    Dcc.setCV( FactoryDefaultCVs[FactoryDefaultCVIndex].CV,
               FactoryDefaultCVs[FactoryDefaultCVIndex].Value);
  }
  sockets[0]->loop();
  sockets[1]->loop();
}
