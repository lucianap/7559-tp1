#include "Utils.h"
#include <string>
#include <vector>
#include <iostream>


void Utils::join(const std::vector<std::string>& v, char c, std::string& s) {
   s.clear();
   for (std::vector<std::string>::const_iterator p = v.begin();
        p != v.end(); ++p) {
      s += *p;
      if (p != v.end() - 1)
        s += c;
   }
}
