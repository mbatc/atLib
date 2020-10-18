
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
  m_layerSize = layerSize;
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
      val = atClamp((double)(rand() % 500) / 500, 0.1, 1.0);
    for (double &val : m_layers[i - 1].biases.m_data)
      val = double(rand() % 1000ll - 500) / 500;
  }

  m_nOutputs = outputSize;
  m_nInputs = inputSize;
}

atVector<double> atBPGNetwork::Predict(const atVector<double> &input) { return Predict(input, nullptr, nullptr).m_data; }

bool atBPGNetwork::Train(const atVector<atVector<double>> &inputs, const atVector<atVector<double>> &outputs)
{
  atMatrixNxM<double> costGradient; // Cost gradient matrix
  atVector<atMatrixNxM<double>> biasAdjustments;
  atVector<atMatrixNxM<double>> weightAdjustments;
  // For each training set
  for (int64_t i = 0; i < inputs.size(); ++i)
  {
    const atVector<double> &set = inputs[i];   // Input set
    const atVector<double> &goal = outputs[i]; // Target values

    // Get the networks current prediction for the input set
    // We also need to store the activates at each layer to perform back propagation
    atVector<atMatrixNxM<double>> activations;
    atVector<atMatrixNxM<double>> rawActivations;
    Predict(set, &activations, &rawActivations);
    weightAdjustments.resize(activations.size() - 1);
    biasAdjustments.resize(activations.size() - 1);

    atMatrixNxM<double> prevLayerRawActivationCostInfluence;
    for (int64_t currentLayer = activations.size() - 1; currentLayer > 0; --currentLayer)
    {
      atMatrixNxM<double> &currentLayerRawActivations = rawActivations[currentLayer];
      atMatrixNxM<double> &prevLayerRawActivations = rawActivations[currentLayer - 1];
      atMatrixNxM<double> &currentLayerActivations = activations[currentLayer];
      atMatrixNxM<double> &prevLayerActivations = activations[currentLayer - 1];

      // Raw activation influences on the cost function for the previous layer
      atMatrixNxM<double> currentLayerRawWeightCostInfluence = prevLayerRawActivationCostInfluence;
      prevLayerRawActivationCostInfluence = atMatrixNxM<double>(1, prevLayerActivations.m_rows, 0);

      // Weight influences on the cost function for this layer
      atMatrixNxM<double> layerWeightCostInfluence(currentLayerActivations.m_rows, prevLayerActivations.m_rows);
      atMatrixNxM<double> layerLayerBiasCostInfluence(1, prevLayerRawActivations.m_rows);

      for (int64_t currentLayerNode = 0; currentLayerNode < currentLayerActivations.m_rows; ++currentLayerNode)
      {
        // double a = currentLayerActivations[currentLayerNode]; // Current layer activation value
        for (int64_t prevLayerNode = 0; prevLayerNode < prevLayerActivations.m_rows; ++prevLayerNode)
        {
          // double aPrev = prevLayerActivations[prevLayerNode]; // Previous layer activation value

          // Compute derivative of the current layers activation with respect to the rawActivation
          double sigmoidActivationInfluence = 1;
          if (m_activationFunc)
            sigmoidActivationInfluence = atDerivative<double, double>(currentLayerRawActivations[currentLayerNode], m_activationFunc, 0.01);

          if (currentLayerRawWeightCostInfluence.m_rows == 0) // The current layer is the output layer
          {
            // Compute derivative of the Cost function with respect to the current layers activation
            double activationCostInfluence = 2 * (currentLayerActivations[currentLayerNode] - goal[currentLayerNode]);

            sigmoidActivationInfluence *= activationCostInfluence;
          }
          else
          {
            sigmoidActivationInfluence *= m_layers[currentLayer - 1].weights(prevLayerNode, currentLayerNode) * currentLayerRawWeightCostInfluence(currentLayerNode, 0);
          }

          // Compute derivative of the current layers activation with respect to the current layers weight
          double activationWeightInfluence = prevLayerActivations[prevLayerNode];

          // Compute derivative of the Cost function with respect to the previous layers raw activation
          double prevRawWeightCostInfluence = sigmoidActivationInfluence;

          // Compute derivative of the Cost function with respect to the weight
          double weightCostInfluence = prevRawWeightCostInfluence * activationWeightInfluence;

          // Add the weight influence to the weight cost influence
          layerWeightCostInfluence(prevLayerNode, currentLayerNode) += weightCostInfluence;

          layerLayerBiasCostInfluence(prevLayerNode, 0) += prevRawWeightCostInfluence;

          // Add the previous layers cost influence contributed by each node in this layer
          prevLayerRawActivationCostInfluence(prevLayerNode, 0) += prevRawWeightCostInfluence;
        }
      }

      if (weightAdjustments[currentLayer - 1].m_rows != layerWeightCostInfluence.m_rows)
        weightAdjustments[currentLayer - 1] = layerWeightCostInfluence;
      else
        weightAdjustments[currentLayer - 1] = weightAdjustments[currentLayer - 1] + layerWeightCostInfluence;

      if (biasAdjustments[currentLayer - 1].m_rows == 0)
        biasAdjustments[currentLayer - 1] = prevLayerRawActivationCostInfluence;
      else
        biasAdjustments[currentLayer - 1] = biasAdjustments[currentLayer - 1] + prevLayerRawActivationCostInfluence;
    }
  }

  // Adjust the training rate so that it depends on the number of nodes in the network
  double trainingAmount = m_trainingRate / (outputs.size() * m_nOutputs);
  for (int64_t i = 0; i < m_layers.size(); ++i)
    m_layers[i].weights = m_layers[i].weights - weightAdjustments[i].Mul(trainingAmount).Apply([](double val) { return atClamp(val, -1.0, 1.0); });
  for (int64_t i = 0; i < m_layers.size(); ++i)
    m_layers[i].biases = m_layers[i].biases - biasAdjustments[i].Mul(trainingAmount);

  return true;
}

void atBPGNetwork::SetActivationFunction(std::function<double(double)> func) { m_activationFunc = func; }

const atMatrixNxM<double>& atBPGNetwork::GetLayerWeights(int64_t layer) const { return m_layers[layer].weights; }
const atMatrixNxM<double>& atBPGNetwork::GetLayerBiases(int64_t layer) const { return m_layers[layer].biases; }

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

atMatrixNxM<double> atBPGNetwork::Predict(const atVector<double> &input, atVector<atMatrixNxM<double>> *pActivations, atVector<atMatrixNxM<double>> *pRawActivations)
{
  atMatrixNxM<double> activations(1, input.size());
  memcpy(activations.m_data.data(), input.data(), input.size() * sizeof(double));

  if (pRawActivations)
    pRawActivations->push_back(activations);
  if (pActivations)
    pActivations->push_back(activations);

  for (int64_t i = 0; i < m_layers.size(); ++i)
  {
    activations = (m_layers[i].weights * activations + m_layers[i].biases);
    if (pRawActivations)
      pRawActivations->push_back(activations);
    if (m_activationFunc)
      activations = activations.Apply(m_activationFunc);
    if (pActivations)
      pActivations->push_back(activations);
  }
  return activations;
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

int64_t atStreamRead(atReadStream *pStream, atBPGNetwork *pData, const int64_t count) { return atBPGNetwork::StreamRead(pStream, pData, count); }
int64_t atStreamWrite(atWriteStream *pStream, const atBPGNetwork *pData, const int64_t count) { return atBPGNetwork::StreamWrite(pStream, pData, count); }
