#ifndef atBPGNetwork_h__
#define atBPGNetwork_h__

#include <atMatrixNxM.h>

template<typename T>
struct NetworkNode
{
  T value;
};

template<typename T>
class NetworkLayer
{
  atMatrixNxM<T, >;
  atVector<NetworkNode<T>>
};

template<typename InType, typename OutType, typename LayerType = InType>
class atBPGNetwork
{
public:
  atBPGNetwork(const atVector<int64_t> &layers);
  atBPGNetwork(int64_t inputCount, int64_t outputCount, int64_t layerCount, int64_t layerSize = 16);

  atVector<OutType> Run(const atVector<InType> &inputs);

  bool Train(const atVector<InType> &inputs, const atVector<OutType> &outputs);

protected:
  atVector<NetworkNode<InType>> m_inputs;
  atVector<NetworkNode<LayerType>> m_layers;
  atVector<NetworkNode<OutType>> m_outputs;
};

#include "atBPGNetwork.inl"
#endif // atBPGNetwork_h__
