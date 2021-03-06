""" Test utils """

import inspect
import neworder


class Test:
  def __init__(self):
    self.any_failed = False

  def check(self, expr):
    if not expr:
      trace = inspect.stack()[1]
      neworder.log("FAIL %s at %s:%d" % (trace.code_context[0].strip("\n"), trace.filename, trace.lineno)) #["code_context"])
      self.any_failed = True

