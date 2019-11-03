enum ButtonStates
{
  RELEASED,
  CHECK_PRESSED,
  PRESSED,
  CHECK_RELEASED
};
//Constante voor knoppen
const byte AANTAL_BUTTONS = 4;
/*Auto 1, auto 2, voetganger, test*/
const byte BUTTONS[AANTAL_BUTTONS] = {A0, A1, A2, A3};
const byte AUTO_NADERT_KNOP1 = 0, AUTO_NADERT_KNOP2 = 1, VOETGANGERS_KNOP = 2, TEST_KNOP = 3;


//Array om af te lezen of een knop is ingedrukt
ButtonStates buttonsDown[AANTAL_BUTTONS] = { RELEASED, RELEASED, RELEASED, RELEASED };

//Om bouncing te voorkomen
unsigned long previousMillisButtons[AANTAL_BUTTONS] = {0, 0, 0, 0};
const byte DEBOUNCE_TIJD = 10;

//Of de button ingedrukt is, nog niet verwerkt is en niet gebounced is.
bool buttonPressed(byte buttonNumber)
{
  return buttonsDown[buttonNumber] == PRESSED;
}

void buttonSetup()
{
  for(byte i = 0; i < AANTAL_BUTTONS; i++)
  {
    pinMode(BUTTONS[i], INPUT);
    buttonsDown[i] = RELEASED;
  }
}

//Of de button is ingedrukt, niet bounce safe
bool buttonDown(byte buttonNumber)
{
  return analogRead(BUTTONS[buttonNumber]) > 200;
}

//Update de knoppen
void checkButtons()
{
  for(byte i = 0; i < AANTAL_BUTTONS; i++)
  {
    unsigned long currentMillis = millis();
    switch(buttonsDown[i])
    {
      case RELEASED:
        if(buttonDown(i))
        {
          buttonsDown[i] = CHECK_PRESSED;
          previousMillisButtons[i] = currentMillis; 
        }
        break;

      case CHECK_PRESSED:
        if(!buttonDown(i))
        {
          buttonsDown[i] = RELEASED;
        }
        else if(buttonDown(i) && currentMillis - previousMillisButtons[i] >= DEBOUNCE_TIJD)
        {
          buttonsDown[i] = PRESSED; 
        }
        break;

      case PRESSED:
        break;
      case CHECK_RELEASED:
        if(!buttonDown(i) && currentMillis - previousMillisButtons[i] >= DEBOUNCE_TIJD)
        {
          buttonsDown[i] = RELEASED;
        }
        break;
    }
  }
}

void buttonAfgehandeld(byte buttonNummer)
{
  buttonsDown[buttonNummer] = CHECK_RELEASED;
  previousMillisButtons[buttonNummer] = millis();
}

