#include <Servo.h>

Servo slagboom;
const byte MAX_HOEK = 90, MIN_HOEK = 0, GRADEN_PER_SECONDE = 15;
unsigned long slagboomPreviousMillis = 0;
unsigned long timerAftellenDisplay = 0;
unsigned long getTimerAftellenDisplay() { return timerAftellenDisplay; }

void slagboomSetup()
{
  slagboom.attach(3);
  slagboom.write(MIN_HOEK);
}

bool isSlagboomOpen()
{
  return slagboom.read() >= MAX_HOEK;
}

bool isSlagboomDicht()
{
  return slagboom.read() <= MIN_HOEK;
}
//Draait de slagboom een stukje omhoog als de timer dit toestaat
void slagboomOmhoog()
{
  unsigned long currentMillis = millis();
  if(currentMillis - slagboomPreviousMillis >= 1000.f / GRADEN_PER_SECONDE && !isSlagboomOpen())
  {
    slagboom.write(slagboom.read() + 1);
    slagboomPreviousMillis = currentMillis;
  }
}
//Draait de slagboom een stukje omlaag als de timer dit toestaat
void slagboomOmlaag()
{
  unsigned long currentMillis = millis();
  if(currentMillis - slagboomPreviousMillis >= 1000.f / GRADEN_PER_SECONDE && !isSlagboomDicht())
  {
    slagboom.write(slagboom.read() - 1);
    slagboomPreviousMillis = currentMillis;
  }
}

void openenVoetgangersOvergang_Entry() 
{
}
void openenVoetgangersOvergang_Do() 
{
  slagboomOmhoog();
}
void openenVoetgangersOvergang_Exit() 
{
}

void voetgangersOvergangOpen_Entry() 
{
  
  buzzerSetFrequentie(HOGE_FREQUENTIE);
  display_ToonOversteken();
  timerBuzzerVoetgangersOvergangOpen = millis();
}
void voetgangersOvergangOpen_Do() 
{
  
}
void voetgangersOvergangOpen_Exit() {}

void aftellenVoetgangersOvergang_Entry() 
{
  timerAftellenDisplay = millis() + 7000;
  buzzerTikken_statemachineSetup();
}
void aftellenVoetgangersOvergang_Do()
{
  buzzerTikken_runStatemachine();
  toonSymbool((timerAftellenDisplay - millis()) / 1000);  
}
void aftellenVoetgangersOvergang_Exit() {}

void afsluitenVoetgangersOvergang_Entry() 
{
  display_ToonWachten();
}
void afsluitenVoetgangersOvergang_Do() 
{
  slagboomOmlaag();
  buzzerSetFrequentie(LAGE_FREQUENTIE);
}
void afsluitenVoetgangersOvergang_Exit() { buttonAfgehandeld(VOETGANGERS_KNOP);}
