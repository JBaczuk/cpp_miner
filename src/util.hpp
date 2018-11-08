#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <vector>

signed char hexDigit(char c);
bool isHex(const std::string& str);
std::vector<unsigned char> parseHex(const char* psz);
bool isSpace(char c);
std::vector<unsigned char> nBitsToTarget(uint32_t nBits);

#endif

