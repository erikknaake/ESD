unsigned long timerOvergangNaarDoorRijden = 0;
const int OVERGANG_NAAR_RIJ_DOOR_TIJD = 800;

unsigned long timerDoorRijden = 0;
const int TIJD_OP_DOOR_RIJDEN = 5000;

unsigned long timerOvergangNaarStoppen = 0;
const int TUSSEN_TIJD_NAAR_STOPPEN = 1000;
const byte AANTAL_STOPLICHTEN = 2, AANTAL_LICHTEN_PER_STOPLICHT = 3, AANTAL_STOPLICHT_FASEN = 4, AANTAL_SOORTEN_STOPLICHTEN = 2, NEDERLANDS_STOPLICHT = 0, DUITS_STOPLICHT = 1;
byte stoplichtSoort;

const byte stoplichten[AANTAL_STOPLICHTEN][AANTAL_LICHTEN_PER_STOPLICHT] =
{
  {6, 5, 4},
  {9, 8, 7}
};

const byte STOPLICHT_STOPPEN = 0, STOPLICHT_OVERGANG_NAAR_DOORRIJDEN = 1, STOPLICHT_DOORRIJDEN = 2, STOPLICHT_OVERGANG_NAAR_STOPPEN = 3;
const boolean stoplichtPatronen[AANTAL_SOORTEN_STOPLICHTEN][AANTAL_STOPLICHT_FASEN][AANTAL_LICHTEN_PER_STOPLICHT] = 
{
  {
    //Nederlands licht
    {true, false, false}, //Rood
    {false, false, false}, //overgang naar groen
    {false, false, true}, //groen
    {false, true, false} //overgang naar rood
  },
  {
    //Duitse licht
    {true, false, false}, //Rood
    {true, true, false}, //overgang naar groen
    {false, false, true}, //groen
    {false, true, false} //overgang naar rood
  }
};

byte stoplichtDoor;

bool knipperOn;
unsigned long previousMillisKnipper = 0;
const int KNIPPER_INTERVAL = 300;
const int KNIPPER_TIJD_OOSTENRIJK = 2000;

//Getters
unsigned long getTimerDoorRijden() { return timerDoorRijden; }
int getTIJD_OP_DOOR_RIJDEN() { return TIJD_OP_DOOR_RIJDEN; }
unsigned long getTimerOvergangNaarStoppen() { return timerOvergangNaarStoppen; }
int getTUSSEN_TIJD_NAAR_STOPPEN() { return TUSSEN_TIJD_NAAR_STOPPEN; }
unsigned long getTimerOvergangNaarDoorRijden() { return timerOvergangNaarDoorRijden; }
byte getAANTAL_STOPLICHTEN() { return AANTAL_STOPLICHTEN; }
int getKNIPPER_TIJD_OOSTERIJK() { return KNIPPER_TIJD_OOSTENRIJK; }

void knipperSetup()
{
  previousMillisKnipper = millis();
  knipperOn = true;
}
//Het knipperen van een lamp.
void knipper(byte stoplichtNummer, byte lampNummer)
{
  unsigned long currentTime = millis();
  if(currentTime - previousMillisKnipper >= KNIPPER_INTERVAL)
  {
    knipperOn = !knipperOn;
    previousMillisKnipper = currentTime;
  }
  stoplichtZetLamp(stoplichtNummer, lampNummer, knipperOn);
}

void stoplichtSetup()
{
  stoplichtSoort = NEDERLANDS_STOPLICHT;
  stoplichtDoor = 255;
  
  for(byte i = 0; i < AANTAL_STOPLICHTEN; i++)
    for(byte j = 0; j < AANTAL_LICHTEN_PER_STOPLICHT; j++)
      pinMode(stoplichten[i][j], OUTPUT);

  for(byte i = 0; i < AANTAL_STOPLICHTEN; i++)
  {
    stoplichtDisplayPatroon(i, STOPLICHT_STOPPEN);
  }
}
//Laat de betreffende patroon regel van het huidige patroon zien op het geselecteerde stoplicht.
void stoplichtDisplayPatroon(byte stoplichtNummer, byte patroonRegelNummer)
{
  for(byte i = 0; i < AANTAL_LICHTEN_PER_STOPLICHT; i++)
  {
    stoplichtZetLamp(stoplichtNummer, i, stoplichtPatronen[stoplichtSoort][patroonRegelNummer][i]);
  }
}

//Zet de geselecteerde lamp van het geselecteerde stoplicht naar de geselecteerde status.
void stoplichtZetLamp(byte stoplichtNummer, byte lampNummer, bool state)
{
  digitalWrite(stoplichten[stoplichtNummer][lampNummer], state);
}

void stoplichtUit(byte stoplichtNummer)
{
  for(byte i = 0; i < AANTAL_LICHTEN_PER_STOPLICHT; i++)
    digitalWrite(stoplichten[stoplichtNummer][i], LOW);
}

//Controleerd of het stoplicht een lamp kan aanzetten op het geselecteerde faseNummer van het huidige patroon
bool heeftStoplichtFase(byte faseNummer)
{
  for(byte i = 0; i < AANTAL_LICHTEN_PER_STOPLICHT; i++)
    if(stoplichtPatronen[stoplichtSoort][faseNummer][i])
      return true;
  return false;
}
void stoplichtOvergangNaarDoorRijden_Entry()
{
  //Serial.println(getLaatstOntvangenKarakter());
  switch(getLaatstOntvangenKarakter())
  {
    case 'N':
      stoplichtSoort = NEDERLANDS_STOPLICHT;
      break;
    case 'D':
      stoplichtSoort = DUITS_STOPLICHT;
      break;
    case 'O':
      stoplichtSoort = DUITS_STOPLICHT; //Duits stoplicht en Oostenrijksstoplicht hebben hetzelfde patroon.
      break;
  }
  stoplichtDoor = buttonPressed(AUTO_NADERT_KNOP1) ? 0 : 1;  
  stoplichtDisplayPatroon(stoplichtDoor, STOPLICHT_OVERGANG_NAAR_DOORRIJDEN);
  if(heeftStoplichtFase(1))
    timerOvergangNaarDoorRijden = millis() + OVERGANG_NAAR_RIJ_DOOR_TIJD;
  else 
    timerOvergangNaarDoorRijden = millis();
}
void stoplichtOvergangNaarDoorRijden_Do() {}
void stoplichtOvergangNaarDoorRijden_Exit() 
{
  stoplichtUit(stoplichtDoor);
}

void stoplichtDoorRijden_Entry() 
{
  stoplichtDisplayPatroon(stoplichtDoor, STOPLICHT_DOORRIJDEN);
  timerDoorRijden = millis();
}
void stoplichtDoorRijden_Do() {}
void stoplichtDoorRijden_Exit() 
{
  stoplichtUit(stoplichtDoor);  
}

void stoplichtDoorRijdenKnipper_Entry() 
{
  timerDoorRijden = millis();
  knipperSetup();  
}
void stoplichtDoorRijdenKnipper_Do() 
{
  knipper(stoplichtDoor, STOPLICHT_DOORRIJDEN);
}
void stoplichtDoorRijdenKnipper_Exit() 
{
  stoplichtZetLamp(stoplichtDoor, STOPLICHT_DOORRIJDEN, false);
}

void stoplichtOvergangNaarStoppen_Entry() 
{
  timerOvergangNaarStoppen = millis();
  stoplichtDisplayPatroon(stoplichtDoor, STOPLICHT_OVERGANG_NAAR_STOPPEN);
}
void stoplichtOvergangNaarStoppen_Do() {}
void stoplichtOvergangNaarStoppen_Exit() 
{
  stoplichtDisplayPatroon(stoplichtDoor, STOPLICHT_STOPPEN);
  buttonAfgehandeld(stoplichtDoor);
}
