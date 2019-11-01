const byte SYMBOLEN[]
{
  B11111100, //0
  B01100000, //1
  B11011010, //2
  B11101010, //3
  B01100110, //4
  B10101110, //5
  B10111110, //6
  B10001010, //WACHTEN
  B10001000, //LET EXTRA OP
  B00000010 //OVERSTEKEN
};

void toonSymbool(byte symboolNummer) 
{ 
  writeWord(SYMBOLEN[symboolNummer]); 
}

void display_ToonWachten()
{
  toonSymbool(7);
}

void display_ToonLetExtraOp()
{
  toonSymbool(8);
}

void display_ToonOversteken()
{
  toonSymbool(9);
}

