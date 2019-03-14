#ifndef atBPGNetwork_h__
#define atBPGNetwork_h__

#include "atMatrixNxM.h"

class atBPGNetwork
{
public:
  struct Layer
  {
    Layer();
    int64_t nNodes;
    atMatrixNxM<double> weights;
    atMatrixNxM<double> biases;
  };

  atBPGNetwork(int64_t inputSize, int64_t outputSize, int64_t layerCount, int64_t layerSize = 16);

  atVector<double> Run(const atVector<double> &input) const;

  void Train(const atVector<double> &input, const atVector<double> &output);

  // To modify a layers weights/bias use the Get functions to retrieve the current
  // values, modify the returned matrices and then use the Set functions to set
  // the networks values.

  // The Set functions will fail if the matrix passed is the incorrect size.

  const atMatrixNxM<double>& GetLayerWeights(int64_t layer) const;
  const atMatrixNxM<double>& GetLayerBiases(int64_t layer) const;

  bool SetLayerWeights(int64_t layer, atMatrixNxM<double> weights);
  bool SetLayerBiases(int64_t layer, atMatrixNxM<double> biases);

  int64_t LayerCount() const;

  static int64_t StreamWrite(atWriteStream *pStream, const atBPGNetwork *pData, const int64_t count);
  static int64_t StreamRead(atReadStream *pStream, atBPGNetwork *pData, const int64_t count);

protected:

  atVector<Layer> m_layers;
};

int64_t atStreamRead(atReadStream *pStream, atBPGNetwork::Layer *pData, const int64_t count);
int64_t atStreamWrite(atWriteStream *pStream, const atBPGNetwork::Layer *pData, const int64_t count);
int64_t atStreamRead(atReadStream *pStream, atBPGNetwork *pData, const int64_t count);
int64_t atStreamWrite(atWriteStream *pStream, const atBPGNetwork *pData, const int64_t count);

// template<typename T>
// class NetworkLayer
// {
// public:
//   NetworkLayer(const int64_t mySize, const int64_t nextLayerSize);
//   atMatrixNxM<T> m_biases;
//   atMatrixNxM<T> m_weights;
//   atMatrixNxM<T> m_activations;
// 
//   
// 
//   template <typename T2> atMatrixNxM<T2> CalculateActivations()
//   {
//     atMatrixNxM ret = m_weights * m_activations + m_biases;
//     return ret;
//   }
// };
// 
// template <typename InType, typename InType2, typename OutType> atMatrixNxM<OutType> atBPGNetworkCalculateActivations(const NetworkLayer<InType> &layer)
// {
//   atMatrixNxM<OutType> ret;
// 
//   ret.
// }
// 
// template<typename InType, typename OutType, typename LayerType = InType>
// class atBPGNetwork
// {
// public:
//   atBPGNetwork(int64_t inputSize, int64_t outputSize, int64_t layerCount, int64_t layerSize = 16);
// 
//   atVector<OutType> Run(const atVector<InType> &inputs);
// 
//   bool Train(const atVector<InType> &inputs, const atVector<OutType> &outputs);
// 
// protected:
// 
//   NetworkLayer<InType> m_input;
//   NetworkLayer<OutType> m_output;
//   atVector<NetworkLayer<LayerType>> m_layers;
// };

// #include "atBPGNetwork.inl"
#endif // atBPGNetwork_h__
