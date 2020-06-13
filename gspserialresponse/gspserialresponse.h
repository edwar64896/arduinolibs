#ifndef GSP_SERIAL_RESPONSE
#define GSP_SERIAL_RESPONSE

#include "Arduino.h"
#include "gspgrouped.h"

class gspSerialResponse: public gspGrouped {

    public:

        static gspSerialResponse * makeOne(
                            const char* _szInput /*Parser input - this is the string we look for*/, 
                            uint8_t _nChars /*number of characters to pull from the serial stream after the parser input has passed */,
                            void (* _callback)(String) /*callback to invoke upon successful parse*/
        ) {
            gspSerialResponse * instance = new gspSerialResponse(_szInput,_nChars,_callback);
            gspGrouped::register_instance(instance);
        return instance;
    }

        gspSerialResponse(  
                            const char* szInput /*Parser input - this is the string we look for*/, 
                            uint8_t nChars /*number of characters to pull from the serial stream after the parser input has passed */,
                            void (* callback)(String) /*callback to invoke upon successful parse*/
        );

        gspSerialResponse();
        ~gspSerialResponse();

        bool check();
        void reset();
        String getLastResponse();

        static void checkAll() {
            gspGrouped::checkAll(gspSerialResponse::firstInstance);
        }

    protected:
        void setFirstInstance(gspGrouped * fi) {gspSerialResponse::firstInstance = fi;}
        gspGrouped * getFirstInstance() {return gspSerialResponse::firstInstance;}
        static gspGrouped * firstInstance;

    private:

        void (*cbProcessor)(String) = nullptr;
        const char* szHeader=nullptr;
        uint8_t iCharsToGrab=0;
        uint8_t cursor=0;
        String getChars(uint8_t nChars);
        String _lastResponse;
        char getChar();
        int parseWin=0;

};

#endif
