GuiStyles = {}

GuiStyles.mainStyleID = "atEMainStyle"

GuiStyles.OnStartup = function()
    atImGui.AddColours(GuiStyles.mainStyleID, [[
Text{1, 1, 1, 1};
WindowBg{0.137, 0.137, 0.137, 1.0};
Border{0, 0, 0, 0.357};
BorderShadow{0.89, 0.443, 0.306, 0.094};
FrameBg{0.105, 0.105, 0.105, 1.0};
FrameBgHovered{1.0, 0.678, 0.541, 0.4};
FramebgActive{0.5, 0.5, 0.5, 0.67};
TitleBg{0.094, 0.094, 0.094, 1.0};
TitlebgActive{0.361, 0.306, 0.255, 1.0};
ScrollBarBg{0.137, 0.137, 0.137, 1.0};
CheckMark{1.0, 1.0, 1.0, 1.0};
SliderGrab{1.0, 0.611, 0.314, 1.0};
SliderGrabActive{0.863, 0.51, 0.114, 1.0};
Button{0.11, 0.11, 0.11, 1.0};
ButtonHovered{0.98, 0.475, 0.259, 1.0};
ButtonActive{0.988, 0.678, 0.498, 1.0};
Header{0.98, 0.55, 0.25, 0.27};
HeaderHovered{0.98, 0.6, 0.25, 0.8};
HeaderActive{0.98, 0.6, 0.25, 1.0};
Separator{0.53, 0.53, 0.53, 0.5};
SeparatorHovered{0.97, 0.97, 0.97, 0.55};
SeparatorActive{1, 0.87, 0. 78, 1};
ResizeGrip{1, 0.55, 0, 0.33};
ResizeGripHovered{1, 0.55, 0, 0.68};
ResizeGripActive{1, 0.65, 0, 1};
Tab{0.4, 0.4, 0.4, 0.87};
TabHovered{0.98, 0.58, 0.27, 0.81};
TabActive{1, 0.67, 0, 1};
]])

    atImGui.AddStyle(GuiStyles.mainStyleID, "WindowRounding{0}; FrameRounding{2}; FrameBorderSize{1}")
end

return GuiStyles
