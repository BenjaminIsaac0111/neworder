#include "Object.h"
#include "Environment.h"
#include "Function.h"
#include "Module.h"

#include <Python.h>

#include <iostream>

// C++-ified version of the example here: https://docs.python.org/3/extending/embedding.html

// perhaps pycpp::String?
std::vector<std::string> dir(PyObject* obj)
{
  auto attrs = pycpp::List(PyObject_Dir(obj)).toVector<std::string>();
  return attrs;
}


int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << "pythonfile funcname [args...]" << std::endl;
    return 1;
  }

  std::cout << "[C++] " << argv[1] << ":" << argv[2];
  for (int i = 3; i < argc; ++i)
    std::cout << " " << argv[i];
  std::cout << std::endl;

  try
  {
    pycpp::Environment env;

    pycpp::String filename(PyUnicode_DecodeFSDefault(argv[1]));

    pycpp::Module module = pycpp::Module::init(filename);

    pycpp::Function function(module.getAttr(argv[2]));

    for (const auto& attr: dir(module.release())) 
    {
      std::cout << "[C++] ::" << attr << std::endl;
    }

    bool has_person = module.hasAttr("Person");
    std::cout << "[C++] Person? " << has_person << std::endl;

    if (has_person)
    {
      for (const auto& attr: dir(module.getAttr("Person"))) 
      {
        std::cout << "[C++] Person::" << attr << std::endl;
      }
    }

    pycpp::Tuple args(argc - 3);
    for (int i = 0; i < argc - 3; ++i)
    {
      args.set(i, pycpp::Int(std::stoi(argv[i + 3])));
    }
    pycpp::Int result(function.call(args));
    std::cout << "[C++] Result: " << (int)result << std::endl;

  }
  catch (std::exception &e)
  {
    std::cerr << "ERROR:" << e.what() << std::endl;
    return 1;
  }
  return 0;
}