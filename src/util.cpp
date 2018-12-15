#include <util.hpp>
#ifdef __APPLE__
    #include <machine/endian.h>
#elif __linux__
    #include <endian.h>
#endif
#include <stdio.h>

const signed char p_util_hexdigit[256] =
{ -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,
  -1,0xa,0xb,0xc,0xd,0xe,0xf,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,0xa,0xb,0xc,0xd,0xe,0xf,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1, 
};

signed char hexDigit(char c)
{
    return p_util_hexdigit[(unsigned char)c];
}

bool isHex(const std::string& str)
{
    for(std::string::const_iterator it(str.begin()); it != str.end(); ++it)
    {
        if (hexDigit(*it) < 0)
            return false;
    }
    return (str.size() > 0) && (str.size()%2 == 0);
}

std::vector<unsigned char> parseHex(const char* psz)
{
    // convert hex dump to vector
    std::vector<unsigned char> vch;

    std::string pszString(psz);
    if(!isHex(pszString))
    {
        return vch;
    }
    while (true)
    {
        while (isSpace(*psz))
            psz++;
        signed char c = hexDigit(*psz++);
        if (c == (signed char)-1)
            break;
        unsigned char n = (c << 4);
        c = hexDigit(*psz++);
        if (c == (signed char)-1)
            break;
        n |= c;
        vch.push_back(n);
    }
    return vch;
}

bool isSpace(char c)
{
    return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}

std::vector<unsigned char> nBitsToTarget(uint32_t nBits)
{
    std::vector<unsigned char> target(32);
    int size = nBits >> 24;
    target.end()[-size] = (nBits & 0x00ff0000) >> 16;
    target.end()[-size + 1] = (nBits & 0x0000ff00) >> 8;
    target.end()[-size + 2] = nBits & 0x000000ff;
    return target;
}

std::vector<unsigned char> intToHex(uint32_t value)
{
    std::vector<unsigned char> charVector = {
	    (value & 0x000000ff),
	    (value & 0x0000ff00)  >> 8,
	    (value & 0x00ff0000)  >> 16,
	    (value & 0xff000000)  >> 24
    };
    return charVector;
};

std::vector<unsigned char> bigIntToHex(uint64_t value)
{
    std::vector<unsigned char> charVector = {
	    (value & 0x00000000000000ff),
	    (value & 0x000000000000ff00)  >> 8,
	    (value & 0x0000000000ff0000)  >> 16,
	    (value & 0x00000000ff000000)  >> 24,
	    (value & 0x000000ff00000000)  >> 32,
	    (value & 0x0000ff0000000000)  >> 40,
	    (value & 0x00ff000000000000)  >> 48,
	    (value & 0xff00000000000000)  >> 56
    };
    return charVector;
};
