#pragma once

#include <vector>
#include <string>
#include <algorithm>

namespace pycpp {

// TODO perhaps pycpp::String?
// TODO move impl to cpp
// TODO rename?

inline std::vector<std::string> dir(PyObject* obj, bool public_only=true)
{
  std::vector<std::string> attrs = pycpp::List(PyObject_Dir(obj)).toVector<std::string>();
  // Ignore anything with a leading underscore - assuming private in as much as private exists in python
  if (public_only)
    attrs.erase(std::remove_if(attrs.begin(), attrs.end(), [](const std::string& s) { return s[0] == '_';}), attrs.end());

  return attrs;
}

inline const char* type(PyObject* p)
{
  return Py_TYPE(p)->tp_name;
}

}