# import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import random

def MakeGrid():
    plt.plot([-10,-10, 20, 20,-10], [-10,20,20,-10, -10], '-',color='blue', linewidth=2)
    plt.plot([10,10], [-10,20], '-', linewidth=2,color='blue')
    plt.plot([0,0], [-10,20], '-', linewidth=2,color='blue')
    plt.plot([-10,20], [10,10], '-', linewidth=2,color='blue')
    plt.plot([-10,20], [0,0], '-', linewidth=2,color='blue')


def update():
    A=[[0],[0]]
    for i in range(25):
        A[0], A[1] = A[0] + [i], A[1] + [i]
        yield A

class ScatterAnimPlot(object):
    """Simple class for drawing scatter plot with animation.
    Update function in __init__ must return tuple object of X coordinates and Y coord."""

    def __init__(self, update, xlim, ylim, init = None, marker = '.', color = 'red'):
        super(ScatterAnimPlot, self).__init__()
        self.update = update
        self.init = init
        self.fig = plt.figure()

        MakeGrid()



        self.line, = plt.plot(None,None, marker, color=color,animated=True)
        plt.xlim(xlim[0], xlim[1])
        plt.ylim(ylim[0], ylim[1])
    
    def draw(self,data):
        self.line.set_xdata(data[0])
        self.line.set_ydata(data[1])
        return self.line,

    def init(self):
        self.line.set_xdata([])
        self.line.set_ydata([])
        return self.line,
    
    def show(self):
        ani = animation.FuncAnimation(self.fig, self.draw, self.update, init_func=self.init, interval=50, blit=True)
        plt.show()


if __name__ == '__main__':
    a = ScatterAnimPlot(update, xlim = [0,30], ylim = [0,30])
    a.show()