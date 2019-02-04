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
			atImGui.Text("Position:")
			atImGui.SameLine()
			atImGui.Text("" .. node:GetPosition().x .. ", " .. node:GetPosition().y .. ", ".. node:GetPosition().z)

			atImGui.Text("Rotation:")
			atImGui.SameLine()
			atImGui.Text("" .. node:GetRotation().x .. ", " .. node:GetRotation().y .. ", ".. node:GetRotation().z)

			atImGui.Text("Scale:")
			atImGui.SameLine()
			atImGui.Text("" .. node:GetScale().x .. ", " .. node:GetScale().y .. ", ".. node:GetScale().z)
			atImGui.PopID()
			
			atImGui.Text("Components")
			atImGui.BeginChild("ComponentList", atMath.Vec2.new(0, 0), true)
			
			componentList = node:GetComponents()
			for i, comp in pairs(componentList) do
				if atImGui.Selectable("Component ".. i .. ", TypeID" .. comp:TypeID(), atCore.Packages["SceneInteraction"].selectedComponent == i - 1) then
					atCore.Packages["SceneInteraction"].selectedComponent = i - 1
				end
			end
			atImGui.EndChild()
			
			addCompType = -1
			if atImGui.Button("Add Camera") then
				addCompType = atScene.Camera.TypeID
			end
			
			if atImGui.Button("Add Mesh") then
				addCompType = atScene.Mesh.TypeID
			end
			
			if atImGui.Button("Add Script") then
				addCompType = atScene.Script.TypeID
			end
			
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