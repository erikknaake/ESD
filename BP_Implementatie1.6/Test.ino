bool aanHetTesten = false;
bool testCompleted()
{
  return !aanHetTesten;
}

unsigned long previousMillisTest = 0;
const int TIJD_LED_AAN_TIJDENS_TEST = 1000;
const int TIJD_TUSSEN_LED_TEST = 500;
const int TIJD_BUZZER_TEST = 2000;

const byte MAXIMALE_TEST_UITSLAG = 15;

byte stoplichtStandDieGetestWordt = 0;
bool faseKlaar = false;
bool slagboomOmhoogGeweest = false;

enum TestFase
{
  LAMP1,
  LAMP2,
  BUZZER_FASE,
  SLAGBOOM
}

testFase = LAMP1;

void voerTestUit()
{
  switch(testFase)
  {
  case LAMP1: 
    testLicht(0);
    if(faseKlaar)
    {
      faseKlaar = false;
      testFase = LAMP2;
      stoplichtZetLamp(0, 0, true);
    }
    break;
  case LAMP2:
    testLicht(1);
    if(faseKlaar)
    {
      faseKlaar = false;
      testFase = BUZZER_FASE;
      stoplichtZetLamp(1, 0, true);
    }
    break;
  case BUZZER_FASE:
    testBuzzer();
    if(faseKlaar)
    {
      faseKlaar = false;
      testFase = SLAGBOOM;
    }
    break;
  case SLAGBOOM:
    testSlagboom();
    if(faseKlaar)
    {
      faseKlaar = false;
      aanHetTesten = false;
      testFase = LAMP1;
    }
  }

}

//Kleine uitslag van de slagboom en weer terug
void testSlagboom()
{
  if(!slagboomOmhoogGeweest)
    slagboomOmhoog();
  else
    slagboomOmlaag();

  if(slagboom.read() >= MAXIMALE_TEST_UITSLAG)
    slagboomOmhoogGeweest = true;
  if(slagboomOmhoogGeweest && slagboom.read() <= MIN_HOEK)
  {
    faseKlaar = true;
    slagboomOmhoogGeweest = false;
  }
}

//Elk licht 1 seconde aan en steeds 0,5 seconde tussen elk licht.
void testLicht(byte stoplichtNummer)
{
  //Serial.println(stoplichtStandDieGetestWordt);
  aanHetTesten = true;
  unsigned long currentMillis = millis();

  if(currentMillis - previousMillisTest < TIJD_LED_AAN_TIJDENS_TEST)
    stoplichtZetLamp(stoplichtNummer, stoplichtStandDieGetestWordt, true);

  if(currentMillis - previousMillisTest >= TIJD_LED_AAN_TIJDENS_TEST)
  {
    //Tussen de lampen in
    stoplichtZetLamp(stoplichtNummer, stoplichtStandDieGetestWordt, false);
    if(currentMillis - previousMillisTest >= TIJD_LED_AAN_TIJDENS_TEST + TIJD_TUSSEN_LED_TEST)
    {
      //Naar het volgende lampje
      stoplichtStandDieGetestWordt++;
      previousMillisTest = currentMillis;
      if(stoplichtStandDieGetestWordt >= AANTAL_LICHTEN_PER_STOPLICHT)
      {
        //Dit stoplicht is getest
        faseKlaar = true;
        stoplichtStandDieGetestWordt = 0;
      }
    }
  }
}

//Laat de buzzer 2 seconden geluid maken.
void testBuzzer()
{
  buzzerSetFrequentie(HOGE_FREQUENTIE);
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillisTest >= TIJD_BUZZER_TEST)
  {
    faseKlaar = true;
    buzzerSetFrequentie(UIT_FREQUENTIE);
    previousMillisTest = currentMillis;
  }
}

void testSetup()
{
  testFase = LAMP1;
  stoplichtStandDieGetestWordt = 0;
  faseKlaar = false;
  slagboomOmhoogGeweest = false;
  aanHetTesten = true;
  buzzerSetFrequentie(UIT_FREQUENTIE);
  previousMillisTest = millis();
}


void test_Entry() { testSetup();}
void test_Do() { voerTestUit();}
void test_Exit()
{
  buttonAfgehandeld(TEST_KNOP);
}

