
import numpy as np
import pandas as pd

def sample(u, t, c):
  for i in range(len(t) - 1):
    if u >= t[i] and u < t[i+1]:
      return c[i]
  return c[len(t) - 1] 


def test():

  return True

def todo():

  # define some global variables describing where the starting population and the parameters of the dynamics come from
  initial_population = "examples/households/data/ssm_hh_E09000001_OA11_2011.csv"

  hh = pd.read_csv(initial_population)

  print(hh.columns.values)
  c = hh.LC4408_C_AHTHUK11.unique()
  print(c)
  t = np.identity(len(c))

  # [ 3  5  1  2 -1  4]
  t = np.array([[0.9,  0.05, 0.05, 0.0,  0.0,  0.0], 
                [0.05, 0.9,  0.04, 0.01, 0.0,  0.0], 
                [0.0,  0.05, 0.9,  0.05, 0.0,  0.0], 
                [0.0,  0.0,  0.05, 0.9,  0.05, 0.0], 
                [0.1,  0.1,  0.1,  0.1,  0.5,  0.1], 
                [0.0,  0.0,  0.00, 0.0,  0.2,  0.8]])

  #print(t[1]) # horz
  #print(t[:,1]) # vert
  tc = np.cumsum(t, axis=1)
  # TODO timing...
  u = np.random.sample(len(hh))
  for i in range(len(hh)):
    current = hh.loc[i, "LC4408_C_AHTHUK11"]
    hh.loc[i, "LC4408_C_AHTHUK11"] = sample(u[i], tc[current], c)

  print(hh.LC4408_C_AHTHUK11.head())

  tc = np.cumsum(t, axis=1)

  print(np.cumsum(t[1]))
  #print()