import os
try:
    MD_ROOT = os.environ[ "MD_ROOT" ]
except KeyError:
    MD_ROOT = os.path.join(os.path.dirname(__file__), "..")

if not os.path.isdir( MD_ROOT ) :
    raise RuntimeError( "MD_ROOT is not valid" )
