#!/bin/env python2

from md_env import MD_ROOT
import os
import platform
import shutil
import argparse
import git
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument('--hash', dest="build_hash", help='Hash of git commit')
parser.add_argument('--clean', dest="clean", help='Clean installation directory before install')
parser.add_argument('--debug', dest="build_type", action='store_const', const="Debug", default="Release", help='Build debug binaries')
parser.add_argument('--rel-with-dbg', dest="build_type", action='store_const', const="RelWithDebInfo", help='Build relase binaries with debug info')
parser.add_argument('--compiler', dest="compiler", default="gcc", help='available options are gcc, intel')


args = parser.parse_args()

if args.compiler not in [ "gcc", "intel" ]:
    print( "Error! Unsupported value for --compiler: " + args.compiler )

compiler_opt = ""
if args.compiler == "gcc":
    print( "Using compiler: " + args.compiler.upper() )
    compiler_opt = "-DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++"

if args.compiler == "intel":
    compiler_opt = "-DCMAKE_C_COMPILER=icc -DCMAKE_CXX_COMPILER=icc"

if args.build_type == "Debug":
    compiler_opt += " -O0 -g"
else:
    compiler_opt = " -O2"

if not args.build_hash: 
    args.build_hash = "current"

build_dir = os.path.join( MD_ROOT, "builds", args.build_hash )

if not os.path.isdir( build_dir ):
    os.makedirs( build_dir )
else:
    if args.clean:
        # remove all files (leaving empty dirs)
        for root, dirs, files in os.walk(build_dir):
            for file in files:
                os.remove(os.path.join(root, file))

        try:
            os.makedirs(build_dir)
        except OSError as e:
            print("Warning: make dir failed %s" % e)

tmp_dir = os.path.join( MD_ROOT, "tmp" )

if not os.path.isdir( tmp_dir ):
    os.makedirs( tmp_dir )

if args.build_hash != "current":
    repo = git.Repo( MD_ROOT )
    active_branch = repo.active_branch
    repo.git.checkout( args.build_hash )


os.chdir( tmp_dir )

subprocess.call( [ "cmake" , MD_ROOT, "-DCMAKE_INSTALL_PREFIX:PATH=" + build_dir,
                   "-DCMAKE_BUILD_TYPE=" + args.build_type, "-DCMAKE_CXX_FLAGS=" + compiler_opt,
                   "-DCMAKE_EXPORT_COMPILE_COMMANDS=1"])

if platform.system() == "Windows":
    print( "Now you can open tmp/moldynam.sln in Visual Studio and build whatever you want" )
else:
    subprocess.call( [ "make", "install" ])

if args.build_hash != "current":
    repo.git.checkout( active_branch )
