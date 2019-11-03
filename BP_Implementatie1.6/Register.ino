const byte wordSize = 8;

const byte dataSerialPin = 12,
  shiftClockPulsePin = 10,
  latchPin = 11;
  
void registerSetup()
{
  pinMode(dataSerialPin, OUTPUT);
  pinMode(shiftClockPulsePin, OUTPUT);
  pinMode(latchPin, OUTPUT);    
}

//Schrijft de eerste bits tot aan de wordSize naar het register.
void writeWord(int data)
{
  for(byte i = 0; i <= wordSize; i++)
    feedBit(data >> i & 0x1);
  latch();
}

void latch()
{
  digitalWrite(latchPin, HIGH);
  digitalWrite(latchPin, LOW);
}

void feedBit(bool state)
{
  digitalWrite(shiftClockPulsePin, HIGH);
  digitalWrite(dataSerialPin, state);
  digitalWrite(shiftClockPulsePin, LOW);
}

