template<typename InType, typename OutType, typename LayerType>
inline atBPGNetwork<InType, OutType, LayerType>::atBPGNetwork(const atVector<int64_t>& layers)
{
}

template<typename InType, typename OutType, typename LayerType>
inline atBPGNetwork<InType, OutType, LayerType>::atBPGNetwork(int64_t inputCount, int64_t outputCount, int64_t layerCount, int64_t layerSize)
{
}

template<typename InType, typename OutType, typename LayerType>
inline atVector<OutType> atBPGNetwork<InType, OutType, LayerType>::Run(const atVector<InType>& inputs)
{
  return atVector<OutType>();
}

template<typename InType, typename OutType, typename LayerType>
inline bool atBPGNetwork<InType, OutType, LayerType>::Train(const atVector<InType>& inputs, const atVector<OutType>& outputs)
{
  return false;
}
