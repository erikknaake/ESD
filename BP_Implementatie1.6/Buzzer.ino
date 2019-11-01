const byte BUZZER = 13;
const int LAGE_FREQUENTIE = 2;
const int HOGE_FREQUENTIE = 60;
const int DRIE_TIKKEN_INTERVAL = 500;
const int DRIE_TIKKEN_FREQUENTIE = 3 / (DRIE_TIKKEN_INTERVAL / 2000.f);
const byte UIT_FREQUENTIE = 0;

unsigned long previousMillisBuzzer = 0;
int buzzerFrequentie = 1; 


unsigned long timerBuzzerVoetgangersOvergangOpen = 0;
unsigned long getTimerBuzzerVoetgangersOvergangOpen() { return timerBuzzerVoetgangersOvergangOpen; }
const int TIJD_BUZZER_AAN_OPENEN_VOETGANGERS = 2000;

enum tikkenStates 
{
  drieTikken,
  pauze
};

tikkenStates buzzerState;
unsigned long buzzerTikkenTimer = 0;
void buzzerSetup()
{
  pinMode(BUZZER, OUTPUT);
}

//Maak geluid op frequentie die vastgelegd is in buzzerFrequentie.
void buzzerMaakGeluid()
{
  unsigned long currentMillis = millis();
  if(buzzerFrequentie != UIT_FREQUENTIE)
  {
    if(currentMillis - previousMillisBuzzer >= 2000.f / buzzerFrequentie)
    {
      digitalWrite(BUZZER, HIGH);
      previousMillisBuzzer = currentMillis;
    }
    else
      digitalWrite(BUZZER, LOW);
  }
}

void buzzerSetFrequentie(byte frequentie)
{
  buzzerFrequentie = frequentie;
}

void buzzerTikken_statemachineSetup()
{
  buzzerState = drieTikken;
  buzzerDrieTikken_Entry();
}

//Om per drie tikken een pauze te kunnen nemen.
void buzzerTikken_runStatemachine()
{
  unsigned long currentTime = millis();
  switch(buzzerState)
  {
    case drieTikken:
      if(currentTime - buzzerTikkenTimer >= DRIE_TIKKEN_INTERVAL)
      {
        buzzerState = pauze;
        buzzerDrieTikkenPauze_Entry();
      }
      break;
    case pauze:
      if(currentTime - buzzerTikkenTimer >= DRIE_TIKKEN_INTERVAL)
      {
        buzzerState = drieTikken;
        buzzerDrieTikken_Entry();
      }
      break;
  }
}

void buzzerDrieTikken_Entry()
{
  buzzerTikkenTimer = millis();
  buzzerSetFrequentie(DRIE_TIKKEN_FREQUENTIE);
}

void buzzerDrieTikkenPauze_Entry()
{
  buzzerTikkenTimer = millis();
  buzzerSetFrequentie(UIT_FREQUENTIE);
}

