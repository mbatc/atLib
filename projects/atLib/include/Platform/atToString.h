#ifndef atToString_h__
#define atToString_h__

class atString;

atString atToString(const std::wstring &str);
atString atToString(const std::string &str);
atString atToString(const int8_t val);
atString atToString(const int16_t val);
atString atToString(const int64_t val);
atString atToString(const int32_t val);
atString atToString(const uint8_t val);
atString atToString(const uint16_t val);
atString atToString(const uint64_t val);
atString atToString(const uint32_t val);
atString atToString(const double val);
atString atToString(const float val);
atString atToString(const bool val, const bool verbose = true);

#endif // atToString_h__
