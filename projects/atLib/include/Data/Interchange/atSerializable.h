#ifndef atSerializable_h__
#define atSerializable_h__

#include "atXML.h"
#include "atJSON.h"

class atObjectDescriptor
{
public:
  atObjectDescriptor();
  atObjectDescriptor(const atXML &xml);
  atObjectDescriptor(const atJSON &json);

  void Export(atXML *pXML);
  void Export(atJSON *pJSON);

};

#endif // atSerializable_h__
