#!/usr/bin/env python3.7



import gurobipy as gp
from gurobipy import GRB

try:

    # Create a new model
    m = gp.Model("size3")

    # Create variables
    x11 = m.addVar(vtype=GRB.BINARY, name="x11")
    x12 = m.addVar(vtype=GRB.BINARY, name="x12")
    x13 = m.addVar(vtype=GRB.BINARY, name="x13")
    
    x21 = m.addVar(vtype=GRB.BINARY, name="x21")
    x22 = m.addVar(vtype=GRB.BINARY, name="x22")
    x23 = m.addVar(vtype=GRB.BINARY, name="x23")

    x31 = m.addVar(vtype=GRB.BINARY, name="x31")
    x32 = m.addVar(vtype=GRB.BINARY, name="x32")
    x33 = m.addVar(vtype=GRB.BINARY, name="x33")

    

    # Set objective
    m.setObjective(x11+0*x12+0*x13+2*x21+0*x22+8*x23+30*x31+7*x32+x33, GRB.MAXIMIZE)

    # Add constraints: 
    m.addConstr(x11 + x21 + x31 == 1, "c0")
    m.addConstr(x12 + x22 + x32 == 1, "c1")
    m.addConstr(x13 + x23 + x33 == 1, "c2")
    
    m.addConstr(x11 + x12 + x13 == 1, "c3")
    m.addConstr(x21 + x22 + x23 == 1, "c4")
    m.addConstr(x31 + x32 + x33 == 1, "c5")

    # Optimize model
    m.optimize()

    for v in m.getVars():
        print('%s %g' % (v.varName, v.x))

    print('Obj: %g' % m.objVal)

except gp.GurobiError as e:
    print('Error code ' + str(e.errno) + ': ' + str(e))

except AttributeError:
    print('Encountered an attribute error')
