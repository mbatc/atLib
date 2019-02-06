NodeView = {}

NodeView.OnStartup = function(dt)
    print("Node View: OnStartup()")
end

NodeView.OnCleanup = function(dt)
    print("Node View: OnCleanup()")
end

NodeView.OnGui = function()
	atImGui.PushColour(atCore.Packages["GuiStyles"].mainStyleID)
	atImGui.PushStyle(atCore.Packages["GuiStyles"].mainStyleID)

	local wndWidth = atCore.GetWindowWidth()
	local wndHeight = atCore.GetWindowHeight()

	if atImGui.Begin("Node View") then
		local nodeID = atCore.Packages["SceneInteraction"].selectedNode
		local scene = atCore.GetScene()
		local node = scene:GetNode(nodeID)

		if nodeID ~= -1 then
			atImGui.PushID(nodeID)
			atImGui.Text("Node Name: ")
			atImGui.SameLine()
			node:SetName(atImGui.TextInput("", node:GetName()))
			
			atImGui.NewLine()
			local result = atImGui.Input3("Position", node:GetPosition())
			if atImGui.IsItemActive() then
				node:SetPosition(result)
			end

			result = atImGui.Input3("Rotation", node:GetRotation())
			if atImGui.IsItemActive() then
				node:SetRotation(result)
			end

			result = atImGui.Input3("Scale", node:GetScale())
			if atImGui.IsItemActive() then
				node:SetScale(result)
			end
			atImGui.PopID()
			
			atImGui.Text("Components")
			atImGui.BeginChild("ComponentList", atMath.Vec2.new(0, 0), true)
			
			local componentList = node:GetComponents()
			for i, comp in pairs(componentList) do
				if atImGui.Selectable("Component ".. i .. ", TypeID" .. comp:TypeID(), atCore.Packages["SceneInteraction"].selectedComponent == i - 1) then
					atCore.Packages["SceneInteraction"].selectedComponent = i - 1
				end
			end
			if atImGui.Selectable("Add New Component", false) then
				atImGui.OpenPopup("AddComponentPopup")
			end

			local addCompType = -1
			if atImGui.BeginPopup("AddComponentPopup") then
				if atImGui.Button("Add Camera") then
					addCompType = atScene.Camera.TypeID
				end
				
				if atImGui.Button("Add Mesh") then
					addCompType = atScene.Mesh.TypeID
				end
				
				if atImGui.Button("Add Script") then
					addCompType = atScene.Script.TypeID
				end
				atImGui.EndPopup()
			end

			atImGui.EndChild()
			
			if addCompType ~= -1 then
				 node:AddComponent(addCompType)
			end
		end
	end
	atImGui.End()
	atImGui.PopStyle()
	atImGui.PopColour()
end

return NodeView