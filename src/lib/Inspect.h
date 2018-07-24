#pragma once

#include "PythonFwd.h"
#include "python.h"

#include <vector>
#include <string>

namespace pycpp {

const char* type(PyObject* p);

const char* type(const py::object& o);

bool callable(const py::object& o);

bool has_attr(const py::object& o, const char* attr_name);

std::vector<std::pair<std::string, const char*>> dir(PyObject* obj, bool public_only=true);

std::vector<std::pair<std::string, const char*>> dir(const py::object& obj, bool public_only=true);

}