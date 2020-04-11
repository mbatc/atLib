
template<typename ...Args>
inline std::shared_ptr<atGFXBufferInterface> atGraphics::CreateBuffer(Args&& ...args)
{
  switch (GetAPI())
  {
  case atGfxApi_DirectX: return std::make_shared<atDXBuffer>(std::forward<Args>(args)...);
  case atGfxApi_OpenGL: return std::make_shared<atGLBuffer>(std::forward<Args>(args)...);
  }
  return nullptr;
}

template<typename ...Args>
inline std::shared_ptr<atGFXTexInterface> atGraphics::CreateTexture(Args&& ...args)
{
  switch (GetAPI())
  {
  case atGfxApi_DirectX: return std::make_shared<atDXTexture>(std::forward<Args>(args)...);
  case atGfxApi_OpenGL: return std::make_shared<atGLTexture>(std::forward<Args>(args)...);
  }
  return nullptr;
}

template<typename ...Args>
inline std::shared_ptr<atGFXSamplerInterface> atGraphics::CreateSampler(Args&& ...args)
{
  switch (GetAPI())
  {
  case atGfxApi_DirectX: return std::make_shared<atDXSampler>(std::forward<Args>(args)...);
  }
  return nullptr;
}

template<typename ...Args>
inline std::shared_ptr<atGFXShaderInterface> atGraphics::CreateShader(Args&& ...args)
{
  switch (GetAPI())
  {
  case atGfxApi_DirectX: return std::make_shared<atDXShader>(std::forward<Args>(args)...);
  case atGfxApi_OpenGL: return std::make_shared<atGLShader>(std::forward<Args>(args)...);
  }
  return nullptr;
}

template<typename ...Args>
inline std::shared_ptr<atGFXPrgmInterface> atGraphics::CreateProgram(Args&& ...args)
{
  switch (GetAPI())
  {
  case atGfxApi_DirectX: return std::make_shared<atDXPrgm>(std::forward<Args>(args)...);
  case atGfxApi_OpenGL: return std::make_shared<atGLPrgm>(std::forward<Args>(args)...);
  }
  return nullptr;
}
