
// -----------------------------------------------------------------------------
// The MIT License
// 
// Copyright(c) 2020 Michael Batchelor, 
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

    // Randomize values to begin with
    for (double &val : m_layers[i - 1].weights.m_data)
      val = (double)(rand() % 500) / 500;
    for (double &val : m_layers[i - 1].biases.m_data)
      val = (double)(rand() % 500) / 500;
  }

  m_nOutputs = outputSize;
  m_nInputs = inputSize;
}

atVector<double> atBPGNetwork::Predict(const atVector<double> &input)
{
  atMatrixNxM<double> activations(1, input.size());
  memcpy(activations.m_data.data(), input.data(), input.size() * sizeof(double));
  for (int64_t i = 0; i < m_layers.size(); ++i)
    activations = (m_layers[i].weights * activations + m_layers[i].biases).Apply(atSigmoidApprox);
  return activations.m_data;
}

void atBPGNetwork::CalculateWeights(int64_t layer, const atVector<atMatrixNxM<double>> &a, const atVector<atMatrixNxM<double>> &z, atVector<atMatrixNxM<double>> *pAdjWeight, double carriedError)
{
  if (layer < 1 || layer >= m_layers.size())
    return;
  int64_t prevLayer = layer - 1;
  for (int64_t j = 0; j < m_layers[layer].nNodes; ++j)
  {
    double dSig = atDerivative<double, double>(z[layer][j], atSigmoidApprox<double>);
    double prevA = 0;
    double err = carriedError * dSig;
    int64_t rowStart = j * m_layers[prevLayer].nNodes;
    for (int64_t k = 0; k < m_layers[prevLayer].nNodes; ++k)
    {
      int64_t matIndex = k + rowStart;
      (*pAdjWeight)[prevLayer][matIndex] += a[prevLayer][k] * err;
      CalculateWeights(layer - 1, a, z, pAdjWeight, err * m_layers[prevLayer].weights[matIndex]);
    }
  }
}

bool atBPGNetwork::Train(const atVector<double> &input, const atVector<double> &output)
{
  if (input.size() != InputCount() || output.size() != OutputCount())
    return false;

  double avgCost = 0;
  atVector<atMatrixNxM<double>> adjBias;
  atVector<atMatrixNxM<double>> adjWeight;

  atVector<atMatrixNxM<double>> activationRaw;
  atVector<atMatrixNxM<double>> activations;

  activations.push_back(atMatrixNxM<double>(1, input.size()));
  memcpy(activations.back().m_data.data(), input.data(), input.size() * sizeof(double));
  activationRaw.push_back(activations.back());
  for (int64_t i = 0; i < m_layers.size(); ++i)
  {
    activationRaw.push_back(m_layers[i].weights * activations.back() + m_layers[i].biases);
    activations.push_back(activationRaw.back().Apply(atSigmoidApprox));
  }

  for (Layer &l : m_layers)
  {
    adjWeight.push_back(atMatrixNxM<double>(l.weights.m_columns, l.weights.m_rows));
    adjBias.push_back(atMatrixNxM<double>(l.biases.m_columns, l.biases.m_rows));
  }

  for (int64_t j = 0; j < activations[activations.size() - 1].m_rows; ++j)
    for (int64_t k = 0; k < activations[activations.size() - 2].m_rows; ++k)
    {
      atMatrixNxM<double> &act = activations[activations.size() - 1];
      atMatrixNxM<double> &dB = adjBias[activations.size() - 2];
      atMatrixNxM<double> &dW = adjWeight[activations.size() - 2];

      double err = act[j] - output[j];
      avgCost += 0.5 * atSquare(err);
      dW[k + j * dW.m_columns] = atDerivative<double, double>(activationRaw[activations.size() - 1][j], atSigmoidApprox<double>) *
        activations[activations.size() - 2][k] * err;

      CalculateWeights(activations.size() - 2, activations, activationRaw, &adjWeight, err);
    }

  for (int64_t lIndex = 0; lIndex < m_layers.size(); ++lIndex)
    m_layers[lIndex].weights = m_layers[lIndex].weights - adjWeight[lIndex].Mul(m_trainingRate);

  return true;
}

const atMatrixNxM<double>& atBPGNetwork::GetLayerWeights(int64_t layer) const
{
  return m_layers[layer].weights;
}

const atMatrixNxM<double>& atBPGNetwork::GetLayerBiases(int64_t layer) const
{
  return m_layers[layer].biases;
}

int64_t atBPGNetwork::LayerCount() const { return m_layers.size(); }
int64_t atBPGNetwork::InputCount() const { return m_nInputs; }
int64_t atBPGNetwork::OutputCount() const { return m_nOutputs; }

void atBPGNetwork::SetTrainingRate(const double &rate) { m_trainingRate = rate; }
const double &atBPGNetwork::GetTrainingRate() const { return m_trainingRate;  }

bool atBPGNetwork::SetLayerWeights(int64_t layer, const atMatrixNxM<double> &weights)
{
  atMatrixNxM<double> &mat = m_layers[layer].weights;
  if (mat.m_columns != weights.m_columns || mat.m_rows != weights.m_rows)
    return false;
  mat = weights;
  return true;
}

bool atBPGNetwork::SetLayerBiases(int64_t layer, const atMatrixNxM<double> &biases)
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

void atSerialize(atObjectDescriptor *pSerialized, const atBPGNetwork &src)
{
  pSerialized->Add("learnRate").Serialize(src.m_trainingRate);
  pSerialized->Add("nOutputs").Serialize(src.m_nOutputs);
  pSerialized->Add("layers").Serialize(src.m_layers);
}

void atDeserialize(const atObjectDescriptor &serialized, atBPGNetwork *pDst)
{
  serialized["learnRate"].Deserialize(&pDst->m_trainingRate);
  serialized["nOutputs"].Deserialize(&pDst->m_nOutputs);
  serialized["layers"].Deserialize(&pDst->m_layers);

  if (pDst->m_layers.size() > 0)
  {
    pDst->m_nInputs = pDst->m_layers.front().nNodes;
    pDst->m_nOutputs = pDst->m_layers.back().nNodes;
  }
}

void atSerialize(atObjectDescriptor *pSerialized, const atBPGNetwork::Layer &src)
{
  pSerialized->Add("nNodes").Serialize(src.nNodes);
  pSerialized->Add("bias").Serialize(src.biases.Transpose());
  pSerialized->Add("weight").Serialize(src.weights);
}

void atDeserialize(const atObjectDescriptor &serialized, atBPGNetwork::Layer *pDst)
{
  serialized.Get("nNodes").Deserialize(&pDst->nNodes);
  serialized.Get("bias").Deserialize(&pDst->biases);
  serialized.Get("weight").Deserialize(&pDst->weights);
  pDst->biases = pDst->biases.Transpose();
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
