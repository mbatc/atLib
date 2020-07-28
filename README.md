##  Welcome to A Typical Library

atLib is a collection of tools written in C++ mainly focusing on 3D graphics (but anything goes really).

For me, this library is a hobby project which I develop on and off in my free time. As such there are no guaratees made that everything will work as intended. I'm sure there are probably plenty of bugs (big and small) scattered about.


## Licensing

All source code in this repository is License under the following:

### The MIT License

```
Copyright(c) 2018 Michael Batchelor,

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
```

All 3rdParty code is distributed under the licenses supplied by it's owner/s. Details can be found in the following section, or in the folder containing the goven 3rdParty source or binaries.

## 3rd Party Library Acknowledgements

### STB https://github.com/nothings/stb 

stb single-file public domain libraries for C/C++ (https://twitter.com/nothings)

The following files were used in atImageHelper.cpp
  
* stb_image_write.h
* stb_image.h

The following files were used in atFont.h and atFont.cpp

* stb_truetype.h

### SoL https://github.com/ThePhD/sol2

Sol is a C++ library binding to Lua. It currently supports all Lua versions 5.1+ (LuaJIT 2.x included). Sol aims to be easy to use and easy to add to a project. The library is header-only for easy integration with projects.

* Sol.hpp is used in atLua.cpp

### Lua https://www.lua.org/

The Programming language Lua (included for SoL)

```
Copyright © 1994–2018 Lua.org, PUC-Rio.
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```
