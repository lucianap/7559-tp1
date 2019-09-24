#include "Utils.h"
#include <string>
#include <vector>
#include <iostream>
#include <getopt.h>
#include <sstream>
#include <TipoFlor/TipoFlor.h>
#include <dirent.h>
#include <cstring>
#include <sys/stat.h>
#include <TipoPedido/TipoPedido.h>
#include <dirent.h>

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
   params.reanudar = false;

   while (pendingParams) {
      static struct option long_options[] =
          {
              {"productores",  required_argument, nullptr, 'p'},
              {"distribuidores", required_argument, nullptr, 'd'},
              {"puntosventa", required_argument, nullptr, 'v'},
              {"debug",  no_argument, nullptr, 'x'},
              {"reanudar",  no_argument, nullptr, 'r'},
              {0, 0, 0, 0}
          };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long(argc, argv, "p:d:v:x:r",
                 long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
         break;

      switch (c) {
         case 'r':
            params.reanudar = true;
            break;
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


std::string Utils::formatearMensajeLog(std::string mensaje) {
    time_t timestamp = time(nullptr);
    string timeString = asctime(localtime(&timestamp));
    timeString.pop_back();
    std::stringstream logMessage;
    logMessage << "pid: " << std::setw(5) << to_string(getpid()) <<" ("<< std::setw(24) << timeString <<") "<< mensaje;
    return logMessage.str();
}

string Utils::getTextTipoFlor(TipoFlor tipoFlor) {
    switch (tipoFlor) {
        case Tulipan:
            return "Tulipan";
        case Rosa:
            return "Rosa";
        default:
            return std::to_string(tipoFlor);
    }
}

string Utils::getTextTipoPedido(TipoPedido tipoPedido) {
    switch (tipoPedido) {
        case INTERNET:
            return "Internet";
        case LOCAL:
            return "Local";
        default:
            return std::to_string(tipoPedido);
    }
}

vector<string> Utils::listarArchivosConPrefijo(const char* path, string prefijo) {
    DIR *d = opendir(path);
    vector<string> files;
    if (d) {
        struct dirent *p;
        while ((p = readdir(d))) {
            if(Utils::startsWith(prefijo.c_str(), p->d_name)) {
                files.push_back(p->d_name);
            }
        }
    }
    closedir(d);

    return files;
}

bool Utils::startsWith(const char *pre, const char *str) {
    size_t lenpre = strlen(pre),
            lenstr = strlen(str);
    return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

int Utils::remove_directory(const char *path) {
    DIR *d = opendir(path);
    size_t path_len = strlen(path);
    int r = -1;

    if (d){
        struct dirent *p;
        r = 0;
        while (!r && (p=readdir(d))){
            int r2 = -1;
            char *buf;
            size_t len;
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, "..")){
                continue;
            }
            len = path_len + strlen(p->d_name) + 2;
            buf = (char*)malloc(len);
            if (buf){
                struct stat statbuf;
                snprintf(buf, len, "%s/%s", path, p->d_name);
                if (!stat(buf, &statbuf)){
                    if (S_ISDIR(statbuf.st_mode)){
                        r2 = remove_directory(buf);
                    }
                    else{
                        r2 = unlink(buf);
                    }
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }

    if (!r) {
        r = rmdir(path);
    }
    return r;
}

int Utils::countFiles(std::string carpeta) {
    int count = 0;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (carpeta.c_str())) != NULL) {
        /* print all the files and directories within directory */
        while ((ent = readdir (dir)) != NULL) {
            count++;
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("");
        return EXIT_FAILURE;
    };

    return count-2;
}

std::vector<std::string> Utils::split(const std::string &str, const std::string &delim) {

    vector<string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;

}

