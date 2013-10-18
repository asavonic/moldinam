import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
from mpl_toolkits.mplot3d import Axes3D

FLOOR = -10
CEILING = 10

class AnimatedScatter(object):
	def __init__(self, numpoints=5):
		self.numpoints = numpoints
		self.stream = self.data_stream()
		self.angle = 0

		self.fig = plt.figure()
		self.ax = self.fig.add_subplot(111,projection = '3d')
		self.ani = animation.FuncAnimation(self.fig, self.update, interval=10, 
										   init_func=self.setup_plot, blit=True)

	def change_angle(self):
		self.angle = (self.angle + 1)%360

	def setup_plot(self):
		x, y, z = next(self.stream)
		self.scat = self.ax.scatter(x, y, z , c="red", s=20, animated=True)

		self.ax.set_xlim3d(-10, 20)
		self.ax.set_ylim3d(-10, 20)
		self.ax.set_zlim3d(0, 10)

		return self.scat,

	def PeriodicBorders(self, x,y, z, sizex, sizey):
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
		
		z_imag = []
		for zvalue in z:
			z_imag.append(zvalue) # NE
			z_imag.append(zvalue) # E
			z_imag.append(zvalue) # SE
			z_imag.append(zvalue) # N
			z_imag.append(zvalue) # S
			z_imag.append(zvalue) # Nw
			z_imag.append(zvalue) # W
			z_imag.append(zvalue) # SW

		return x + x_imag, y + y_imag, z + z_imag


	def data_stream(self):
		"""  """
		flag = True
		print("hi!")
		if flag :
			linecount = 0
			x = []
			y = []
			z = []

			for line in file:
				if line != "\n":
					line = line.split()
					# print (line.__repr__() )
					x.append(eval(line[0]));
					y.append(eval(line[1]));
					z.append(eval(line[2]));
				else:
					# print(x.__repr__() + " \n " ,y.__repr__() + " \n " ,z.__repr__() + "\n")
					x,y,z = self.PeriodicBorders(x,y,z,10,10)
					yield x,y,z
					x = []
					y = []
					z = []
					
		print("LOG ENDED")
		flag = False



	def update(self, i):
		x, y, z = next(self.stream)

		self.scat._offsets3d = ( x, y, z)
		plt.draw()
		return self.scat,

	def show(self):
		plt.show()

if __name__ == '__main__':
	file = open("output.txt")
	a = AnimatedScatter()
	a.show()