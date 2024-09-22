#include "Auth.h"

// #include <SimpleVector.h>

AuthRFIDs.put("837b9ee4", 0);

void isGatePassAuth() { return AuthdRFIDs.exists(getRFID()); }

void showAllowedUsers() {
  Serial.print("\n");
  SimpleVector<String> keys = AuthorizedRFIDs.keys();

  for (const String& key: keys) {
    Serial.print("[+] Key: " + key.c_str() + " | Value: ");
    Serial.println(*AuthorizedRFIDs.get(key));
  }
}
