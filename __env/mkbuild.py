#!/usr/bin/python

from md_env import MD_ROOT
import os
import shutil
import argparse
import git
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument('--hash', dest="build_hash", required=True, help='Hash of git commit')
parser.add_argument('--debug', dest="build_type", action='store_const', const="Debug", default="Release", 
                        help='Build debug binaries')

args = parser.parse_args()


args = parser.parse_args()

build_dir = os.path.join( MD_ROOT, "builds", args.build_hash )

if not os.path.isdir( build_dir ):
    os.makedirs( build_dir )
else:
    shutil.rmtree( build_dir )
    os.makedirs( build_dir )

tmp_dir = os.path.join( MD_ROOT, "tmp" )

if not os.path.isdir( tmp_dir ):
    os.makedirs( tmp_dir )

repo = git.Repo( MD_ROOT )
head = repo.head.commit.sha
repo.git.checkout( args.build_hash )

os.chdir( tmp_dir )

subprocess.call( [ "cmake" , MD_ROOT, "-DCMAKE_INSTALL_PREFIX:PATH=" + build_dir,
                   "-DCMAKE_BUILD_TYPE=" + args.build_type, "-DCMAKE_CXX_FLAGS=" + "-O2" ])

subprocess.call( [ "make", "install" ])

repo.git.checkout( head )
