#ifndef atPredictiveModel_h__
#define atPredictiveModel_h__

#include "atMatrixNxM.h"

// Predictive Model interface

class atPredictiveModel
{
public:
  // Predict using a matrix of inputs and receive a matrix back
  // Each row is treated as an individual set of data
  virtual atVector<double> Predict(const atVector<double> &input) = 0;
  virtual bool Train(const atVector<atVector<double>> &input, const atVector<atVector<double>> &output) = 0;

  virtual int64_t InputCount() const = 0;
  virtual int64_t OutputCount() const = 0;

  atMatrixNxM<double> PredictBatch(const atMatrixNxM<double> &input);
  atVector<atVector<double>> PredictBatch(const atVector<atVector<double>> &input);
  bool TrainBatch(const atMatrixNxM<double> &input, const atMatrixNxM<double> &output);
};

#endif // atPredictiveModel_h__
