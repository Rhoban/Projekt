#include <sstream>
#include "util.h"

std::vector<std::string> split(const std::string &s, char delim, int limit) {
    if (limit > 0) limit--;
    std::vector<std::string> parts;

    std::stringstream ss(s);
    std::string item;
    while (limit!=0 && getline(ss, item, delim)) {
        parts.push_back(item);
        limit--;
    }
    unsigned int c;
    std::string end = "";
    while ((c = ss.get()) <= 256) {
        end += (char)c;
    }
    if (end != "") {
        parts.push_back(end);
    }
    return parts;
}
