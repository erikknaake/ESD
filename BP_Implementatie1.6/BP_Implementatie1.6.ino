void setup() {
  Serial.begin(9600);
  slagboomSetup();
  stoplichtSetup();
  buttonSetup();
  dagNachtSetup();
  registerSetup();
  buzzerSetup();

  stateMachineSetup();
}


void loop() {
  if(isDag())
    checkButtons();
  runStatemachine();
  serialLoop();
  buzzerMaakGeluid();
}
