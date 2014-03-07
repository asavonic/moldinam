#!/usr/bin/python

from md_env import MD_ROOT
import os
import shutil
import argparse
import git
import subprocess

parser = argparse.ArgumentParser()
parser.add_argument('--hash', dest="build_hash", required=True, help='Hash of git commit')
args = parser.parse_args()


args = parser.parse_args()

builds_dir = os.path.join( MD_ROOT, "builds", args.build_hash )

if not os.path.isdir( builds_dir ):
    os.makedirs( builds_dir )
else:
    shutil.rmtree( builds_dir )
    os.makedirs( builds_dir )

tmp_dir = os.path.join( MD_ROOT, "tmp" )

if not os.path.isdir( tmp_dir ):
    os.makedirs( tmp_dir )

repo = git.Repo( MD_ROOT )
head = repo.head.commit.sha
repo.git.checkout( args.build_hash )

os.chdir( tmp_dir )

subprocess.call( [ "cmake" , MD_ROOT ])

repo.git.checkout( head )
