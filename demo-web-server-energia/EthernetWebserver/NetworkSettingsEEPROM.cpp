#include <Arduino.h>
#include <EEPROM.h>
#include "NetworkSettingsEEPROM.h"

#define IP_OFFSET (_addressEEPROM)
#define DNS_OFFSET (IP_OFFSET + 4)
#define GW_OFFSET (DNS_OFFSET + 4)
#define SUBNET_OFFSET (GW_OFFSET + 4)

NetworkSettingsEEPROM::NetworkSettingsEEPROM(int addressEEPROM) {
  _addressEEPROM = addressEEPROM;
}

void NetworkSettingsEEPROM::readSettings() {
  byte ip[4];
  byte dns[4];
  byte gw[4];
  byte subnet[4];

  readIp(ip);
  readDns(dns);
  readGateway(gw);
  readSubnet(subnet);
}

void NetworkSettingsEEPROM::writeSettings(byte mac[], byte ip[], byte dns[], byte gw[], byte subnet[]) {
  writeEEPROM(ip, IP_OFFSET, 4);
  writeEEPROM(dns, DNS_OFFSET, 4);
  writeEEPROM(gw, GW_OFFSET, 4);
  writeEEPROM(subnet, SUBNET_OFFSET, 4);
}

void NetworkSettingsEEPROM::readIp(byte ip[]) {
  readEEPROM(ip, IP_OFFSET, 4);
}

void NetworkSettingsEEPROM::readDns(byte dns[]) {
  readEEPROM(dns, DNS_OFFSET, 4);
}

void NetworkSettingsEEPROM::readGateway(byte gw[]) {
  readEEPROM(gw, GW_OFFSET, 4);
}

void NetworkSettingsEEPROM::readSubnet(byte subnet[]) {
  readEEPROM(subnet, SUBNET_OFFSET, 4);
}

void NetworkSettingsEEPROM::readEEPROM(byte data[], int offset, int length) {
  for (int i = 0; i < length; i++) {
    data[i] = EEPROM.read(offset + i);
  }
}

void NetworkSettingsEEPROM::writeEEPROM(byte data[], int offset, int length) {
  for (int i = 0; i < length; i++) {
    EEPROM.write(offset + i, data[i]);
  }
}
