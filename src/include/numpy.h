
#pragma once

#include "python.h"

#include "pybind11/numpy.h"

// create a np namespace
namespace np {
  using py::array;

  template<typename T>
  array empty(const std::initializer_list<size_t>& shape)
  {
    return py::array_t<T>(shape);
  }

  template<typename T>
  array zeros(const std::initializer_list<size_t>& shape)
  {
    return py::array_t<T>(shape, T(0));
  }

  // TODO what is required here (if anything)
  inline void initialize()
  {
  }
}

// helper functions for ndarrays
namespace pycpp {

inline size_t size(const np::array& a)
{
  // size_t dim = a.ndim();
  // // assumes dim >=1 
  // size_t s = a.shape(0);
  // for (size_t i = 1; i < dim; ++i)
  //   s *= a.shape(i);
  // return s;
  return a.size();
}

template<typename T>
T& at(np::array& a, size_t index)
{
  // Flattened indexing. TODO reuse Index from humanleague if necess
  // if (a.get_nd() != 1)
  //   throw std::runtime_error("np::array dim>1");
  return *(reinterpret_cast<T*>(a.mutable_data((int)index)));
}

template<typename T>
T* begin(np::array& a)
{
  return reinterpret_cast<T*>(a.mutable_data(0));
}

template<typename T>
const T* cbegin(const np::array& a)
{
  return reinterpret_cast<const T*>(a.data(0));
}

template<typename T>
T* end(const np::array& a)
{
  return begin<T>(a) + size(a);
}

// Uninitialised 1d array
template<typename T>
np::array empty_1d_array(size_t n)
{
  return np::empty<T>({n});
}

// Zero-initialised 1d array
template<typename T>
np::array zero_1d_array(size_t n)
{
  return np::zeros<T>({n});
}

// Create a 1d array, initialising with a function
// e.g. "ones" is make_array<double>(n, [](){ return 1.0; })
template<typename T>
np::array make_array(size_t n, const std::function<T()>& f)
{
  np::array a = pycpp::empty_1d_array<T>(n); 
  T* p = reinterpret_cast<T*>(a.data());
  std::generate(p, p + n, f);
  return a;
}

template<typename T>
np::array& fill(np::array& a, T val)
{
  T* p = reinterpret_cast<T*>(a.data());
  size_t n = pycpp::size(a);
  std::fill(p, p + n, val);
  return a;
}

// template<typename R, typename A>
// struct UnaryArrayOp
// {
//   typedef A argument_type;
//   typedef R result_type;

//   virtual ~UnaryArrayOp() { }

//   virtual R operator()(A) = 0;

//   // implementing the above function in a derived class hides the (below) base-class implementations of operator() 
//   // see https://stackoverflow.com/questions/1628768/why-does-an-overridden-function-in-the-derived-class-hide-other-overloads-of-the/1629074#1629074
//   // use a using declaration in the derived class to force it to be visible
//   np::array operator()(const py::object& arg) 
//   {
//     return np::array(np::unary_ufunc<UnaryArrayOp<R,A>>::call(*this, arg, py::object()));      
//   }
// };

// template<typename R, typename A1, typename A2>
// struct BinaryArrayOp
// {
//   typedef A1 first_argument_type;
//   typedef A2 second_argument_type;
//   typedef R result_type;

//   virtual ~BinaryArrayOp() { }

//   virtual R operator()(A1, A2) = 0;

//   // implementing the above function in a derived class hides the (below) base-class implementations of operator() 
//   // see https://stackoverflow.com/questions/1628768/why-does-an-overridden-function-in-the-derived-class-hide-other-overloads-of-the/1629074#1629074
//   // use a using declaration in the derived class to force it to be visible
//   np::array operator()(const py::object& arg1, const py::object& arg2) 
//   {
//     return np::array(np::binary_ufunc<BinaryArrayOp<R, A1, A2>>::call(*this, arg1, arg2, py::object()));      
//   }
// };

}