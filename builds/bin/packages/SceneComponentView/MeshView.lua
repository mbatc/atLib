MeshView = {}

MeshView.meshPath = ""

MeshView.DrawUI = function(mesh)
    atImGui.Text("Mesh View")

    if atCore.Packages["SceneInteraction"].ComponentSelectionChanged() then
        MeshView.meshPath = mesh:GetModelPath()
    end

    MeshView.meshPath = atImGui.TextInput("Model Path", MeshView.meshPath)
    atImGui.SameLine()

    if atImGui.Button("Update") then
        mesh:SetModelPath(MeshView.meshPath)
    end

    if atImGui.Button("Browse") then
        local browsePath = atCore.DialogOpen("", "Wavefront OBJ(*.obj)\0*.obj\0\0", 1)
        if browsePath ~= "" then
            MeshView.meshPath = browsePath
            mesh:SetModelPath(browsePath)
        end
    end
end

return MeshView