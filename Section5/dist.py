from __future__ import print_function, division
from scipy import stats
from math import log, sqrt

def computeN( p0, p1, a0, a1 ):
    return ( sqrt( p0 * ( 1 - p0 ) ) * stats.norm.ppf( 1 - a0 ) + sqrt( p1 * ( 1 - p1) ) * stats.norm.ppf( 1 - a1 ) ) ** 2 / ( p1 - p0 ) **2

if __name__ == '__main__':


    e0 = 2**-4.75
    e1 = 2**-3.19

    p0 = 0.5 + e0
    p1 = 0.5 + e1

    N = computeN( p0, p1, 0.05, 0.05 )
    print( N ) 












