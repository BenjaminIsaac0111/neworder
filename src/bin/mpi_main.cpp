// MPI entry point

#include "run.h"
#include "MPIResource.h"

#include <iostream>

int main(int argc, char** argv)
{
  // Directory containing model (config.py, etc) is specified on the command line
  // It's added to PYTHONPATH
  if (argc != 2)
  {
    std::cerr << "usage: neworder <model-path>\n"
              << "where <model-path> is a directory containing the model config (config.py) plus the model definition python files" << std::endl;
    exit(1);
  }
  append_model_path(argv[1]);

  MPIResource mpi(&argc, &argv);

  run(mpi.rank(), mpi.size());
}