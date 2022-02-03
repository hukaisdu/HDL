from __future__ import print_function, division
from scipy import stats
from math import log, sqrt

#def computeN( p0, p1, alpha0, alpha1 ):
#    return ( ( sqrt( p0 * ( 1 - p0 ) ) * stats.norm.ppf( 1 - alpha0 ) + sqrt( p1 * ( 1 - p1 ) ) * stats.norm.ppf( 1 - alpha1 #) ) / ( p1 - p0 ) ) ** 2

def computeN( e0, e1, alpha0, alpha1 ):
    return ( ( sqrt( 0.25 - e0**2 ) * stats.norm.ppf( 1 - alpha0 ) + sqrt( 0.25 - e1 ** 2 ) * stats.norm.ppf( 1 - alpha1 ) ) / ( e1 - e0 ) ) ** 2

def computeAlpha0 ( wrongKeyNumber, p ):
    return 1 - 2 ** ( log( 1 - p, 2 ) / wrongKeyNumber ) 

def computeTao( N, e0, alpha0 ):
    return N * ( 0.5 + e0 ) + stats.norm.ppf( 1 - alpha0 ) * sqrt( N * ( 0.25 - e0 ** 2  ) )

def computeTao1( N, e1, alpha1 ):
    return N * ( 0.5 + e1 ) - stats.norm.ppf( 1 - alpha1 ) * sqrt( N * ( 0.25 - e1 ** 2 ) )

if __name__ == '__main__':
    e0 = 2** -4.47  
    e1 = 2** -3.19

    p0 = 0.5 + e0
    p1 = 0.5 + e1

    alpha0 = computeAlpha0( 7, 0.05 )

    alpha1 = 0.05

    N = computeN( e0, e1, alpha0, alpha1 )

    print ( N )

    print ( log(N, 2) )

    tao = computeTao( N, e0, alpha0 )

    print ( N * p0,  tao, N * p1 )









