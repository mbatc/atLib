SceneView = {}

SceneView.OnStartup = function()
    print("Scene View: OnStartup()")
end

SceneView.OnCleanup = function()
    print("Scene View: OnCleanup()")
end

SceneView.OnGui = function()
	local scene = atCore.GetScene()
	local wndWidth = atCore.GetWindowWidth()
	local wndHeight = atCore.GetWindowHeight()
	scene:SetViewport(0, 0, wndWidth, wndHeight)

	atImGui.PushColour(atCore.Packages["GuiStyles"].mainStyleID)
	atImGui.PushStyle(atCore.Packages["GuiStyles"].mainStyleID)

	atImGui.Begin("Scene View")
	if atImGui.BeginChild("SceneViewFrame", atMath.Vec2.new(0, wndHeight / 2- 60), true) then
		if atImGui.BeginTreeNode(scene:GetName()) then
			SceneView.DrawNode(scene:GetRootNode())
			atImGui.EndTreeNode()
		end
	end
	atImGui.EndChild()

	local selectedID = atCore.Packages["SceneInteraction"].selectedNode;
	local defPos = atMath.Vec3.new(0, 0, 0);
	local defRot = atMath.Vec3.new(0, 0, 0);
	local defScl = atMath.Vec3.new(1, 1, 1);
	if atImGui.Button("Add Node") then
		scene:CreateNode("New Node", defPos, defRot, defScl, selectedID)
	end
	atImGui.SameLine()

	rootSelected = selectedID == scene:GetRootNodeID()

	if rootSelected then
		atImGui.PushStyle(atCore.Packages["GuiStyles"].disabledID)
	end

	if atImGui.Button("Delete Node") then
		scene:DeleteNode(selectedID)
	end

	if rootSelected then
		atImGui.PopStyle()
	end
	
	atImGui.End()
	atImGui.PopColour()
	atImGui.PopStyle()
end

SceneView.DrawNode = function(node)
	atImGui.PushID(node:GetID())

	local isTreeOpen = false
	if node:ChildCount() ~= 0 then
		isTreeOpen = atImGui.BeginTreeNode("")
		atImGui.SameLine()
	end
	
	if atImGui.Selectable(node:GetName(), atCore.Packages["SceneInteraction"].selectedNode == node:GetID()) then
		atCore.Packages["SceneInteraction"].selectedNode = node:GetID()
	end

	if isTreeOpen then
		local nodeChildren = node:GetChildren()
		for i, child in pairs(nodeChildren) do
			SceneView.DrawNode(child)
		end

		atImGui.EndTreeNode()
	end

	atImGui.PopID(node:GetID())

end

return SceneView