import random
import sys

class Particle:

    def __init__( self ):
        pass

    def __str__( self ):
        return str( "H %f %f %f %f %f %f 0 0 0" % ( self.pos[0],
                                               self.pos[1],
                                               self.pos[2],
                                               self.vel[0],
                                               self.vel[1],
                                               self.vel[2] ) )


def create_random_particles( num, area_size ):
    particles = [ Particle() for x in range( 0, num ) ]
    for particle in particles:
        particle.pos = tuple( random.uniform( 0, comp ) for comp in area_size )
        particle.vel = tuple( random.uniform( 0, comp / 100.0 ) for comp in area_size )

    return particles


if len(sys.argv) < 2:
    sys.stderr.write("please specify particles number\n")
    sys.exit(1)

particles_num = int(sys.argv[1])

particles = create_random_particles( particles_num, ( 10, 10, 10 ) )

for particle in particles:
    print( particle )
