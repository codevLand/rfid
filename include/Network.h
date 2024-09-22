#ifndef NETWORK_H
#define NETWORK_H

class Network {
  private:
    /* data */

  public:
    Network();
    // ~Network();

    // wifi / sta functions
    void wifiConnect();
    bool isWifiConnected();

    // ap / hotspot functions
    void apConnect();
    void connectedDevices();

};

#endif