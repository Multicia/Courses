import types
import scipy

print [getattr(scipy, a, None) for a in dir(scipy)
  if isinstance(scipy.__dict__.get(a), types.FunctionType)]
