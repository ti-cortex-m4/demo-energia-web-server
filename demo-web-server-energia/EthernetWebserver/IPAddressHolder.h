#ifndef IPAddressHolder_h
#define IPAddressHolder_h

#include <IPAddress.h>

class IPAddressHolder {
public:
  IPAddress ipAddress;
  bool valid;
  IPAddressHolder(const char *address);
  IPAddressHolder(const String &address);
};

#endif
