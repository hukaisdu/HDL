from __future__ import division, print_function
from sage.all import *
from brial import *
import asconUpdate
from random import randint
import random

def SingleMatrix( X, r0, r1 ):
    Y = []
    for i in range(64):
        Y.append( X[i] + X[( i + (64 - r0) ) % 64] + X[ ( i + (64 - r1) ) % 64 ] )
    return Y

def Matrix( X ):
    X[0  :64]  = SingleMatrix(X[0  : 64], 19, 28 )
    X[64 :128] = SingleMatrix(X[64 :128], 61, 39 )
    X[128:192] = SingleMatrix(X[128:192], 1,  6 )
    X[192:256] = SingleMatrix(X[192:256], 10, 17 )
    X[256:320] = SingleMatrix(X[256:320], 7, 41 )

    return X

def Sbox( Y ):
    Z = [ R(0) for i in range(320)]
    for j in range(64):
        Z[0 + j], Z[64 + j], Z[128 + j], Z[192 + j] , Z[256 + j] = SingleSbox( Y[0 + j], Y[64 + j], Y[128 + j], Y[192 + j], Y[256+j] )
    return Z

def addConst ( X, r ):
    constant = [ 0xf0, 0xe1, 0xd2, 0xc3, 0xb4, 0xa5, 0x96, 0x87, 0x78, 0x69,
            0x5a, 0x4b ]
    base = 184

    for i in range(8):
        if constant[r] >> ( 7 - i ) & 0x1:
            X[ base + i] += 1

    return X

def SingleSbox( y0, y1, y2, y3, y4 ):
    x0 = y4*y1 + y3 + y2*y1 + y2 + y1*y0 + y1 + y0
    x1 = y4 + y3*y2 + y3*y1 + y3 + y2*y1 + y2 + y1 + y0
    x2 = y4*y3 + y4 + y2 + y1 + 1
    x3 = y4*y0 + y4 + y3*y0 + y3 + y2 + y1 + y0
    x4 = y4*y1 + y4 + y3 + y1*y0 + y1
    return x0, x1, x2, x3, x4

def int_2_bin( x ):
    X = []
    for i in range(5):
        X.append( x >> ( 4 - i ) & 0x1 )

    return X

def printD( X ):
    for i in range(5):
        for j in range(64):
            print ( X[64 * i + j], end = ' ' )
        print ()

def monomialDegree( s ):
    d = 0
    for ss in s:
        if ss == 'x' or ss == 'y':
            d += 1
    return d


def getXDegree( s ):
    if str( s ) == '0':
        return -1

    md = 0
    s = str( s ).split( '+' )
    for ss in s:
        d = monomialDegree( ss )
        if d > md:
            md = d
    return md

if __name__ == '__main__':
    R = declare_ring ( [ Block('x', 64), Block( 'k', 320), Block( 'u', 128 ), Block( 'y', 128 ) ] )

    minValue = 1000

    for index0 in range(0, 1):
        for index1 in range(index0 + 1, 64):

            #print( index0, index1 )

            DIFF = [ 0 for i in range(320) ]

            for X in [0x0, 0x10]:
                for D0 in [0x10]:

                    V = [  0 for i in range( 320 ) ]

                    # all 320 bits are variables
                    for i in range( 320 ):
                        V[i] = R ( k(i) )

                    XB =  int_2_bin ( X )
                    DB0 = int_2_bin ( D0 )

                    for index in [index0, index1]:
                        for j in range(5):
                            V[ 64 * j + index ] += XB[j] + DB0[j] * R( x(index) )


                    V = addConst( V, 0 )
                    V = Sbox( V )
                    V = Matrix( V )

                    V = addConst( V, 1 )
                    V = Sbox( V )

                    for i in range(0, 320):
                        DIFF[i]  = getXDegree( V[i] )


                    DIFF = asconUpdate.Matrix( DIFF )


                    DIFF = asconUpdate.update( DIFF, 2 )

                    if min( DIFF[0:64] ) <= 1:
                        minValue = min( DIFF[0:64] )
                        print ( index0, index1, DIFF[0:64].index(1), X, D0, minValue )






        #print ( minValue )
