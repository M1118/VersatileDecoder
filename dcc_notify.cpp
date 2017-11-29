#include <Arduino.h>
#include <NmraDcc.h>
#include "setpiece.h"
#include "pins.h"

extern DCC_PINS dcc;

extern void setSpeed(int percentage, boolean forwards);
extern void setFuncState(int func,  boolean state, boolean sp);

const unsigned long AckLength       = 6;  // Nominal ack lenth in mS
const unsigned int  AckAdditionaluS = 100; // Addition time in uS to strech ack pulse

// This function is called by the NmraDcc library
// when a DCC ACK needs to be sent
// Calling this function should cause an increased 60ma current drain
// on the power supply for 6ms to ACK a CV Read
void notifyCVAck(void)
{
  digitalWrite(dcc.acknowledge, ACK_ON);
  delay(AckLength);
  delayMicroseconds(AckAdditionaluS);
  digitalWrite(dcc.acknowledge, ACK_OFF);
}

/*
 *  Periodically called with the speed, direction and number of speed steps
 *
 *  Work out the current speed percentage and direction and update each of the
 *  servos with this data
 */
void notifyDccSpeed( uint16_t Addr, DCC_ADDR_TYPE AddrType, uint8_t Speed, DCC_DIRECTION ForwardDir, DCC_SPEED_STEPS SpeedSteps )
{
  int percentage = ((Speed - 1) * 100) / SpeedSteps;
  if (setPiece.isActive())
  {
    return;
  }
  setSpeed(percentage, ForwardDir != 0);
}


/*
 *  Called regularly to report the state of the function keys
 */
void notifyDccFunc( uint16_t Addr, DCC_ADDR_TYPE AddrType, FN_GROUP FuncGrp, uint8_t FuncState)
{
  if (FuncGrp == FN_0_4)
  {
    /* Function group 1 */
    if (FuncState & 0x10)
      setFuncState(0, true, false);
    else
      setFuncState(0, false, false);
    if (FuncState & 0x01)
      setFuncState(1, true, false);
    else
      setFuncState(1, false, false);
    if (FuncState & 0x02)
      setFuncState(2, true, false);
    else
      setFuncState(2, false, false);
    if (FuncState & 0x04)
      setFuncState(3, true, false);
    else
      setFuncState(3, false, false);
    if (FuncState & 0x08)
      setFuncState(4, true, false);
    else
      setFuncState(4, false, false);
  }
  else if (FuncGrp == FN_5_8)
  {
    /* Function group 2 */
    if (FuncState & 0x01)
      setFuncState(5, true, false);
    else
      setFuncState(5, false, false);
    if (FuncState & 0x02)
      setFuncState(6, true, false);
    else
      setFuncState(6, false, false);
    if (FuncState & 0x04)
      setFuncState(7, true, false);
    else
      setFuncState(7, false, false);
    if (FuncState & 0x08)
      setFuncState(8, true, false);
    else
      setFuncState(8, false, false);

  }
  else if (FuncGrp == FN_9_12)
  {
    /* Function group 3 */
    if (FuncState & 0x01)
      setFuncState(9, true, false);
    else
      setFuncState(9, false, false);
    if (FuncState & 0x02)
      setFuncState(10, true, false);
    else
      setFuncState(10, false, false);
    if (FuncState & 0x04)
      setFuncState(11, true, false);
    else
      setFuncState(11, false, false);
    if (FuncState & 0x08)
      setFuncState(12, true, false);
    else
      setFuncState(12, false, false);
  }
  else if (FuncGrp == FN_13_20)
  {
    /* Function group 3 */
    if (FuncState & 0x01)
      setFuncState(13, true, false);
    else
      setFuncState(13, false, false);
    if (FuncState & 0x02)
      setFuncState(14, true, false);
    else
      setFuncState(14, false, false);
    if (FuncState & 0x04)
      setFuncState(15, true, false);
    else
      setFuncState(15, false, false);
    if (FuncState & 0x08)
      setFuncState(16, true, false);
    else
      setFuncState(16, false, false);
    if (FuncState & 0x10)
      setFuncState(17, true, false);
    else
      setFuncState(17, false, false);
    if (FuncState & 0x20)
      setFuncState(18, true, false);
    else
      setFuncState(18, false, false);
    if (FuncState & 0x40)
      setFuncState(19, true, false);
    else
      setFuncState(19, false, false);
    if (FuncState & 0x80)
      setFuncState(20, true, false);
    else
      setFuncState(20, false, false);
  }
  else if (FuncGrp == FN_21_28)
  {
    /* Function group 4 */
    if (FuncState & 0x01)
      setFuncState(21, true, false);
    else
      setFuncState(21, false, false);
    if (FuncState & 0x02)
      setFuncState(22, true, false);
    else
      setFuncState(22, false, false);
    if (FuncState & 0x04)
      setFuncState(23, true, false);
    else
      setFuncState(23, false, false);
    if (FuncState & 0x08)
      setFuncState(24, true, false);
    else
      setFuncState(24, false, false);
    if (FuncState & 0x10)
      setFuncState(25, true, false);
    else
      setFuncState(25, false, false);
    if (FuncState & 0x20)
      setFuncState(26, true, false);
    else
      setFuncState(26, false, false);
    if (FuncState & 0x40)
      setFuncState(27, true, false);
    else
      setFuncState(27, false, false);
    if (FuncState & 0x80)
      setFuncState(28, true, false);
    else
      setFuncState(28, false, false);
  }
}

extern void updateCV(uint16_t, uint8_t);
/*
 * Called to notify a CV value has been changed
 */
void notifyCVChange(uint16_t CV, uint8_t value)
{
  updateCV(CV, value);
}
