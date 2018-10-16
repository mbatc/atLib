template <typename T> static T atOBJReader::ParseVector(char **ppSrc, const int64_t srcLen, int64_t *pLen)
{
  int64_t newLine = atString::_find_first_of(*ppSrc, srcLen, "\n");
  int64_t pos = 0;
  int64_t scanLen = 0;
  T ret;
  for (double &val : ret)
  {
    if (pos >= newLine)
      break;
    val = atScan::Float(*ppSrc, &scanLen);
    *ppSrc += scanLen;
    pos += scanLen;
  }
  return ret;
}