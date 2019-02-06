require(atCore.packageDir .. "." .. "SceneComponentView.ScriptView") 
require(atCore.packageDir .. "." .. "SceneComponentView.MeshView")
require(atCore.packageDir .. "." .. "SceneComponentView.CameraView")

ComponentView = {}

ComponentView.OnGui = function()
	local comp = atCore.Packages["SceneInteraction"].GetSelectedComponent()
	
	atImGui.PushColour(atCore.Packages["GuiStyles"].mainStyleID)
	atImGui.PushStyle(atCore.Packages["GuiStyles"].mainStyleID)
	if atImGui.Begin("Component View") then
		if comp:Valid() then
			typeID = comp:TypeID()
			if typeID == atScene.Script.TypeID then
				ScriptView.DrawUI(atScene.Script.new(comp))
			end

			if typeID == atScene.Mesh.TypeID then
				MeshView.DrawUI(atScene.Mesh.new(comp))
			end

			if typeID == atScene.Camera.TypeID then
				CameraView.DrawUI(atScene.Camera.new(comp))
			end
		end
	end
	
	atImGui.End()
	atImGui.PopStyle()
	atImGui.PopColour()
end

return ComponentView