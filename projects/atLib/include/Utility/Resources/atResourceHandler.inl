
template<typename T> bool atResourceHandlerBase::Handles() const { return Handles(typeid(T)); }

template<typename T> atResourceHandler<T>::atResourceHandler(const atString &resourceTypeID) : atResourceHandlerBase(resourceTypeID) {}

template<typename T>
atObject atResourceHandler<T>::_Load(const atObjectDescriptor &request)
{
  uint8_t buffer[sizeof(T)] = { 0 };
  if (Load(request, (T *)buffer))
    return std::move(*(T*)buffer);
  return atObject();
}

template<typename T> bool atResourceHandler<T>::Handles(const std::type_index &typeInfo) const { return typeInfo == typeid(T); }