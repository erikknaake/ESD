char laatstOntvangenKarakter = '\0';

//Update het laatstOntvangenKarakter als deze er is.
void serialLoop()
{
  if(Serial.available())
    laatstOntvangenKarakter = Serial.read();
}

char getLaatstOntvangenKarakter() { return toupper(laatstOntvangenKarakter); }
