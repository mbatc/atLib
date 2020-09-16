#ifndef atPredictiveModel_h__
#define atPredictiveModel_h__


// Predictive Model interface

class atPredictiveModel
{
public:
  atPredictiveModel(int64_t nInputs, int64_t nOutputs);

  // Predict using a matrix of inputs and receive a matrix back
  // Each row is treated as an individual set of data
  virtual atMatrixNxM<double> Predict(const atMatrixNxM<double> &input);
  virtual atVector<atVector<double>> Predict(const atVector<atVector<double>> &input);
  virtual atVector<double> Predict(const atVector<double> &input);

  virtual bool Train(const atVector<double> &input, const atVector<double> &);
};

#endif // atPredictiveModel_h__
