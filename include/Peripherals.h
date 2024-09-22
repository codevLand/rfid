#ifndef PERIPHERALS_H
#define PERIPHERALS_H

// typedef struct {
//   uint8_t pin;
//   bool gateOpen;
//   unsigned long gateOpenTime;
// } AC_SETTINGS;

class Periperals {
  private:
    /* data */

  public:
    Periperals();
    // ~Periperals();

    void start();
    void readRFID();
    void relaySetup();
    void lcdSetup();

    bool isGateLocked();
    void verifyGateAccess();
    void allowGatePassage();
    void denyGatePassage();

  protected:
    /* dont ask me why */
};

#endif