import unittest
import subprocess
import os
import shutil
import filecmp

class MDTestCase(unittest.TestCase):
    def setUp( self ) :
        pass
    def tearDown( self ) :
        pass
    def test_algorithms( self ):
        test_exe = os.path.join( os.getcwd(), "algorithms_test" )
        self.assertEqual(subprocess.call( [ test_exe ] ), 0 )

    def test_helpers_read_from_file( self ):
        test_exe = os.path.join( os.getcwd(), "helpers_test" )
        self.assertEqual(subprocess.call( [ test_exe, "--gtest_filter=*read_from_file*" ] ), 0 )

    def test_helpers_write_to_file( self ):
        test_exe = os.path.join( os.getcwd(), "helpers_test" )
        self.assertEqual(subprocess.call( [ test_exe, "--gtest_filter=*write_to_file*" ] ), 0 )
        self.assertEqual( filecmp.cmp( "test_helpers_write_to_file.xyz", "test_helpers_read_from_file.xyz" ), True )
        os.remove( "test_helpers_write_to_file.xyz" );

    def test_helpers_trace_read_write( self ):
        test_exe = os.path.join( os.getcwd(), "helpers_test" )
        self.assertEqual(subprocess.call( [ test_exe, "--gtest_filter=*trace_read_write*" ] ), 0 )

suite = unittest.TestLoader().loadTestsFromTestCase(MDTestCase)

if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(suite)
