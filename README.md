moldynam
========

Molecular dynamic project

TODO
--------
*Common:*
* Area split (24h)
* binary traces (1h)
* Windows build (8h)

*Visualizer:*
* trace rewind forward/backward (1h)
* trace speed (1h)
* particle color from velocity (2h)

*Optimization:*
* Native with OMP (port to platforms) (3h)
* Native with TBB (5h)
* OpenCL: avoid global barrier after each iteration (?)
* MPI: depends on area split (8h)

*Code cleanup*
* Move old code to reference directory and use it only it tests (3h)

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
Go to this unpacked directory and run:<br/>
`bootstrap`<br/>
`b2 variant=release,debug link=static runtime-link=static stage`<br/>
