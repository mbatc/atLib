SceneView = {}

SceneView.OnStartup = function()
    print("Scene View: OnStartup()")
end

SceneView.OnCleanup = function()
    print("Scene View: OnCleanup()")
end

SceneView.OnGui = function()
	atImGui.Begin("Scene View")
	atImGui.Selectable("Scene Selectable", false)
	atImGui.End()
end

return SceneView