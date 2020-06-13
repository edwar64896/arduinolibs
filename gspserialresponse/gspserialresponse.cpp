#include <gspserialresponse.h>

gspGrouped* gspSerialResponse::firstInstance=nullptr;

String gspSerialResponse::getLastResponse() {
    return _lastResponse; //TODO: replace this with something useful!
}

char gspSerialResponse::getChar()// Get a character from the serial buffer
{
  while (!Serial.available()); // wait for data
  return ((char)Serial.read()); // Thanks Doug
}//end of getchar

String gspSerialResponse::getChars(uint8_t nChars) {
  String retval = "";
  for (int i = 0; i < nChars;  i++) {
    retval += getChar();
  }
  retval += "       ";
  return retval.substring(0, 8); // should be 8 characters in total?
}

bool gspSerialResponse::check() {

    if (parseWin){
        _lastResponse=getChars(iCharsToGrab);
        cbProcessor(_lastResponse);
        reset();   
        return false;     
    }


    // take a peek at the incoming source.
    char incoming=Serial.peek();

    // anything there?
    if (incoming==-1) {
        return false; // force a reset of the check sequence.
    }

    // start matching the buffer
    if (incoming != (*(szHeader+(cursor++))) ) {
        cursor=0;
    }

    // if we get to here we have matched the buffer so far.
    // we need to do this twice to win the prize.

    // if we are the last instance in the list, then we need to "read" one byte from the serial input as we are done peeking
    if (getNextInstance()==nullptr)
        Serial.read();

    //only one instance should get this far.
    //TODO: work out how to do a global reset 
    if (cursor==2) {
        parseWin=1;
        return true;
    }

    return true; // continue.
}

// reset the cursor and the parse status.
// then we can have another go next time around.
void gspSerialResponse::reset() {
    cursor=0;
    parseWin=0;
}

gspSerialResponse::gspSerialResponse(const char* _szHeader, uint8_t _iCharsToGrab,void (* _cbProcessor)(String)/*callback*/)
:gspGrouped() {
    szHeader=_szHeader;
    iCharsToGrab=_iCharsToGrab;
    cbProcessor=_cbProcessor;
    cursor=0;
}

gspSerialResponse::gspSerialResponse():gspGrouped() {}

gspSerialResponse::~gspSerialResponse(){}
