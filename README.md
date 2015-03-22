moldynam
========

Molecular dynamic project

Requirements
--------
C++ compiler with C++11 support<br/>
Boost<br/>
Python (optional)

How to get sources
--------
`git clone https://github.com/asavonic/moldynam.git`<br/>
`cd moldynam`<br/>
`git submodule update --init --recursive`<br/>

How to build
--------

**Linux:**<br/>
Debian:
`apt-get install g++ cmake libglew-dev libxrandr-dev libxi-dev libxxf86vm-dev`


`cd moldynam`  
`mkdir build`  
`cd build`  
`cmake ..`  
`make install`  

or you can build it with python script automatically

`export MD_ROOT=<path to moldynam root>`  
`python $MD_ROOT/__env/mkbuild.py` 

**Windows:** <br />
MSVS2012 or higher required for build.

If you already have Boost installed then specify path to it in BOOST_ROOT variable for CMake<br/>
If not, see *Build Boost on Windows* section<br/>

Build Boost on Windows
--------
Download and unpack the latest version: http://www.boost.org/users/download/ <br/>
Go to this unpacked directory and run:
`bootstrap`
`b2 variant=release,debug link=static runtime-link=static stage`