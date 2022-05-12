from __future__ import print_function, division
from scipy import stats
from math import log, sqrt

def computeN( e0, e1, alpha0, alpha1 ):
    return ( ( sqrt( 0.25 - e0**2 ) * stats.norm.ppf( 1 - alpha0 ) + sqrt( 0.25 - e1 ** 2) * stats.norm.ppf( 1 - alpha1 ) ) / ( e1 - e0 ) ) ** 2

def computeAlpha0 ( wrongKeyNumber, p ):
    return 1 - 2 ** ( log( 1 - p, 2 ) / wrongKeyNumber ) 

def computeTao( N, e0, alpha0 ):
    return N * ( 0.5 + e0 ) + stats.norm.ppf( 1 - alpha0 ) * sqrt( N * ( 0.25 - e0 ** 2  ) )

def computeTao1( N, e1, alpha1 ):
    return N * ( 0.5 + e1 ) - stats.norm.ppf( 1 - alpha1 ) * sqrt( N * ( 0.25 - e1 ** 2 ) )

if __name__ == '__main__':

    e0 = 2**-5.80
    e1 = 2**-3.16

    alpha0 = computeAlpha0( 125, 0.05 )

    alpha1 = 0.05

    N = computeN( e0, e1, alpha0, alpha1 )


    print ( N, log(N, 2) )

    tao = computeTao( N, e0, alpha0 )

    tao1 = computeTao1( N, e1, alpha1 )

    print ( tao, tao1 )










