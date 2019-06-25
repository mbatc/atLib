#include "atString.h"
#include "atScan.h"
#include <codecvt>

template<> std::wstring atFromString<std::wstring>(const atString &str) { std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter; return converter.from_bytes(str.c_str()); }
template<> std::string atFromString<std::string>(const atString &str) { return str.c_str(); }
template<> double atFromString<double>(const atString &str) { return atScan::Float(str); }
template<> float atFromString<float>(const atString &str) { return (float)atScan::Float(str); }
template<> int8_t atFromString<int8_t>(const atString &str) { return (int8_t)atScan::Int(str); }
template<> int16_t atFromString<int16_t>(const atString &str) { return (int16_t)atScan::Int(str); }
template<> int32_t atFromString<int32_t>(const atString &str) { return (int32_t)atScan::Int(str); }
template<> int64_t atFromString<int64_t>(const atString &str) { return (int64_t)atScan::Int(str); }
template<> uint8_t atFromString<uint8_t>(const atString &str) { return (uint8_t)atScan::Int(str); }
template<> uint16_t atFromString<uint16_t>(const atString &str) { return (uint16_t)atScan::Int(str); }
template<> uint32_t atFromString<uint32_t>(const atString &str) { return (uint32_t)atScan::Int(str); }
template<> uint64_t atFromString<uint64_t>(const atString &str) { return (uint64_t)atScan::Int(str); }
