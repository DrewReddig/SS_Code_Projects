#include "arduino.h"

class Timeout
{
private:
  unsigned long timeoutTime;
  bool isElapsed;

public:
  Timeout()
    : timeoutTime(0), isElapsed(1)
  {
  }
  
  bool Tick(unsigned long currentTime)
  {
    if (isElapsed || currentTime < timeoutTime) return false;
    isElapsed = true;
    return true;
  }

  void Reset(unsigned long timeoutTime)
  {
    this->timeoutTime = timeoutTime;
    isElapsed = false;
  }
};

