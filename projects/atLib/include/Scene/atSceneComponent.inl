
template<typename T> inline T* atSceneComponent::As() const { return Is<T>() ? (T*)this : nullptr; }
template<typename T> inline bool atSceneComponent::Is() const { return typeID == T::TypeID(); }
