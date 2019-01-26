atCore = {}

atCore.File = {}

-- File Helpers

atCore.File.GetFiles = function(path)
    files = {}
    for dir in io.popen([[dir "]] .. path .. [[" /b]]):lines() do
        files[#files + 1] = dir
    end
    return files
end

atCore.File.GetDirectories = function(path)
    files = {}
    for dir in io.popen([[dir "]] .. path .. [[" /b /ad]]):lines() do
        files[#files + 1] = dir
    end
    return files
end

atCore.File.HasExtension = function(filename, ext)
    return filename:find("." .. ext, #filename - #ext - 1) ~= nil    
end

atCore.File.GetExtension = function(filename)
    extIndex = filename:find(".", -1)
    if(extIndex == nil) then
        return ""
    else
        return filename:sub(extIndex, #filename)
    end
end

-- String Helpers

atCore.String = {}

atCore.String.Split = function(str, sep)
    if (sep == nil) then
        sep = "%s"
    end
    local t={} ; i=1
    for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
        t[i] = str
        i = i + 1
    end
    return t
end

atCore.String.Join = function(strList, sep)
    ret = ""
    for i, str in pairs(strList) do
        ret = ret .. str
        if (i ~= #strList) then
             ret = ret .. sep
        end
    end
    return ret
end

-- Package Loading

atCore.Packages = {}

atCore.PackageDir = ""

atCore.WorkingDir = ""

atCore.Initialise = function(cwd, packageDir)
    for _, dir in pairs(File.GetDirectories()) do
        LoadPackage(dir)
    end
end

atCore.LoadPackage = function(dir)
    print("Loading LUA Package " .. dir)
end

atCore.Call = function(func, ...)
    for k, p in pairs(atCore.Packages) do
        if (p[func] ~= nil) then
            p[func](unpack(arg))
        end
    end
end

return atCore