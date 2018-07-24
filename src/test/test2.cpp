#include "Object.h"
#include "Environment.h"
#include "Function.h"
#include "Module.h"
#include "Inspect.h"

#include <Python.h>

#include <vector>
#include <string>
#include <iostream>


void test2(const std::string& modulename, const std::string& objectname, const std::vector<std::string>& methodnames)
{
  std::cout << "[C++] " << modulename << ":" << objectname << std::endl;
  py::object module = py::import(modulename.c_str());

  py::object o = module.attr(objectname.c_str());

  for (const auto& methodname: methodnames)
  {
    py::object method = o.attr(methodname.c_str());

    py::object res = method();
    std::cout << "[C++] " << objectname << "." << methodname << "(): " << res << std::endl;
  }

  // for (const auto& attr: pycpp::dir(module.release())) 
  // {
  //   std::cout << "[C++] ::" << attr << std::endl;
  //   for (const auto& sattr: pycpp::dir(module.getAttr(attr))) 
  //   {
  //     std::cout << "[C++] " << attr << "::" << sattr << std::endl;
  //   }
  // }

  // for (const auto& attr: pycpp::dir(function.release(), false)) 
  // {
  //   std::cout << "[C++] function::" << attr << std::endl;
  // }

  bool has_person = pycpp::has_attr(module, "Person");
  std::cout << "[C++] Person? " << has_person << std::endl;

  if (has_person)
  {
    for (const auto& attr: pycpp::dir(module.attr("Person"))) 
    {
      std::cout << "[C++] Person::" << attr.first << " [" << attr.second << "]" << std::endl;
    }
  }
}