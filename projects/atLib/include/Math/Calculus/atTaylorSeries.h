#ifndef atTaylorSeries_h__
#define atTaylorSeries_h__

#include "atVector.h"
#include <functional>

class atTaylorSeries
{
public:
  atTaylorSeries(const std::function<double(double)> &func, const double &xPos = 0, const int64_t &depth = 3);

  void SetApproximationDepth(const int64_t &depth);
  void SetApproximationStart(const double &x);
  void SetFunction(const std::function<double(double)> &func);

  double Get(const double &x) const;

protected:
  void CalculateTerms(const int64_t &depth);

  atVector<double> m_coefficients;
  double m_approxStart = 0;
  double m_derivativeStep = 0.0001;
  std::function<double(double)> m_func;
};

#endif // atTaylorSeries_h__
