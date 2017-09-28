#ifndef NetworkSettingsEEPROM_h
#define NetworkSettingsEEPROM_h

#include <Arduino.h>


class NetworkSettingsEEPROM {

  public:
  NetworkSettingsEEPROM(int addressEEPROM);
  void readSettings();
  void writeSettings(byte mac[], byte ip[], byte dns[], byte gw[], byte subnet[]);
  void readIp(byte ip[]);
  void readDns(byte dns[]);
  void readGateway(byte gw[]);
  void readSubnet(byte subnet[]);

  private:
  int _addressEEPROM;
  void readEEPROM(byte data[], int offset, int length);
  void writeEEPROM(byte data[], int offset, int length);
};

#endif
