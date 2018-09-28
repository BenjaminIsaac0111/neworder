#include "run.h"
#include "test.h"
#include "Environment.h"
#include "MPIResource.h"

#include "python.h"

#include <vector>
#include <string>
#include <iostream>

void test1(const std::string& modulename, const std::string& functionname, const std::vector<std::string>& args, const py::object& expected);
void test_no();
void test_env();
void test_np();
void test_mpi();
void test_errors();

void test_py(int nmodules, const char* testmodules[])
{
  for (int i = 0; i < nmodules; ++i)
  {
    py::object module = py::import(testmodules[i]);
    py::object testfunc = module.attr("test");
    neworder::log("running test %%.py"_s % testmodules[i]);
    CHECK(py::extract<bool>(testfunc())());
  }
}

int run(int rank, int size, bool indep, int nmodules, const char* testmodules[]) 
{
  pycpp::Environment& env = pycpp::Environment::init(rank, size, indep);
  try
  {
    // load module, call func with args
    test1("op", "mul", {"2", "3"}, py::object(6));
    test1("op", "void", {"2", "3"}, py::object());

    // module (C++) tests
    test_no();
    test_env();
    test_np(); // boost.Python.numpy
    test_mpi();
    test_errors();

    // python tests
    test_py(nmodules, testmodules);

    REPORT()
  }
  catch(py::error_already_set&)
  {
    std::cerr << "%%ERROR (py::error_already_set): %%"_s % env.context(pycpp::Environment::PY) % env.get_error() << std::endl;
    return 1;
  }
  catch(std::exception& e)
  {
    std::cerr << "%%ERROR (std::exception): %%"_s % env.context() % e.what() << std::endl;
    return 1;
  }
  catch(...)
  {
    std::cerr << "%% ERROR: unknown exception"_s % env.context() << std::endl;
    return 1;
  }
  RETURN();
}

