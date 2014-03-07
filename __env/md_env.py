import os
try:
    MD_ROOT = os.environ[ "MD_ROOT" ]
except KeyError:
    raise RuntimeError( "MD_ROOT is not set" )

if not os.path.isdir( MD_ROOT ) :
    raise RuntimeError( "MD_ROOT is not valid" )
