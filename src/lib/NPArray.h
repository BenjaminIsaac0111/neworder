#pragma once

#include "NewOrder.h"
#include "numpy.h"
// "vectorised" functions operating on, and/or returning numpy arrays 
// for now, only vectorised in the sense the code is compiled (and the compiler is free to choose SIMD if it sees fit)
// TODO if bottleneck help compiler's SIMD vectorisation using ideally openmp directives

namespace no { namespace nparray {

np::array isnever(const np::array& x);

}} //no::nparray