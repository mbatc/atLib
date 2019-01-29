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

	if atImGui.Begin("Node View", atMath.Vec2.new(wndWidth / 5, wndHeight / 2), atMath.Vec2.new(0, wndHeight / 2)) then
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
		end
	end
	atImGui.End()
	atImGui.PopStyle()
	atImGui.PopColour()
end

return NodeView