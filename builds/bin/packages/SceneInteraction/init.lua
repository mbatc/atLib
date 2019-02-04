SceneInteraction = {}

SceneInteraction.selectedNode = -1
SceneInteraction.selectedComponent = -1

SceneInteraction.SelectNode = function(id)
	DeselectNode(id)
	SceneInteraction.selectedNode = id
	return true
end

SceneInteraction.DeselectNode = function()
	SceneInteraction.selectedNode = -1
	SceneInteraction.DeselectComponent()
	return true
end

SceneInteraction.IsIDSelected = function(id)
	return id == SceneInteraction.selectedNode
end

SceneInteraction.IsNodeSelected = function(node)
	return node:GetID() == SceneInteraction.selectedNode
end

SceneInteraction.SelectComponent = function(index)
	SceneInteraction.DeselectComponent()
	local node = SceneInteraction.GetSelectedNode(SceneInteraction.selectedNode)
	if not node:Valid() then
		return false
	end
	
	if index < 0 or index >= node:ComponentCount() then
		return false
	end
	
	SceneInteraction.selectedComponent = index
	return true
end

SceneInteraction.DeselectComponent = function()
	SceneInteraction.selectedComponent = -1
end

SceneInteraction.GetSelectedNode = function()
  local scene = atCore.GetScene()
  return scene:GetNode(SceneInteraction.selectedNode)
end

SceneInteraction.GetSelectedComponent = function()
  local node = SceneInteraction.GetSelectedNode()
  return node:GetComponent(SceneInteraction.selectedComponent)
end

return SceneInteraction