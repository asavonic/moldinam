import unittest
import subprocess
import os
import shutil

class MDTestCase(unittest.TestCase):
    def setUp( self ) :
        pass
    def tearDown( self ) :
        pass
    def test_test1( self ):
        test_exe = os.path.join( os.getcwd(), "cpp_sorter_test" )
        self.assertEqual(subprocess.call( [ test_exe ] ), 0 )
    def test_algorithms( self ):
        test_exe = os.path.join( os.getcwd(), "algorithms_test" )
        self.assertEqual(subprocess.call( [ test_exe ] ), 0 )

suite = unittest.TestLoader().loadTestsFromTestCase(MDTestCase)

if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite)
