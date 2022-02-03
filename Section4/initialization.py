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
    R = declare_ring ( [ Block('x', 64), Block( 'k', 256), Block( 'u', 128 ), Block( 'y', 128 ) ] )

    IV = 0x80400c0600000000
    
    minValue = 64

    for X in range(0, 4):
        for D0 in range(1, 4):
            V = [  0 for i in range( 320 ) ]

            DIFF = [ 0  for i in range( 320 ) ]

            for i in range(64):
                V[i] = R ( IV >> ( 63 - i ) & 0x1 ) 

            for i in range( 128 ):
                V[i + 64] = R ( k(i) )

            XB =  int_2_bin ( X )
            DB0 = int_2_bin ( D0 )

            for index in range(0, 64):
                for j in range(3, 5):
                    V[ 64 * j + index ] += XB[j] + DB0[j] * R( x(index) )

            V = addConst( V, 0 )
            V = Sbox( V )
            V = Matrix( V )

            V = addConst( V, 1 )
            V = Sbox( V )

            for i in range(0, 320):
                DIFF[i]  = getXDegree( V[i] )

            DIFF = asconUpdate.Matrix( DIFF )

            DIFF = asconUpdate.update( DIFF, 5 )

            if min( DIFF[0:64] ) <= minValue:
                minValue = min( DIFF[0:64] )

                print ( X, D0, minValue )
