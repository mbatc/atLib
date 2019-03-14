#include "atBPGNetwork.h"
#include "atMath.h"

atBPGNetwork::atBPGNetwork(int64_t inputSize, int64_t outputSize, int64_t layerCount, int64_t layerSize)
{
  m_layers.resize(1 + atMax(0, layerCount));
  m_layers[0].nNodes = inputSize;
  for (int64_t i = 1; i < m_layers.size() + 1; ++i)
  {
    int64_t nextLayerNodeCount = outputSize;
    if (i < m_layers.size())
    {
      m_layers[i].nNodes = layerSize;
      nextLayerNodeCount = layerSize;
    }

    m_layers[i - 1].weights = atMatrixNxM<double>(m_layers[i - 1].nNodes, nextLayerNodeCount);
    m_layers[i - 1].biases = atMatrixNxM<double>(1, nextLayerNodeCount);
  }
}

atVector<double> atBPGNetwork::Run(const atVector<double> &input) const
{
  atMatrixNxM<double> activations(1, input.size());
  memcpy(activations.m_data.data(), input.data(), input.size() * sizeof(double));
  for (int64_t i = 0; i < m_layers.size(); ++i)
    activations = m_layers[i].weights * activations + m_layers[i].biases;
  return activations.m_data;
}

void atBPGNetwork::Train(const atVector<double> &input, const atVector<double> &output)
{
  atVector<double> actualOut = Run(input);
  atUnused(actualOut);
}

const atMatrixNxM<double>& atBPGNetwork::GetLayerWeights(int64_t layer) const
{
  return m_layers[layer].weights;
}

const atMatrixNxM<double>& atBPGNetwork::GetLayerBiases(int64_t layer) const
{
  return m_layers[layer].biases;
}

int64_t atBPGNetwork::LayerCount() const
{
  return m_layers.size();
}

bool atBPGNetwork::SetLayerWeights(int64_t layer, atMatrixNxM<double> weights)
{
  atMatrixNxM<double> &mat = m_layers[layer].weights;
  if (mat.m_columns != weights.m_columns || mat.m_rows != weights.m_rows)
    return false;
  mat = weights;
  return true;
}

bool atBPGNetwork::SetLayerBiases(int64_t layer, atMatrixNxM<double> biases)
{
  atMatrixNxM<double> &mat = m_layers[layer].biases;
  if (mat.m_columns != biases.m_columns || mat.m_rows != biases.m_rows)
    return false;
  mat = biases;
  return true;
}

atBPGNetwork::Layer::Layer()
  : weights(0, 0)
  , biases(0, 0)
{}

int64_t atBPGNetwork::StreamWrite(atWriteStream *pStream, const atBPGNetwork *pData, const int64_t count)
{
  int64_t ret = 0;
  for (int64_t i = 0; i < count; ++i)
    ret += atStreamWrite(pStream, &pData[i].m_layers, 1);
  return ret;
}

int64_t atBPGNetwork::StreamRead(atReadStream *pStream, atBPGNetwork *pData, const int64_t count)
{
  int64_t ret = 0;
  for (int64_t i = 0; i < count; ++i)
    ret += atStreamRead(pStream, &pData[i].m_layers, 1);
  return ret;
}

int64_t atStreamRead(atReadStream *pStream, atBPGNetwork::Layer *pData, const int64_t count)
{
  int64_t ret = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    ret += atStreamRead(pStream, &pData[i].weights, 1);
    ret += atStreamRead(pStream, &pData[i].biases, 1);
  }
  return ret;
}

int64_t atStreamWrite(atWriteStream *pStream, const atBPGNetwork::Layer *pData, const int64_t count)
{
  int64_t ret = 0;
  for (int64_t i = 0; i < count; ++i)
  {
    ret += atStreamWrite(pStream, &pData[i].weights, 1);
    ret += atStreamWrite(pStream, &pData[i].biases, 1);
  }
  return ret;
}

int64_t atStreamRead(atReadStream *pStream, atBPGNetwork *pData, const int64_t count)
{
  return atBPGNetwork::StreamRead(pStream, pData, count);
}

int64_t atStreamWrite(atWriteStream *pStream, const atBPGNetwork *pData, const int64_t count)
{
  return atBPGNetwork::StreamWrite(pStream, pData, count);
}
