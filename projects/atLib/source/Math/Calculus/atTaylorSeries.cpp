#include "atTaylorSeries.h"
#include "atMath.h"

atTaylorSeries::atTaylorSeries(const std::function<double(double)> &func, const double &xPos, const int64_t &depth)
  : m_approxStart(xPos)
  , m_func(func)
{
  CalculateTerms(depth);
}

void atTaylorSeries::SetApproximationDepth(const int64_t &depth)
{
  CalculateTerms(depth);
}

void atTaylorSeries::SetApproximationStart(const double &x)
{
  m_approxStart = x;
  CalculateTerms(int64_t(m_coefficients.size() - 1));
}

void atTaylorSeries::SetFunction(const std::function<double(double)> &func)
{
  m_func = func;
  CalculateTerms(int64_t(m_coefficients.size() - 1));
}

double atTaylorSeries::Get(const double &x) const
{
  if (m_coefficients.size() == 0)
    return 0; 

  double value = m_coefficients[0];
  for (int64_t n = 1; n < m_coefficients.size(); ++n)
    value += m_coefficients[n] * pow(x - m_approxStart, (double)n);
  return value;
}

void atTaylorSeries::CalculateTerms(const int64_t &depth)
{
  if (depth <= 0 || !m_func)
    return;

  m_coefficients.clear();
  m_coefficients.push_back(m_func(m_approxStart));
  for (int64_t n = 0; n < depth; ++n)
    m_coefficients.push_back(atNthDerivative(m_approxStart, m_func, n, m_derivativeStep) / atFactorial(n));
}
