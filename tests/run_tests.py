#!/bin/env python

import unittest
import subprocess
import os
import sys
import shutil
import filecmp
import re 

class MDTestCase(unittest.TestCase):
    def setUp( self ) :
        pass
    def tearDown( self ) :
        pass

    def run_exe(self, args):
        proc = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = proc.communicate()
        exitcode = proc.returncode

        out = out.decode(sys.stdout.encoding)
        err = err.decode(sys.stderr.encoding)

        return { "exitcode": exitcode, "stdout": out, "stderr": err }

    def run_test(self, name):
        return self.run_exe([self.exe_name, '--gtest_filter=*{}*'.format(name)])

    def assert_test_ok(self, result):
        exitcode, stdout, stderr = result['exitcode'], result['stdout'], result['stderr']
        errmsg = 'Exit code = {}\nstdout:\n{}\nstderr:\n{}'.format(exitcode, stdout, stderr)
        self.assertEqual(exitcode, 0, msg=errmsg)
        self.assertFalse(re.match('(error|exception|fail)', stdout, flags=re.I), msg=errmsg)
        self.assertFalse(re.match('(error|exception|fail)', stderr, flags=re.I), msg=errmsg)


class AlgorithmsTest(MDTestCase):
    def setUp(self):
        self.exe_name = "algorithms_test"
        if os.name == 'nt':
            self.exe_name += '.exe'

        self.exe_name = os.path.abspath(os.path.join(os.path.curdir, self.exe_name))

    def test_lennard_jones(self):
        self.assert_test_ok(self.run_test('lennard_jones'))

    def test_periodic_basic(self):
        self.assert_test_ok(self.run_test('periodic_basic'))

    def test_periodic_full(self):
        self.assert_test_ok(self.run_test('periodic_full'))


class FileIO(MDTestCase):
    def setUp(self):
        self.exe_name = "file_io_test"
        if os.name == 'nt':
            self.exe_name += '.exe'

        self.exe_name = os.path.abspath(os.path.join(os.path.curdir, self.exe_name))

    def test_trace_read(self):
        self.assert_test_ok(self.run_test('trace_read'))

    def test_trace_write(self):
        self.assert_test_ok(self.run_test('trace_write'))

    def test_trace_read_write(self):
        self.assert_test_ok(self.run_test('test_read_write'))

    def test_state_read(self):
        self.assert_test_ok(self.run_test('state_read'))

    def test_state_write(self):
        self.assert_test_ok(self.run_test('state_write'))

    def test_state_read_write(self):
        self.assert_test_ok(self.run_test('state_read_write'))

algorithms = AlgorithmsTest()
file_io = FileIO()

fast_tests = unittest.TestSuite([algorithms, file_io])

all_tests = unittest.TestSuite([fast_tests])

if __name__ == '__main__':
    unittest.TextTestRunner(verbosity=2).run(all_tests)
