import random
from ScatterAnimPlotClass import ScatterAnimPlot
import math
from molecule import Molecule
# import numpy
from functools import reduce

def GetData(molList):
    x = []
    y = []
    for mol in molList:
        x.append(mol.coord[0])
        y.append(mol.coord[1])

    return x,y

def PeriodicBorders(x,y, sizex, sizey):
    x_imag = []
    for xvalue in x:
        x_imag.append(xvalue+sizex) # NE
        x_imag.append(xvalue+sizex) # E
        x_imag.append(xvalue+sizex) # SE
        x_imag.append(xvalue)       # N
        x_imag.append(xvalue)       # S
        x_imag.append(xvalue-sizex) # NW
        x_imag.append(xvalue-sizex) # W
        x_imag.append(xvalue-sizex) # SW
    
    y_imag = []
    for yvalue in y:
        y_imag.append(yvalue + sizey) # NE
        y_imag.append(yvalue)         # E
        y_imag.append(yvalue - sizey) # SE
        y_imag.append(yvalue + sizey) # N
        y_imag.append(yvalue - sizey) # S
        y_imag.append(yvalue + sizey) # Nw
        y_imag.append(yvalue)         # W
        y_imag.append(yvalue - sizey) # SW

    return x + x_imag, y + y_imag


def DataStream():
    """  """
    flag = True
    print("hi!")
    if flag :
        linecount = 0
        x = []
        y = []

        for line in file:
            if line != "\n":
                line = line.split()
                print (line.__repr__() )
                x.append(eval(line[0]));
                y.append(eval(line[1]));
            else:
                print(x.__repr__() + " \n " ,y.__repr__() + "\n")
                x,y = PeriodicBorders(x,y,10,10)
                yield (x,y)
                x = []
                y = []
                
    print("LOG ENDED")
    flag = False


if __name__ == "__main__":
    # file = open("/home/andrew/Projects/MolDinam/c_code/output.txt")
    file = open("output.txt")
    print("gi again!")
    a = ScatterAnimPlot(DataStream, xlim = [-20,30], ylim = [-20,30])
    print("no-no-no")
    a.show()