

#include "test.h"

#include "Environment.h"
#include "Module.h"

#include "NewOrder.h"


void test_errors()
{
  py::object module = py::module::import("op");
  py::object function(module.attr("notafunc"));
  CHECK_THROWS(function(), py::error_already_set);
}