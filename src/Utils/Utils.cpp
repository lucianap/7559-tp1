#include "Utils.h"
#include <string>
#include <vector>
#include <iostream>
#include <getopt.h>

void Utils::join(const std::vector<std::string>& v, char c, std::string& s) {
   s.clear();
   for (std::vector<std::string>::const_iterator p = v.begin();
        p != v.end(); ++p) {
      s += *p;
      if (p != v.end() - 1)
        s += c;
   }
}

void Utils::join(const std::vector<std::string>& v, std::string& s) {
    s.clear();
    for (std::vector<std::string>::const_iterator p = v.begin();
         p != v.end(); ++p) {
        s += *p;
    }
}

std::string& Utils::ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::string& Utils::rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}

std::string& Utils::trim(std::string& str)
{
    const std::string& chars = "\t\n\v\f\r ";
    return ltrim(rtrim(str, chars), chars);
}

t_parametros Utils::tomarParametros(int argc,char* argv[]) {
   int c;
   bool pendingParams = true;
   t_parametros params;
   params.cantProductores = 0;
   params.cantDistribuidores = 0;
   params.cantPuntosVenta = 0;
   params.debug = false;

   while (pendingParams) {
      static struct option long_options[] =
          {
              {"productores",  required_argument, nullptr, 'p'},
              {"distribuidores",  required_argument, nullptr, 'd'},
              {"puntosventa",  required_argument, nullptr, 'v'},
              {"debug",  no_argument, nullptr, 'x'},
              {0, 0, 0, 0}
          };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long(argc, argv, "p:d:v:x",
                 long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
         break;

      switch (c) {
         case 'p':
            params.cantProductores = atoi(optarg);
              break;
         case 'd':
            params.cantDistribuidores = atoi(optarg);
              break;
         case 'v':
            params.cantPuntosVenta = atoi(optarg);
              break;
         case 'x':
            params.debug = true;
              break;

         case '?':
            /* getopt_long already printed an error message. */
            break;

         default:
            pendingParams = false;
      }
   }

   return params;
}
