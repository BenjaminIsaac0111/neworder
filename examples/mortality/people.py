
import numpy as np
import pandas as pd 
from matplotlib import pyplot as plt
import neworder
import ethpop
import animation

# A more "pythonic" approach using pandas DataFrames

class People():
  """ A simple aggregration of Persons each represented as a row in a data frame """
  def __init__(self, mortality_hazard_file, n):
    # initialise cohort      
    # assert False
    # filter by location, ethnicity and gender
    self.mortality_hazard = ethpop.create(pd.read_csv(mortality_hazard_file), "E09000030").reset_index()

    self.mortality_hazard = self.mortality_hazard[(self.mortality_hazard.NewEthpop_ETH=="WBI") 
                                                & (self.mortality_hazard.DC1117EW_C_SEX==1)]
    #neworder.log(self.mortality_hazard.head())
    self.population = pd.DataFrame(data={"Alive": np.full(n, True),
                                         "Age": np.zeros(n), 
                                         "TimeOfDeath": np.zeros(n)})

  def plot(self, filename=None):
    # dump the population out
    #self.population.to_csv(filename, index=False)

    y1, x1 = np.histogram(self.population.TimeOfDeathNHPP, int(max(self.population.Age)))
    plt.plot(x1[1:], y1)
    y2, x2 = np.histogram(self.population.TimeOfDeath, int(max(self.population.Age)))
    plt.plot(x2[1:], y2)
    #animation.Hist(self.population.TimeOfDeath, int(max(self.population.Age)), filename)

    plt.show()

  def die(self):
    # using indexes to subset data as cannot store a reference to a subset of the dataframe (it just copies)

    # first filter out the already dead
    alive = self.population.loc[self.population.Alive].index
    # sample time of death
    r = neworder.stopping(self.mortality_hazard.Rate.values[min(neworder.timeindex-1, 85)], len(alive))
    # select if death happens before next timestep...
    dt = neworder.timestep
    # at final timestep everybody dies (at some later time) so dt is infinite
    if neworder.time == neworder.MAX_AGE:
      dt = neworder.TIME_INFINITY
    # 
    newly_dead = alive[r<dt]

    # kill off those who die before next timestep
    self.population.ix[newly_dead, "Alive"] = False
    self.population.ix[newly_dead, "TimeOfDeath"] = self.population.ix[newly_dead, "Age"] + np.clip(r[r<dt], 0.0, 15.0)

  def age(self):
    # kill off some people
    self.die()

    # age the living only
    alive = self.population.loc[self.population.Alive].index
    self.population.ix[alive, "Age"] = self.population.ix[alive, "Age"] + neworder.timestep

  def calc_life_expectancy(self):  
    # ensure all people have died 
    #self.dump("./population.csv")

    # in this case we can just compute the mortality directly by modelling a non-homogeneuou Poisson process and 
    # using the Lewis-Shedler algorithm
    self.population["TimeOfDeathNHPP"] = np.clip(
      neworder.stopping_nhpp(self.mortality_hazard.Rate.values, neworder.timestep, len(self.population)), 0, 114)

    #neworder.log(np.histogram(self.population.TimeOfDeathNHPP, 100)[0] - np.histogram(self.population.TimeOfDeath, 100)[0])

    assert np.sum(self.population.Alive) == 0
    neworder.log("%f vs %f" % (np.mean(self.population.TimeOfDeath), np.mean(self.population.TimeOfDeathNHPP)))
    return np.mean(self.population.TimeOfDeath)

  def prop_alive(self):  
    # # compute mean
    neworder.log("pct alive = %f" % (100.0 * np.mean(self.population.Alive)))
    return True