CameraView = {}

CameraView.DrawUI = function(comp)
    atImGui.Text("Camera View")
    local scene = atCore.GetScene()
    
    local lastActive = scene:IsActiveCamera(comp:Node())
    local res = atImGui.Checkbox("IsActive", lastActive)
    if lastActive ~= res then
        if res then
            scene:AddActiveCamera(comp:Node())
        else
            scene:RemoveActiveCamera(comp:Node())
        end
    end
end

return CameraView