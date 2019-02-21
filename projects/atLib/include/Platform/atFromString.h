#ifndef atFromString_h__
#define atFromString_h__

class atString;

template <> std::wstring atFromString<std::wstring>(const atString &str);
template <> double atFromString<double>(const atString &str);
template <> float atFromString<float>(const atString &str);
template <> int8_t atFromString<int8_t>(const atString &str);
template <> int16_t atFromString<int16_t>(const atString &str);
template <> int32_t atFromString<int32_t>(const atString &str);
template <> int64_t atFromString<int64_t>(const atString &str);
template <> uint8_t atFromString<uint8_t>(const atString &str);
template <> uint16_t atFromString<uint16_t>(const atString &str);
template <> uint32_t atFromString<uint32_t>(const atString &str);
template <> uint64_t atFromString<uint64_t>(const atString &str);

#endif // atFromString_h__
