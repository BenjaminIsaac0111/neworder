#pragma once

#include "python.h"

#include <vector>
#include <map>

namespace neworder {

// Define a piece of python code to be exec/eval-u(a)ted on calling operator()
// Perhaps better named LazyEval?
class Callback
{
public:
  // Construct using one of these two variants 
  static Callback exec(const std::string& code);
  static Callback eval(const std::string& code);
  
  ~Callback() = default;

  py::object operator()() const;

  bool is_exec() const { return m_exec; }

  const std::string code() const
  {
    return (m_exec ? "exec(\"" : "eval(\"") + m_code + "\")";
  }

private:
  // construct using one of the static functions
  Callback(const std::string& code, bool exec);

  bool m_exec;
  std::string m_code;
  py::object m_globals;
  py::object m_locals;
};

typedef std::map<std::string, Callback> CallbackTable;

const char* module_name();

const char* module_version();

std::string python_version();

// interactive shell mk2 - uses the code module
void shell(/*const py::object& local*/);

// TODO these are not visible to python and don't really belong here
// msg is forcibly coerced to a string
void log(const py::object& msg);
void log(const std::string& msg);

void import_module();

} // namespace neworder


