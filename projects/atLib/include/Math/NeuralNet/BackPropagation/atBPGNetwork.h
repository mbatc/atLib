
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

  // Train on a specific input/output
  bool Train(const atVector<double> &input, const atVector<double> &output, double rate = 0.5);

  // Train using batched data
  bool TrainBatch(const atVector<atVector<double>> &inputs, const atVector<atVector<double>> &outputs, double rate = 0.5);

  // To modify a layers weights/bias use the Get functions to retrieve the current
  // values, modify the returned matrices and then use the Set functions to set
  // the networks values.

  // The Set functions will fail if the matrix passed is the incorrect size.

  const atMatrixNxM<double>& GetLayerWeights(int64_t layer) const;
  const atMatrixNxM<double>& GetLayerBiases(int64_t layer) const;

  bool SetLayerWeights(int64_t layer, atMatrixNxM<double> weights);
  bool SetLayerBiases(int64_t layer, atMatrixNxM<double> biases);

  int64_t LayerCount() const;
  int64_t InputCount() const;
  int64_t OutputCount() const;

  static int64_t StreamWrite(atWriteStream *pStream, const atBPGNetwork *pData, const int64_t count);
  static int64_t StreamRead(atReadStream *pStream, atBPGNetwork *pData, const int64_t count);

protected:
  void CalculateWeights(int64_t layer, const atVector<atMatrixNxM<double>> &a, const atVector<atMatrixNxM<double>> &z, atVector<atMatrixNxM<double>> *pWeights, double carriedError);

  int64_t m_nInputs;
  int64_t m_nOutputs;
  atVector<Layer> m_layers;
};

int64_t atStreamRead(atReadStream *pStream, atBPGNetwork::Layer *pData, const int64_t count);
int64_t atStreamWrite(atWriteStream *pStream, const atBPGNetwork::Layer *pData, const int64_t count);
int64_t atStreamRead(atReadStream *pStream, atBPGNetwork *pData, const int64_t count);
int64_t atStreamWrite(atWriteStream *pStream, const atBPGNetwork *pData, const int64_t count);

#endif // atBPGNetwork_h__
