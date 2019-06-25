#include "atString.h"
#include <codecvt>
#include "atPrint.h"

atString atToString(const std::wstring &str) { std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> coverter; return atString(coverter.to_bytes(str.c_str())); }
atString atToString(const std::string &val) { return atString(val.c_str(), val.c_str() + val.length()); }
atString atToString(const int8_t val) { return atPrint::Int(val); }
atString atToString(const int16_t val) { return atPrint::Int(val); }
atString atToString(const int64_t val) { return atPrint::Int(val); }
atString atToString(const int32_t val) { return atPrint::Int(val); }
atString atToString(const uint8_t val) { return atPrint::Int(val); }
atString atToString(const uint16_t val) { return atPrint::Int(val); }
atString atToString(const uint64_t val) { return atPrint::Int(val); }
atString atToString(const uint32_t val) { return atPrint::Int(val); }
atString atToString(const double val) { return atPrint::Float(val); }
atString atToString(const float val) { return atPrint::Float(val); }
