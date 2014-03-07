#!/usr/bin/python

from md_env import MD_ROOT
import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument('--hash', dest="build_hash", required=True, help='Hash of git commit')
args = parser.parse_args()


args = parser.parse_args()

builds_dir = os.path.join( MD_ROOT, "builds", args.build_hash )

if not os.path.isdir( builds_dir ):
    os.makedirs( builds_dir )
else:
    os.rmdir( builds_dir )
    os.makedirs( builds_dir )


