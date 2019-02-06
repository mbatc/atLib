-- atCore Namespaces

atCore = {}
atCore.File = {}
atCore.String = {}
atCore.Packages = {}

unpack = table.unpack

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

atCore.packageDir = ""

atCore.workingDir = ""

atCore.Initialise = function(cwd, packageDir)
	atCore.packageDir = packageDir
	atCore.workingDir = cwd
    for _, dir in pairs(atCore.File.GetDirectories(cwd)) do
        atCore.Packages[dir] = require(packageDir .. dir)
        if (type(atCore.Packages[dir]) ~= 'table') then
            print("Warning: " .. dir .. " is not a table. Make sure the package table is returned in init.lua")
        else
            print("Loaded " .. dir)
        end
    end
end

atCore.Call = function(func, ...)
    for k, p in pairs(atCore.Packages) do
        if(type(p) == 'table') then
           if (p[func] ~= nil) then
               if (arg ~= nil) then
                   p[func](unpack(arg))
               else
                   p[func]()
               end
           end
        end
    end
end
