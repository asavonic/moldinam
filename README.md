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

**Linux:**  
`cd moldynam`  
`mkdir build`  
`cd build`  
`cmake ..`  
`make install`  

or you can build it with python script automatically

`export MD_ROOT=<path to moldynam root>`  
`python $MD_ROOT/__env/mkbuild.py` 

**Windows:** <br />
MS Visual Studio is the only supported compiler for Windows, at least for now, and you can generate VS Solution automatically:<br/><br/>
`set MD_ROOT=<path to moldynam root>`<br/>
`python %MD_ROOT%\__env\mkbuild.py`<br/><br/>
After that you`ll find moldynam.sln in %MD_ROOT%/tmp
