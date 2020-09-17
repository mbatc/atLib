#include "atPredictiveModel.h"

atMatrixNxM<double> atPredictiveModel::PredictBatch(const atMatrixNxM<double> &input)
{
  // Convert to vectors
  atVector<atVector<double>> inVec;
  inVec.resize(input.m_rows);
  for (int64_t r = 0; r < input.m_rows; ++r)
    inVec[r].push_back(atVector<double>(&input.at(r, 0), input.m_columns));

  // Make predictions
  atVector<atVector<double>> outVec = PredictBatch(inVec);
  if (outVec.size() == 0)
    return atMatrixNxM<double>();

  // Convert back to a matrix
  atMatrixNxM<double> outMat(outVec.front().size(), input.m_rows);
  for (int64_t r = 0; r < input.m_rows; ++r)
    for (int64_t c = 0; c < outVec[r].size(); ++c)
      outMat.at(r, c) = outVec[r][c];
  return outMat;
}

atVector<atVector<double>> atPredictiveModel::PredictBatch(const atVector<atVector<double>> &input)
{
  atVector<atVector<double>> output;
  int64_t cols = -1;
  for (const atVector<double> &in : input)
  {
    output.push_back(Predict(in));
    if (cols < 0)
      cols = output.back().size();
    if (output.back().size() != cols)
      return atVector<atVector<double>>();
  }
  return output;
}

bool atPredictiveModel::TrainBatch(const atMatrixNxM<double> &input, const atMatrixNxM<double> &output)
{
  if (input.m_rows != output.m_rows)
    return false;

  atVector<atVector<double>> inVec;
  atVector<atVector<double>> outVec;
  inVec.resize(input.m_rows);
  outVec.resize(output.m_rows);
  for (int64_t r = 0; r < input.m_rows; ++r)
  {
    inVec[r].push_back(atVector<double>(&input.at(r, 0), input.m_columns));
    outVec[r].push_back(atVector<double>(&output.at(r, 0), output.m_columns));
  }

  return TrainBatch(inVec, outVec);
}

bool atPredictiveModel::TrainBatch(const atVector<atVector<double>> &input, const atVector<atVector<double>> &output)
{
  if (input.size() != output.size())
    return false;

  bool success = true;
  for (int64_t i = 0; i < input.size(); ++i)
    success &= Train(input[i], output[i]);
  return success;
}
