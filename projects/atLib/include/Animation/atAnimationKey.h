#ifndef atAnimationKey_h__
#define atAnimationKey_h__

class atAnimationKey
{
public:
  enum Interpolation
  {
    Linear,
    Constant,
    Cubic,
    Count
  };

  // Get the keys value
  double GetValue() const;

  // Set the keys value
  void SetValue(const double &val);

  // Get the keys interpolation type
  Interpolation GetInterpolation() const;

  // Set the keys interpolation type
  void SetInterpolation(const Interpolation &type);

  // Interpolate between two keys
  double Interpolate(const atAnimationKey &key, const double &time) const;

protected:
  double m_value = 0;
  Interpolation m_interp = Linear;
};

#endif // atAnimationKey_h__
