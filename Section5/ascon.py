from sage.all import *
from random import randint
from sage.rings.polynomial.pbori.pbori import *
from sage.rings.polynomial.pbori import *

# total symbolic variables

def getName( label, Round, index ):
    return 'VAR/{}_{}_{}'.format ( label, Round, index )

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

def SingleSboxPN ( x0, x1, x2, x3, x4 ):
    t0 = x0;
    t4 = x4;
    t3 = x3;
    t1 = x1;
    t2 = x2;
    x0 = t0 + ((1 + t1) * t2);
    x2 = t2 + ((1 + t3) * t4);
    x4 = t4 + ((1 + t0) * t1);
    x1 = t1 + ((1 + t2) * t3);
    x3 = t3 + ((1 + t4) * t0);
    x1 = x1 + x0;
    x3 = x3 + x2;
    x0 = x0 + x4;
    x2 = 1 + x2;

    return x0, x1, x2, x3, x4

def SingleSboxPL ( x0, x1, x2, x3, x4 ):
    x0 = x0 + x4
    x4 = x4 + x3
    x2 = x2 + x1

    return x0, x1, x2, x3, x4

def SboxPN( Y ):
    Z = [ 0 for i in range(320)]
    for j in range(64):
        Z[0 + j], Z[64 + j], Z[128 + j], Z[192 + j] , Z[256 + j] = SingleSboxPN( Y[0 + j], Y[64 + j], Y[128 + j], Y[192 + j], Y[256+j] )
    return Z

def SboxPL( Y ):
    Z = [ 0 for i in range(320)]
    for j in range(64):
        Z[0 + j], Z[64 + j], Z[128 + j], Z[192 + j] , Z[256 + j] = SingleSboxPL( Y[0 + j], Y[64 + j], Y[128 + j], Y[192 + j], Y[256+j] )
    return Z

def PA ( X, R ):
    X = addConst ( X, R )
    X = SboxPL ( X )

    return X

def PB( X ):
    X = SboxPN ( X )
    X = Matrix( X )

    return X

def RF( X, R ):
    X = PB ( X )
    X = PA ( X, R )
    return X

def SingleSbox( y0, y1, y2, y3, y4 ):
    x0 = y4*y1 + y3 + y2*y1 + y2 + y1*y0 + y1 + y0
    x1 = y4 + y3*y2 + y3*y1 + y3 + y2*y1 + y2 + y1 + y0
    x2 = y4*y3 + y4 + y2 + y1 + 1
    x3 = y4*y0 + y4 + y3*y0 + y3 + y2 + y1 + y0
    x4 = y4*y1 + y4 + y3 + y1*y0 + y1
    return x0, x1, x2, x3, x4

def Sbox( Y ):
    Z = [ R(0) for i in range(320)]
    for j in range(64):
        Z[0 + j], Z[64 + j], Z[128 + j], Z[192 + j] , Z[256 + j] = SingleSbox( Y[0 + j], Y[64 + j], Y[128 + j], Y[192 + j], Y[256+j] )
    return Z

def addConst ( X, v ):
    constant = [ 0xf0, 0xe1, 0xd2, 0xc3, 0xb4, 0xa5, 0x96, 0x87, 0x78, 0x69,
            0x5a, 0x4b ]
    #constant = [0] * 12

    base = 184

    for i in range(8):
        if constant[v] >> ( 7 - i ) & 0x1:
            X[ base + i] += 1

    return X


if __name__ == '__main__':

    R = declare_ring( [ Block( 'x', 128 ), Block( 'k', 128), Block( 'u',  8 ), 'v' ] )

    X = [ 0 for i in range( 320 ) ]

    IV = 0x80400c0600000000

    for i in range(64):
        X[i] = R ( IV >> (63 - i) & 0x1 )
    for i in range(64):
        X[64 + i] = R ( k(i) )
    for i in range(64):
        X[128 + i] = R ( k(64 + i) )
    for i in range(64):
        X[192 + i] = R ( x(i) )
    for i in range(64):
        X[256 + i] = R ( x(64 + i) )

    X[192 + 0] += R( v )
    X[256 + 0] += R( v )

    X = addConst( X, 0 )
    X = Sbox( X )

    L = [ 230,212,197,166,117,18,0  ]

    res = 1

    for l in L:
        res *= X[l]

    print ( res / R ( v )  ) 










