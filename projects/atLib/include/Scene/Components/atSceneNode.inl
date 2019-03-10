
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2018 Michael Batchelor, 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------

template<typename T> inline atVector<T*> atSceneNode::Components() const
{
  atVector<T*> ret;
  for (atSceneComponent *pComp : m_components)
    if (pComp->Is<T>())
      ret.push_back((T*)pComp);
  return ret;
}

template<typename T> inline int64_t atSceneNode::ComponentCount() const
{
  int64_t count = 0;
  for (atSceneComponent *pComp : m_components)
    count += pComp->Is<T>();
  return count;
}

template<typename T> inline T* atSceneNode::AddComponent(T *pComponent)
{
  atIsValidSceneComponentType<T>();
  pComponent->m_pNode = this;
  m_components.push_back(pComponent);
  return pComponent;
}

template<typename T> inline T* atSceneNode::AddComponent() { return AddComponent(atNew<T>()); }
template<typename T> inline T* atSceneNode::Component(const int64_t index) const { return Components<T>()[index]; }
