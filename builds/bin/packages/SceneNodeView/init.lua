NodeView = {}

NodeView.OnStartup = function(dt)
    print("Node View: OnStartup()")
end

NodeView.OnCleanup = function(dt)
    print("Node View: OnCleanup()")
end

NodeView.OnGui = function()
	atImGui.Begin("Node View"	)
	atImGui.Button("Test Button")
	atImGui.End()
end

return NodeView