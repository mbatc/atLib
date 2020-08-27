
template<typename T> inline T* atSceneComponent::As(const bool &allowDerived) { return Is<T>() || (allowDerived && IsDerived<T>()) ? (T*)this : nullptr; }
template<typename T> inline const T* atSceneComponent::As(const bool &allowDerived) const { return Is<T>() || (allowDerived && IsDerived<T>()) ? (const T*)this : nullptr; }
template<typename T> inline bool atSceneComponent::Is() const { return typeID == T::TypeID(); }
template<typename T> inline bool atSceneComponent::IsDerived() const { return IsDerived(T::TypeID()); }
