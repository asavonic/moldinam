from os import path, environ

try:
    MD_ROOT = environ[ "MD_ROOT" ]
except KeyError:
    MD_ROOT = path.abspath(path.join(path.dirname(__file__), ".."))

if not path.isdir( MD_ROOT ) :
    raise RuntimeError( "MD_ROOT is not valid" )
