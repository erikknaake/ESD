const byte LDR = A4;
const int LICHTGRENS_NAAR_DAG = 600;
const int LICHTGRENS_NAAR_NACHT = 500;

bool dag = true;

bool isDag()
{
  int lichtWaarde = analogRead(LDR);
  Serial.println(lichtWaarde);
  if(lichtWaarde >= LICHTGRENS_NAAR_DAG)
    dag = true;
  else if(lichtWaarde <= LICHTGRENS_NAAR_NACHT)
    dag = false;
  return dag;
}

void dagNachtSetup()
{
  pinMode(LDR, INPUT);
}

void naarNachtstand_Entry() 
{
  buzzerSetFrequentie(LAGE_FREQUENTIE);
  display_ToonWachten();
}
void naarNachtstand_Do() 
{
  slagboomOmhoog();
}
void naarNachtstand_Exit() {}

void nachtStand_Entry() 
{
  for(byte i = 0; i < getAANTAL_STOPLICHTEN(); i++)
    stoplichtUit(i);
  knipperSetup();
  buzzerSetFrequentie(UIT_FREQUENTIE);
  display_ToonLetExtraOp();
}
void nachtStand_Do() 
{
  for(byte i = 0; i < getAANTAL_STOPLICHTEN(); i++)
    knipper(i, 1);
}
void nachtStand_Exit() 
{
  for(byte i = 0; i < getAANTAL_STOPLICHTEN(); i++)
  {
    stoplichtUit(i);
  }
}

void naarDagStand_Entry() 
{
  buzzerSetFrequentie(LAGE_FREQUENTIE); 
  display_ToonWachten();
  
  for(byte i = 0; i < getAANTAL_STOPLICHTEN(); i++)
    stoplichtZetLamp(i, 1, true);
}
void naarDagStand_Do() 
{
  slagboomOmlaag();  
}
void naarDagStand_Exit()
{
  for(byte i = 0; i < getAANTAL_STOPLICHTEN(); i++)
  {
    stoplichtZetLamp(i, 1, false);   
    stoplichtZetLamp(i, 0, true);   
  }
}

