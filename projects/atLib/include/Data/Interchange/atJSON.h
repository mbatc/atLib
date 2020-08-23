#ifndef atJSON_h__
#define atJSON_h__

#include "atString.h"
#include "atHashMap.h"

class atJSON
{
public:
  atJSON(atJSON &&o);
  atJSON(const atJSON &o);
  atJSON(const atString &json = "");
  ~atJSON();

  const atJSON& operator=(atJSON &&o);
  const atJSON& operator=(const atJSON &o);

  // Parse a JSON string and assign the result to this object
  bool Parse(const atString &json);

  // Set the JSON value from an object
  // atToString must be defined for for type T
  // This isString to true by default so for standard 
  // JSON types it is recommended to use the typed functions
  template <typename T>
  void SetValue(const T &val, const bool &isString = true);

  // Convert the JSON value to an object
  // atFromString must be specialized for type T
  template <typename T>
  T GetValueAs() const;

  // Set the JSON value to an integer
  void SetInt(const int &val);

  // Set the JSON value to a bool.
  // If verbose is true then true/false will be used.
  // Otherwise, 0 and 1 will be used.
  void SetBool(const bool &val, const bool verbose = true);

  // Set the JSON value to a floating point number
  void SetDecimal(const float &val);
  void SetDecimal(const double &val);

  // Convert to an integer
  // Cannot fail
  int64_t ToInt() const;

  // Convert to a float
  // Cannot fail
  float ToFloat() const;

  // Convert to a double
  // Cannot fail
  double ToDouble() const;

  // Convert to a bool
  // Cannot fail
  bool ToBool() const;

  // Check if this is a null JSON element
  // Cannot fail
  bool IsNull() const;
  
  // Check if this JSON element is an array
  // Cannot fail
  bool IsArray() const;

  // Check if this JSON element is an object
  // Cannot fail
  bool IsObject() const;

  // Check if this JSON element is a strings
  // Cannot fail
  bool IsString() const;

  // Check if this JSON element is a value type (decimal, integer, bool)
  // Cannot fail
  bool IsValue() const;

  // Set an array value. If index > ElementCount(), then the array will
  // be padded with null elements.
  // If JSON element is not an array, current data will be
  // discarded and this JSON element will become an object.
  void SetElement(const int64_t &index, const atJSON &value);

  // Set a member of this JSON object.
  // If JSON element is not an object, current data will be
  // discarded and this JSON element will become an object.
  void SetMember(const atString &key, const atJSON &value);

  // Return the number of elements that are contained in this JSON value
  int64_t ElementCount() const;

  atJSON& GetMember(const atString &key);
  atJSON& GetElement(const int64_t &index);
  atJSON* TryGetMember(const atString &key) const;
  atJSON* TryGetElement(const int64_t &index) const;

  atVector<atString> GetKeys() const;

  // Ensure JSON is a Value type before calling the function
  const atString& Value() const;
  
  // Ensure JSON is an Array type before calling the function
  const atVector<atJSON>& Array() const;
  
  // Ensure JSON is a Object type before calling the function
  const atHashMap<atString, atJSON>& Object() const;

  // Object accessors
  atJSON& operator[](const atString &key);
  const atJSON& operator[](const atString &key) const;

  // Array accessors
  atJSON& operator[](const int64_t &index);
  const atJSON& operator[](const int64_t &index) const;

  // Export this json object as a json string
  // If prettyPrint is true then whitespace will be
  // added to help readability
  atString ToString(const bool prettyPrint = false) const; 

  // Change the JSON element type
  void MakeNull();
  void MakeArray();
  void MakeObject();
  void MakeValue(const bool &isString);

protected:
  bool m_isString = false;
  atString *m_pValue = nullptr;
  atVector<atJSON> *m_pArray = nullptr;
  atHashMap<atString, atJSON> *m_pObject = nullptr;

  int64_t Parse(const char *pStart, const int64_t &length);
};

atString atToString(const atJSON &json);
template<> atJSON atFromString<atJSON>(const atString &json);

#include "atJSON.inl"
#endif // atJSON_h__

