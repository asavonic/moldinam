import unittest
import sys
import subprocess

class WidgetTestCase(unittest.TestCase):
    def setUp(self):
        self.sample_exec = open(sys.argv[1])
        self.subject_exec = open(sys.argv[2])


    def execute(self,*args):
        subprocess.call(lest(args))

    def tearDown(self):
        self.widget.dispose()
        self.widget = None

    def writeInputFile(cellSize, molNum, molList=None):
        f = open("input.txt")
        map(lambda x : print(x % "%d "), cellSize)

    def test_2D_random_molecules(self):
        cellSize = [10, 10, 10]
        molNum = 20

    def test_3D_random_molecules(self):

    def test_default_size(self):
        self.assertEqual(self.widget.size(), (50,50),
                         'incorrect default size')

    def test_resize(self):
        self.widget.resize(100,150)
        self.assertEqual(self.widget.size(), (100,150),
                         'wrong size after resize')