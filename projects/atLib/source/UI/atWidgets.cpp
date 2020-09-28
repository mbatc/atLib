#include "atWidgets.h"
#include "atInput.h"

bool atWidgets::Input(const atString &name, atString *pValue)
{
  static atVector<char> buffer;
  buffer.resize(pValue->length() + 128);
  memcpy(buffer.data(), pValue->c_str(), pValue->length() + 1);
  if (!ImGui::InputText(name.c_str(), buffer.data(), buffer.size()))
    return false;

  *pValue = buffer.data();
  return true;
}

struct SideBarCtx
{
  atString name;
  ImVec2 pos = { 0, 0 };
  ImVec2 iconSize = { 0, 0 };
  ImVec2 iconPadding = { 0, 0 };
  ImVec2 iconFullSize = { 0, 0 };
  int64_t activeItemID = 0;
  float nextPos = 0;
};

static atHashMap<int64_t, SideBarCtx*> _sidebars;
static atVector<SideBarCtx*> _sidebarStack;

static SideBarCtx* CurrentSidebar() { return _sidebarStack.back(); }

bool atWidgets::BeginSidebar(const atString &id, const ImVec2 &position, float iconWidth, float height)
{
  if (height <= 0)
    height = ImGui::GetIO().DisplaySize.y - position.y;

  float icoFullSize = iconWidth + ImGui::GetStyle().FramePadding.x * 2;

  // Set position and size
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::SetNextWindowPos(position);
  ImGui::SetNextWindowSize(ImVec2(icoFullSize, height));

  // Create the window
  bool success = ImGui::Begin(id, 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
  ImGui::PopStyleVar(2);

  ImGui::BringWindowToFocusFront(ImGui::GetCurrentWindow());
  // Get or create the sidebar context
  int64_t sidebarID = ImGui::GetCurrentWindow()->ID;
  bool newSidebar = _sidebars.TryAdd(sidebarID, atNew<SideBarCtx>());
  _sidebarStack.push_back(_sidebars[sidebarID]);
  SideBarCtx *pCtx = CurrentSidebar();

  if (newSidebar)
  { // Do some setup stuff
    pCtx->activeItemID = 0;
  }

  pCtx->name = id;
  pCtx->nextPos = 0;
  pCtx->pos = position;
  pCtx->iconSize = { iconWidth, iconWidth };
  pCtx->iconPadding = ImGui::GetStyle().FramePadding;
  pCtx->iconFullSize = pCtx->iconSize + pCtx->iconPadding * 2;
  return success;
}

static void _DrawItemHighlight(ImVec2 pos, SideBarCtx *pCtx, ImVec4 colour)
{
  ImVec2 icoMax = pos + pCtx->iconFullSize;
  ImDrawList *pCanvas = ImGui::GetWindowDrawList();
  colour.w = 0.1f;
  pCanvas->AddRectFilled(pos, icoMax, ImGui::ColorConvertFloat4ToU32(colour));
  colour.w = 1.0f;
  pCanvas->AddRectFilled(pos, ImVec2(pos.x, icoMax.y) + ImVec2(pCtx->iconPadding.x, 0), ImGui::ColorConvertFloat4ToU32(colour));
}

bool atWidgets::SidebarItem(const atString &id, atTexture *pIcon)
{
  SideBarCtx *pCtx = CurrentSidebar();

  int64_t itemID = (int64_t)ImGui::GetID(id.c_str());
  bool isSelected = pCtx->activeItemID == itemID;

  // Position the cursor
  ImGui::SetCursorPos(ImVec2(0, pCtx->nextPos));
  ImVec2 cursorScreenPos = ImGui::GetCursorScreenPos();
  ImGui::InvisibleButton(id.c_str(), pCtx->iconFullSize);
  bool isClicked = ImGui::IsItemClicked();
  bool isHovered = ImGui::IsItemHovered();

  ImGui::SetCursorScreenPos(cursorScreenPos + pCtx->iconPadding);
  // Draw the icon
  // ImGui::Button(id.c_str(), pCtx->iconSize);
  ImGui::Image((ImTextureID)pIcon, pCtx->iconSize);

  // Draw highlighting
  if (isHovered)
    _DrawItemHighlight(cursorScreenPos, pCtx, ImGui::GetStyleColorVec4(ImGuiCol_TabHovered));
  else if (isSelected)
    _DrawItemHighlight(cursorScreenPos, pCtx, ImGui::GetStyleColorVec4(ImGuiCol_TabActive));

  if (isClicked)
    pCtx->activeItemID = isSelected ? 0 : itemID;
  pCtx->nextPos += pCtx->iconFullSize.y;

  return pCtx->activeItemID == itemID;
}

void atWidgets::EndSidebar()
{
  ImGui::End();
  _sidebarStack.pop_back();
}

bool atWidgets::BeginSidebarContent()
{
  SideBarCtx *pCtx = CurrentSidebar();
  ImVec2 contentPos = pCtx->pos + ImVec2(pCtx->iconFullSize.x, 0);
  ImGui::SetNextWindowPos(contentPos);
  ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize - contentPos);
  // ImGui::End();
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  bool open = ImGui::Begin(pCtx->name + "_Content_" + pCtx->activeItemID, 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
  ImGui::PopStyleVar(1);
  return open;
}

void atWidgets::EndSidebarContent()
{
  SideBarCtx *pCtx = CurrentSidebar();
  ImGui::End();
  // ImGui::Begin(pCtx->name);
}
