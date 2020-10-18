#ifndef atWidgets_h__
#define atWidgets_h__

#include "atImGui.h"

class atTexture;

class atWidgets
{
public:
  // IDEA: Add an label align, plus input width option similar to imgui's regular Push* functions
  static bool Input(const atString &name, bool *pValue);
  static bool Input(const atString &name, atString *pValue);
  static bool Input(atObjectDescriptor *pSerialized, const bool &modifiable = false);

  template<typename T> static bool Input(T *pValue);
  template<typename T> static bool Input(const atString &name, T *pValue);

  // Draw text with a pivot
  static void Text(const atString &text, atVec2F pivot = { 0, 1 });

  // Begin a sidebar window
  static bool BeginSidebar(const atString &id, const ImVec2 &position, float iconWidth = ImGui::GetItemsLineHeightWithSpacing(), float height = 0);

  // Add a sidebar item, returns true if this is the active item
  static bool SidebarItem(const atString &id, atTexture *pIcon);

  // End a sidebar window
  static void EndSidebar();

  static bool BeginSidebarContent();
  static void EndSidebarContent();
};

template<typename T>
inline bool atWidgets::Input(T *pValue)
{
  atObjectDescriptor serialized;
  atSerialize(&serialized, *pValue);
  bool result = Input(&serialized);
  atDeserialize(serialized, pValue);
  return result;
}

template<typename T>
inline bool atWidgets::Input(const atString &name, T *pValue)
{
  atTypeDesc desc = atGetTypeDesc<T>();
  atAssert(desc.type != atType_Unknown, "Unknown basic type T");

  int64_t stride = desc.width * atSize(desc.type);
  uint8_t *pData = (uint8_t *)pValue;

  ImGuiDataType dataType = ImGuiDataType_COUNT;
  switch (desc.type)
  {
  case atType_Float32: dataType = ImGuiDataType_Float; break;
  case atType_Float64: dataType = ImGuiDataType_Double; break;
  case atType_Int32: dataType = ImGuiDataType_S32;
  case atType_Int64: dataType = ImGuiDataType_S64;
  case atType_Uint64: dataType = ImGuiDataType_U64;
  case atType_Uint32: dataType = ImGuiDataType_U32;
  case atType_Int8: break;
  case atType_Int16: break;
  case atType_Uint8: break;
  case atType_Uint16: break;
  case atType_Float16: break;
  }

  bool modified = false;
  ImGui::PushID(name.c_str());
  for (int64_t i = 0; i < desc.count; ++i)
  {
    ImGui::PushID((int)i);
    modified |= ImGui::InputScalarN(i == 0 ? name.c_str() : "", dataType, pData, (int)desc.width);
    ImGui::PopID();
    pData += stride;
  }
  ImGui::PopID();

  return modified;
}

#endif // atWidgets_h__
