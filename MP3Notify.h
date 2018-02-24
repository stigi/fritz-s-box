#include "Context.h"

class Mp3Notify {
  public:
    static void OnError(uint16_t errorCode) {
      // see DfMp3_Error for code meaning
      Serial.print(F("Com Error "));
      Serial.println(errorCode);
    }

    static void OnPlayFinished(uint16_t globalTrack, void *ctx) {
      Serial.print(F("Play finished for #"));
      Serial.println(globalTrack);

      handlePlayFinished((Context *)ctx);
    }

    static void OnCardOnline(uint16_t code) {
      Serial.print(F("Card online "));
      Serial.println(code);
    }

    static void OnCardInserted(uint16_t code) {
      Serial.print(F("Card inserted "));
      Serial.println(code);
    }

    static void OnCardRemoved(uint16_t code) {
      Serial.print(F("Card removed "));
      Serial.println(code);
    }
};

