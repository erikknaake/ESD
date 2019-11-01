unsigned long timerKruispuntLeeg = 0;
unsigned long getTimerKruispuntLeeg() { return timerKruispuntLeeg; }
const int DODE_TIJD = 3000;

void rustStand_Entry()
{
  buzzerSetFrequentie(LAGE_FREQUENTIE);
  display_ToonWachten();
  
}
void rustStand_Do() {veranderHuidigeKnopOmTeChecken();}
void rustStand_Exit() {}

void dodeTijd_Entry() 
{
  timerKruispuntLeeg = millis();
}
void dodeTijd_Do() {}
void dodeTijd_Exit() {}




