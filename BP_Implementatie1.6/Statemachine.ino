enum States 
{
  rustStand,
  naarNachtstand,
  nachtStand,
  naarDagStand,
  dodeTijd,
  openenVoetgangersOvergang,
  voetgangersOvergangOpen,
  aftellenVoetgangersOvergang,
  afsluitenVoetgangersOvergang,
  stoplichtOvergangNaarDoorRijden,
  stoplichtDoorRijden,
  stoplichtDoorRijdenKnipper,
  stoplichtOvergangNaarStoppen,
  test
};
byte huidigeKnopOmTeChecken;
void veranderHuidigeKnopOmTeChecken()
{
  huidigeKnopOmTeChecken = (huidigeKnopOmTeChecken + 1) % AANTAL_BUTTONS;
}
//Hoofd state van het programma
States mainState = rustStand;

void stateMachineSetup()
{
  huidigeKnopOmTeChecken = AANTAL_BUTTONS - 1;
  mainState = rustStand;
  rustStand_Entry();
}
//Voert de hoofdstatemachine uit
void runStatemachine()
{
  unsigned long currentTime = millis();
  switch(mainState)
  {
    case rustStand:
    rustStand_Do();
      if(buttonPressed(AUTO_NADERT_KNOP1) && huidigeKnopOmTeChecken == 0)
      {
        rustStand_Exit();
        stoplichtOvergangNaarDoorRijden_Entry();
        mainState = stoplichtOvergangNaarDoorRijden;
      }
      else if(buttonPressed(AUTO_NADERT_KNOP2) && huidigeKnopOmTeChecken == 1)
      {
        rustStand_Exit();
        stoplichtOvergangNaarDoorRijden_Entry();
        mainState = stoplichtOvergangNaarDoorRijden;
      }
      else if(buttonPressed(VOETGANGERS_KNOP) && huidigeKnopOmTeChecken == 2)
      {
        rustStand_Exit();
        openenVoetgangersOvergang_Entry();
        mainState = openenVoetgangersOvergang;
      }
      else if(buttonPressed(TEST_KNOP) && huidigeKnopOmTeChecken == 3)
      {
        rustStand_Exit();
        test_Entry();
        mainState = test;
      }
      else if(!isDag())
      {
        rustStand_Exit();
        mainState = naarNachtstand;
        naarNachtstand_Entry();
      } 
      break;
    case naarNachtstand:
      if(isSlagboomOpen())
      {
        naarNachtstand_Exit();
        nachtStand_Entry();
        mainState = nachtStand;
      }
      else
        naarNachtstand_Do();
      break;
    case nachtStand: 
      if(isDag())
      {
        nachtStand_Exit();
        naarDagStand_Entry();
        mainState = naarDagStand;
      }
      else
        nachtStand_Do();
      break;
    case naarDagStand:
      if(isSlagboomDicht())
      {
        rustStand_Entry();
        naarDagStand_Exit();
        mainState = rustStand;
      }
      else
        naarDagStand_Do();
      break;
    case dodeTijd:
      if(currentTime - getTimerKruispuntLeeg() >= DODE_TIJD)
      {
        dodeTijd_Exit();
        rustStand_Entry();
        mainState = rustStand;
      }
      else
        dodeTijd_Do();
      break;
      
    case openenVoetgangersOvergang:
      if(isSlagboomOpen())
      {
        openenVoetgangersOvergang_Exit();
        voetgangersOvergangOpen_Entry();
        mainState = voetgangersOvergangOpen;
      }
      else
        openenVoetgangersOvergang_Do();
      break;
    case voetgangersOvergangOpen:
      if(currentTime - getTimerBuzzerVoetgangersOvergangOpen() >= TIJD_BUZZER_AAN_OPENEN_VOETGANGERS)
      {
        voetgangersOvergangOpen_Exit();
        aftellenVoetgangersOvergang_Entry();
        mainState = aftellenVoetgangersOvergang;
      }
      else
        voetgangersOvergangOpen_Do();
      break;
    case aftellenVoetgangersOvergang:
      if(currentTime >= getTimerAftellenDisplay())
      {
        aftellenVoetgangersOvergang_Exit();
        mainState = afsluitenVoetgangersOvergang;
        afsluitenVoetgangersOvergang_Entry();
      }
      else
        aftellenVoetgangersOvergang_Do();
      break;
    case afsluitenVoetgangersOvergang:
      if(isSlagboomDicht())
      {
        afsluitenVoetgangersOvergang_Exit();
        dodeTijd_Entry();
        mainState = dodeTijd;
      }
      else
        afsluitenVoetgangersOvergang_Do();
      break;

    case stoplichtOvergangNaarDoorRijden:
      if(currentTime >= getTimerOvergangNaarDoorRijden())
      {
        stoplichtOvergangNaarDoorRijden_Exit();
        stoplichtDoorRijden_Entry();
        mainState = stoplichtDoorRijden;
      }
      else
        stoplichtOvergangNaarDoorRijden_Do();
      break;
    case stoplichtDoorRijden:
      if(currentTime - getTimerDoorRijden() >= getTIJD_OP_DOOR_RIJDEN())
      {
        stoplichtDoorRijden_Exit();
        if(getLaatstOntvangenKarakter() == 'O')
        {
          stoplichtDoorRijdenKnipper_Entry();
          mainState = stoplichtDoorRijdenKnipper;
        }
        else
        {
          stoplichtOvergangNaarStoppen_Entry();
          mainState = stoplichtOvergangNaarStoppen;
        }
      }
      else
        stoplichtDoorRijden_Do();
      break;
    case stoplichtDoorRijdenKnipper:
      if(currentTime - getTimerDoorRijden() >= getKNIPPER_TIJD_OOSTERIJK())
      {
        stoplichtDoorRijdenKnipper_Exit();
        stoplichtOvergangNaarStoppen_Entry();
        mainState = stoplichtOvergangNaarStoppen;
      }
      else
        stoplichtDoorRijdenKnipper_Do();
      break;
    case stoplichtOvergangNaarStoppen:
      if(currentTime - getTimerOvergangNaarStoppen() >= getTUSSEN_TIJD_NAAR_STOPPEN())
      {
        stoplichtOvergangNaarStoppen_Exit();
        dodeTijd_Entry();
        mainState = dodeTijd;
      }
      else
        stoplichtOvergangNaarStoppen_Do();
      break;
    case test:
      if(testCompleted())
      {
        test_Exit();
        rustStand_Entry();
        mainState = rustStand;
      }
      else
        test_Do();
      break;
  }
}

