"""
diagnostics.py

Prints diagnostic info
"""

import os
import sys
import subprocess
import neworder


neworder.log("MODULE=" + neworder.name() + neworder.version())
neworder.log("PYTHON=" + neworder.python())

all_libs = subprocess.getoutput("ldd src/bin/neworder").replace("\t", "").split("\n")
neworder.log("Loaded neworder/boost/python libs:")
[neworder.log("  " + s) for _,s in enumerate(all_libs) if "neworder" in s or "python" in s or "boost" in s]

neworder.log("PYTHONPATH=" + os.environ["PYTHONPATH"])

# TODO MPI?

do_checks = False
neworder.timespan = neworder.DVector.fromlist([0,1])
neworder.timestep = neworder.timespan[1]

initialisations = {}
transitions = {}
checkpoints = {}

# test callback
neworder.log("2 + 2 = %d" % neworder.Callback("2+2")())

