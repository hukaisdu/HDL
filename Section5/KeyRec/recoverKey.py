from sage.all import *
from sage.matrix import *
import random


Equations = []

def genEqu( L, key, flip = 0 ): 
    l = [0] * 129

    v = 0
    for x in L:
        l[x] = 1
        v ^= key[x]

    l[128] = v ^ flip

    return l

def Ind( x, offset ):
    if x < 64:
        return (x + offset) % 64
    else:
        return 64 + ( ( x - 64 ) + offset ) % 64

def Add3( x0, y0, x1, y1, x2, y2 ):
    Equations.append( genEqu( [ Ind( x0, offset ), Ind( y0, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( x1, offset ), Ind( y1, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( x2, offset ), Ind( y2, offset ) ], key ) )

    a = key[ Ind(x0, offset) ] ^ key[ Ind( y0, offset ) ]
    b = key[ Ind(x1, offset) ] ^ key[ Ind( y1, offset ) ]
    c = key[ Ind(x2, offset) ] ^ key[ Ind( y2, offset ) ]
    X = [a, b, c]
    Y = [ Ind( x0, offset ), Ind( x1, offset ), Ind( x2, offset ) ]
    L = [ Ind( y0, offset ), Ind( y1, offset), Ind( y2, offset) ]

    for i in range( 3 ):
        if X[i] == 1:
            L.append( Y[i] ) 
    Equations.append( genEqu( L, key ) )


def recover0725( key, offset ):
    Equations.append( genEqu( [ Ind( 0, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 64, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 7, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 71, offset ) ], key ) )

    # k19
    Equations.append( genEqu( [ Ind( 19, offset ) ], key ) )
    # k26 = c1 , k49 = c2, k3 ⊕k67 = c3, k25 ⊕k89 = c4, k10 ⊕k74 = c5, k32 ⊕k96 = c6,
    Equations.append( genEqu( [ Ind( 26, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 49, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 3, offset ), Ind( 67, offset) ],  key )  ) 
    Equations.append( genEqu( [ Ind( 25, offset ), Ind( 89, offset) ], key )  )
    Equations.append( genEqu( [ Ind( 10, offset ), Ind( 74, offset) ], key )  ) 
    Equations.append( genEqu( [ Ind( 32, offset ), Ind( 96, offset) ], key )  )

    #k9 ⊕k16 ⊕k19 ⊕k49 ⊕k73 ⊕k80 ⊕k90 = c7
    Equations.append( genEqu( [ Ind( 9, offset ), Ind( 16, offset), Ind( 19, offset), Ind( 49, offset ), Ind( 73, offset), Ind( 80, offset), Ind( 90, offset ) ],  \
        key )  )

    if ( key[ Ind(3, offset) ] ^  key[ Ind(67, offset) ], key[ Ind( 25, offset) ] ^ key[ Ind(89, offset) ] ) == (0, 0): 
        Equations.append( genEqu( [ Ind( 67, offset ), Ind( 89, offset) ], key )  )
    if ( key[ Ind(3, offset) ] ^  key[ Ind(67, offset) ], key[ Ind( 25, offset) ] ^ key[ Ind(89, offset) ] ) == (0, 1): 
        Equations.append( genEqu( [ Ind( 25, offset ), Ind(67, offset), Ind( 89, offset ) ], key ) )
    if ( key[ Ind(3, offset) ] ^  key[ Ind(67, offset) ], key[ Ind( 25, offset) ] ^ key[ Ind(89, offset) ] ) == (1, 0): 
        Equations.append( genEqu( [ Ind( 3, offset ), Ind(67, offset), Ind( 89, offset ) ], key ) )
    if ( key[ Ind(3, offset) ] ^  key[ Ind(67, offset) ], key[ Ind( 25, offset) ] ^ key[ Ind(89, offset) ] ) == (1, 1): 
        Equations.append( genEqu( [ Ind( 3, offset ), Ind( 25, offset), Ind(67, offset), Ind( 89, offset ) ], key ) )

    if ( key[ Ind(10, offset) ] ^  key[ Ind( 32, offset) ], key[ Ind( 74, offset) ] ^ key[ Ind( 96, offset) ] ) == (0, 0): 
        Equations.append( genEqu( [ Ind( 74, offset ), Ind(96, offset) ], key )  )
    if ( key[ Ind(10, offset) ] ^  key[ Ind( 32, offset) ], key[ Ind( 74, offset) ] ^ key[ Ind( 96, offset) ] ) == (0, 1): 
        Equations.append( genEqu( [ Ind( 32, offset ), Ind(74, offset), Ind(96, offset ) ], key ) )
    if ( key[ Ind(10, offset) ] ^  key[ Ind( 32, offset) ], key[ Ind( 74, offset) ] ^ key[ Ind( 96, offset) ] ) == (1, 0): 
        Equations.append( genEqu( [ Ind( 10, offset ), Ind(74, offset), Ind(96, offset ) ], key ) )
    if ( key[ Ind(10, offset) ] ^  key[ Ind( 32, offset) ], key[ Ind( 74, offset) ] ^ key[ Ind( 96, offset) ] ) == (1, 1): 
        Equations.append( genEqu( [ Ind( 10, offset ), Ind(32, offset), Ind(74, offset), Ind( 96, offset ) ], key ) )

    Equations.append( genEqu( [ Ind( 28, offset ), Ind( 92, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 31, offset ), Ind( 95, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 53, offset ), Ind( 117, offset ) ], key ) )

    a = key[ Ind(28, offset) ] ^ key[ Ind( 92, offset ) ]
    b = key[ Ind(31, offset) ] ^ key[ Ind( 95, offset ) ]
    c = key[ Ind(53, offset) ] ^ key[ Ind( 117, offset ) ]
    X = [a, b, c]
    Y = [ Ind( 28, offset ), Ind( 31, offset ), Ind( 53, offset ) ]
    L = [ Ind( 92, offset ), Ind(95, offset), Ind(117, offset) ]

    for i in range( 3 ):
        if X[i] == 1:
            L.append( Y[i] ) 
    Equations.append( genEqu( L, key ) )

    Equations.append( genEqu( [ Ind( 21, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 28, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 51, offset ) ], key ) )

    L = []
    for i in [ 11, 18, 51, 75, 82, 92 ]:
        L.append( Ind( i, offset ) )
    Equations.append( genEqu( L, key, 1 ) )

    Equations.append( genEqu( [ Ind( 12, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 19, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 42, offset ) ], key ) )

    L = []
    for i in [ 2, 9, 12, 42, 66, 73, 83 ]:
        L.append( Ind( i, offset ) )
    Equations.append( genEqu( L, key ) )

    Equations.append( genEqu( [ Ind( 26, offset ), Ind( 90, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 29, offset ), Ind( 93, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 51, offset ), Ind( 115, offset ) ], key ) )

    a = key[ Ind(26, offset) ] ^ key[ Ind( 90, offset ) ]
    b = key[ Ind(29, offset) ] ^ key[ Ind( 93, offset ) ]
    c = key[ Ind(51, offset) ] ^ key[ Ind( 115, offset ) ]

    X = [a, b, c]
    Y = [ Ind( 26, offset ), Ind( 29, offset ), Ind( 51, offset ) ]
    L = [ Ind( 90, offset ), Ind( 93, offset), Ind( 115, offset) ]

    for i in range( 3 ):
        if X[i] == 1:
            L.append( Y[i] ) 
    Equations.append( genEqu( L, key ) )

    Equations.append( genEqu( [ Ind( 35, offset ), Ind( 99, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 38, offset ), Ind( 102, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 60, offset ), Ind( 124, offset ) ], key ) )

    a = key[ Ind(35, offset) ] ^ key[ Ind( 99, offset ) ]
    b = key[ Ind(38, offset) ] ^ key[ Ind( 102, offset ) ]
    c = key[ Ind(60, offset) ] ^ key[ Ind( 124, offset ) ]

    X = [a, b, c]
    Y = [ Ind( 35, offset ), Ind( 38, offset ), Ind( 60, offset ) ]
    L = [ Ind( 99, offset ), Ind( 102, offset), Ind(  124, offset) ]

    for i in range( 3 ):
        if X[i] == 1:
            L.append( Y[i] ) 
    Equations.append( genEqu( L, key ) )

    Equations.append( genEqu( [ Ind( 28, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 35, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 58, offset ) ], key ) )

    L = []
    for i in [ 18, 25, 28, 58, 82, 89, 99 ]:
        L.append( Ind( i, offset ) )
    Equations.append( genEqu( L, key, 1 ) )

    Equations.append( genEqu( [ Ind( 23, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 57, offset ) ], key ) )

    L = []
    for i in [ 23, 47, 54, 57, 111, 118 ]:
        L.append( Ind( i, offset ) )
    Equations.append( genEqu( L, key ) )

    Equations.append( genEqu( [ Ind( 30, offset ) ], key ) )

    L = []
    for i in [ 54, 61, 118, 125 ]:
        L.append( Ind( i, offset ) )
    Equations.append( genEqu( L, key, 1 ) )

    Equations.append( genEqu( [ Ind( 19, offset ), Ind( 83, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 22, offset ), Ind( 86, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 44, offset ), Ind( 108, offset ) ], key ) )

    a = key[ Ind(19, offset) ] ^ key[ Ind( 83, offset ) ]
    b = key[ Ind(22, offset) ] ^ key[ Ind( 86, offset ) ]
    c = key[ Ind(44, offset) ] ^ key[ Ind( 108, offset ) ]

    X = [a, b, c]
    Y = [ Ind( 19, offset ), Ind( 22, offset ), Ind( 44, offset ) ]
    L = [ Ind( 83, offset ), Ind( 86, offset), Ind( 108, offset) ]

    for i in range( 3 ):
        if X[i] == 1:
            L.append( Y[i] ) 
    Equations.append( genEqu( L, key ) )


def recover01451( key, offset ):
    Equations.append( genEqu( [ Ind( 0, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 64, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 14, offset ) ], key ) )

    #######jl
    Equations.append( genEqu( [ Ind( 17, offset ), Ind( 81, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 39, offset ), Ind( 103, offset ) ], key ) )

    a = key[ Ind(17, offset) ] ^ key[ Ind( 81, offset ) ]
    b = key[ Ind(39, offset) ] ^ key[ Ind( 103, offset ) ]

    X = [a, b ]
    Y = [ Ind( 18, offset ), Ind( 39, offset )]
    L = [ Ind( 81, offset ), Ind( 103, offset) ]

    for i in range( 2 ):
        if X[i] == 1:
            L.append( Y[i] ) 
    Equations.append( genEqu( L, key ) )

    Equations.append( genEqu( [ Ind( 33, offset ), Ind( 97, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 36, offset ), Ind( 100, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 58, offset ), Ind( 122, offset ) ], key ) )

    a = key[ Ind(33, offset) ] ^ key[ Ind( 97, offset ) ]
    b = key[ Ind(36, offset) ] ^ key[ Ind( 100, offset ) ]
    c = key[ Ind(58, offset) ] ^ key[ Ind( 122, offset ) ]

    X = [ a, b, c ]
    Y = [ Ind( 33, offset ), Ind( 97, offset ), Ind( 58, offset ) ]
    L = [ Ind( 97, offset ), Ind( 100, offset), Ind( 122, offset ) ]

    for i in range( 3 ):
        if X[i] == 1:
            L.append( Y[i] ) 
    Equations.append( genEqu( L, key, 1 ) )
    #Equations.append( genEqu( [ Ind( 78, offset ) ], key ) )

    Equations.append( genEqu( [ Ind( 21, offset ) ], key )  )
    Equations.append( genEqu( [ Ind( 28, offset ) ], key )  )
    Equations.append( genEqu( [ Ind( 51, offset ) ], key )  )
    L = []
    for i in [ 11, 18, 51, 75, 82, 92 ]:
        L.append( Ind( i, offset ) )
    Equations.append( genEqu( L, key, 1 ) )

    #k33 ⊕ k97 = c0,k36 ⊕ k100 = c1,k58 ⊕ k122 = c2,k33k97 ⊕ k33 ⊕ k36k100 ⊕ k36 ⊕k58k122 ⊕k97 ⊕k100 ⊕k122 ⊕1 = c3,k28 ⊕k92 = c4,k31 ⊕k95 = c5,k53 ⊕ k117 = c6,k28k92 ⊕k28 ⊕k31k95 ⊕k31 ⊕k53k117 ⊕k53 ⊕k92 ⊕k95 ⊕k117 = c7,k23 = c8,k57 = c9,k23 ⊕ k47 ⊕ k54 ⊕ k57 ⊕ k111 ⊕ k118 = c10.
    Equations.append( genEqu( [ Ind( 3, offset ), Ind( 67, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 25, offset ), Ind( 89, offset ) ], key ) )

    a = key[ Ind(3, offset) ] ^ key[ Ind( 67, offset ) ]
    b = key[ Ind(25,offset) ] ^ key[ Ind( 89, offset ) ]

    X = [ a, b ]
    Y = [ Ind( 3, offset ), Ind( 25, offset ) ]
    L = [ Ind( 67, offset ), Ind( 89, offset) ]

    for i in range( 2 ):
        if X[i] == 1:
            L.append( Y[i] ) 
    Equations.append( genEqu( L, key ) )

    #k33k97 ⊕ k33 ⊕ k36k100 ⊕ k36 ⊕k58k122 ⊕k97 ⊕k100 ⊕k122 ⊕1 = c3
    # k19

    # k28 ⊕k92 = c4,k31 ⊕k95 = c5,k53 ⊕ k117 = c6,k28k92 ⊕k28 ⊕k31k95 ⊕k31 ⊕k53k117 ⊕k53 ⊕k92 ⊕k95 ⊕k117 = c7

    Equations.append( genEqu( [ Ind( 28, offset ), Ind( 92, offset) ],  key )  ) 
    Equations.append( genEqu( [ Ind( 31, offset ), Ind( 95, offset) ],  key )  )
    Equations.append( genEqu( [ Ind( 53, offset ), Ind( 117, offset) ], key )  ) 

    a = key[ Ind(28, offset) ] ^ key[ Ind( 92, offset ) ]
    b = key[ Ind(31, offset) ] ^ key[ Ind( 95, offset ) ]
    c = key[ Ind(53, offset) ] ^ key[ Ind( 117, offset ) ]

    if (a, b, c ) == (0, 0, 0):
        Equations.append( genEqu( [ Ind( 92, offset ), Ind( 95, offset), Ind( 117, offset) ], key ) )
    if (a, b, c ) == (0, 0, 1):
        Equations.append( genEqu( [ Ind( 53, offset ), Ind( 92, offset), Ind( 95, offset ), Ind( 117, offset) ], key ) )
    if (a, b, c ) == (0, 1, 0):
        Equations.append( genEqu( [ Ind( 31, offset ), Ind( 92, offset), Ind( 95, offset ), Ind( 117, offset) ], key ) )
    if (a, b, c ) == (0, 1, 1):
        Equations.append( genEqu( [ Ind( 31, offset ), Ind(53, offset), Ind( 92, offset), Ind( 95, offset ), Ind( 117, offset) ], key ) )
    if (a, b, c ) == (1, 0, 0):
        Equations.append( genEqu( [ Ind( 28, offset ), Ind( 92, offset), Ind( 95, offset ), Ind( 117, offset) ], key ) )
    if (a, b, c ) == (1, 0, 1):
        Equations.append( genEqu( [ Ind( 28, offset ), Ind( 53, offset ), Ind( 92, offset), Ind( 95, offset ), Ind( 117, offset) ], key ) )
    if (a, b, c ) == (1, 1, 0):
        Equations.append( genEqu( [ Ind( 28, offset ), Ind( 31, offset ), Ind( 92, offset), Ind( 95, offset ), Ind( 117, offset) ], key ) )
    if (a, b, c ) == (1, 1, 1):
        Equations.append( genEqu( [ Ind( 28, offset ), Ind( 31, offset ), Ind( 53, offset), Ind( 92, offset), Ind( 95, offset ), Ind( 117, offset) ], key ) )

    Equations.append( genEqu( [ Ind( 23, offset ) ], key )  )
    Equations.append( genEqu( [ Ind( 57, offset ) ], key )  )
    Equations.append( genEqu( [ Ind( 23, offset ), Ind( 47, offset), Ind( 54, offset), Ind( 57, offset ), Ind( 111, offset), Ind( 118, offset) ],  \
        key )  )

    Equations.append( genEqu( [ Ind( 12, offset ) ], key )  )
    Equations.append( genEqu( [ Ind( 19, offset ) ], key )  )
    Equations.append( genEqu( [ Ind( 42, offset ) ], key )  )
    L = []
    for i in [ 2, 9, 12, 42, 66, 73, 83 ]:
        L.append( Ind( i, offset ) )
    Equations.append( genEqu( L, key ) )

    Equations.append( genEqu( [ Ind( 7, offset ) ], key )  )
    Equations.append( genEqu( [ Ind( 37, offset ) ], key )  )
    L = []
    for i in [ 4, 7, 37, 61, 68, 125 ]:
        L.append( Ind( i, offset ) )
    Equations.append( genEqu( L, key, 1 ) )


def recover05118( key, offset ):
    Equations.append( genEqu( [ Ind( 0, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 64, offset ) ], key ) )
    #Equations.append( genEqu( [ Ind( 51, offset ) ], key ) )
    #Equations.append( genEqu( [ Ind( 115, offset ) ], key ) )

    #k33 ⊕ k97 = c0,k36 ⊕ k100 = c1,k58 ⊕ k122 = c2,k33k97 ⊕ k33 ⊕ k36k100 ⊕ k36 ⊕k58k122 ⊕k97 ⊕k100 ⊕k122 ⊕1 = c3,k28 ⊕k92 = c4,k31 ⊕k95 = c5,k53 ⊕ k117 = c6,k28k92 ⊕k28 ⊕k31k95 ⊕k31 ⊕k53k117 ⊕k53 ⊕k92 ⊕k95 ⊕k117 = c7,k23 = c8,k57 = c9,k23 ⊕ k47 ⊕ k54 ⊕ k57 ⊕ k111 ⊕ k118 = c10.

    #k33k97 ⊕ k33 ⊕ k36k100 ⊕ k36 ⊕k58k122 ⊕k97 ⊕k100 ⊕k122 ⊕1 = c3
    # k19
    Equations.append( genEqu( [ Ind( 3, offset ), Ind( 67, offset) ],  key )  ) 
    Equations.append( genEqu( [ Ind( 25, offset ), Ind( 89, offset) ], key )  )
    Equations.append( genEqu( [ Ind( 58, offset ), Ind( 122, offset) ], key )  ) 

    a = key[ Ind(3, offset) ] ^ key[ Ind( 67, offset ) ]
    b = key[ Ind(25, offset) ] ^ key[ Ind( 89, offset ) ]

    if (a, b ) == (0, 0):
        Equations.append( genEqu( [ Ind(67, offset), Ind( 89, offset ) ], key ) )
    if (a, b ) == (0, 1):
        Equations.append( genEqu( [ Ind(25, offset ), Ind(67, offset), Ind(89, offset ) ], key ) )
    if (a, b ) == (1, 0):
        Equations.append( genEqu( [ Ind(3, offset ), Ind(67, offset), Ind(89, offset) ], key ) )
    if (a, b ) == (1, 1):
        Equations.append( genEqu( [ Ind(3, offset ), Ind(25, offset), Ind(67, offset), Ind(89, offset ) ], key ) )

    # k28 ⊕k92 = c4,k31 ⊕k95 = c5,k53 ⊕ k117 = c6,k28k92 ⊕k28 ⊕k31k95 ⊕k31 ⊕k53k117 ⊕k53 ⊕k92 ⊕k95 ⊕k117 = c7

    Equations.append( genEqu( [ Ind( 12, offset ), Ind( 76, offset) ],  key )  ) 
    Equations.append( genEqu( [ Ind( 54, offset ), Ind( 118, offset) ],  key )  )

    #a = key[ Ind(12, offset) ] ^ key[ Ind( 76, offset ) ]
    #b = key[ Ind(54, offset) ] ^ key[ Ind( 118, offset ) ]

    #if (a, b ) == ( 0, 0):
    #    Equations.append( genEqu( [ Ind( 76, offset ), Ind( 118, offset) ], key ) )
    #if (a, b ) == ( 0, 1):
    #    Equations.append( genEqu( [ Ind( 54, offset ), Ind( 76, offset), Ind( 118, offset ) ], key ) )
    #if (a, b ) == ( 1, 0):
    #    Equations.append( genEqu( [ Ind( 12, offset ), Ind( 76, offset), Ind( 118, offset ) ], key ) )
    #if (a, b ) == ( 1, 1):
    #    Equations.append( genEqu( [ Ind( 12, offset ), Ind( 54, offset), Ind( 76, offset), Ind( 118, offset ) ], key ) )

    Equations.append( genEqu( [ Ind( 12, offset ) ], key )  )
    Equations.append( genEqu( [ Ind( 19, offset ) ], key )  )
    Equations.append( genEqu( [ Ind( 42, offset ) ], key )  )
    Equations.append( genEqu( [ Ind( 2, offset ), Ind( 9, offset), Ind( 12, offset), Ind( 42, offset ), Ind( 66, offset), Ind( 73, offset), Ind( 83, offset) ],  \
        key )  )

def recover05718( key, offset ):
    Equations.append( genEqu( [ Ind( 0, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 64, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 57, offset ) ], key ) )
    Equations.append( genEqu( [ Ind( 121, offset ) ], key ) )

    #k33 ⊕ k97 = c0,k36 ⊕ k100 = c1,k58 ⊕ k122 = c2,k33k97 ⊕ k33 ⊕ k36k100 ⊕ k36 ⊕k58k122 ⊕k97 ⊕k100 ⊕k122 ⊕1 = c3,k28 ⊕k92 = c4,k31 ⊕k95 = c5,k53 ⊕ k117 = c6,k28k92 ⊕k28 ⊕k31k95 ⊕k31 ⊕k53k117 ⊕k53 ⊕k92 ⊕k95 ⊕k117 = c7,k23 = c8,k57 = c9,k23 ⊕ k47 ⊕ k54 ⊕ k57 ⊕ k111 ⊕ k118 = c10.

    #k33k97 ⊕ k33 ⊕ k36k100 ⊕ k36 ⊕k58k122 ⊕k97 ⊕k100 ⊕k122 ⊕1 = c3
    # k19
    Equations.append( genEqu( [ Ind( 3, offset ), Ind( 67, offset) ],  key )  ) 
    Equations.append( genEqu( [ Ind( 25, offset ), Ind( 89, offset) ], key )  )
    Equations.append( genEqu( [ Ind( 58, offset ), Ind( 122, offset) ], key )  ) 

    a = key[ Ind(3, offset) ] ^ key[ Ind( 67, offset ) ]
    b = key[ Ind(25, offset) ] ^ key[ Ind( 89, offset ) ]

    if (a, b ) == (0, 0):
        Equations.append( genEqu( [ Ind(67, offset), Ind( 89, offset ) ], key ) )
    if (a, b ) == (0, 1):
        Equations.append( genEqu( [ Ind(25, offset ), Ind(67, offset), Ind(89, offset ) ], key ) )
    if (a, b ) == (1, 0):
        Equations.append( genEqu( [ Ind(3, offset ), Ind(67, offset), Ind(89, offset) ], key ) )
    if (a, b ) == (1, 1):
        Equations.append( genEqu( [ Ind(3, offset ), Ind(25, offset), Ind(67, offset), Ind(89, offset ) ], key ) )

    # k28 ⊕k92 = c4,k31 ⊕k95 = c5,k53 ⊕ k117 = c6,k28k92 ⊕k28 ⊕k31k95 ⊕k31 ⊕k53k117 ⊕k53 ⊕k92 ⊕k95 ⊕k117 = c7
    Equations.append( genEqu( [ Ind( 19, offset ) ], key )  )
    Equations.append( genEqu( [ Ind( 42, offset ) ], key )  )
    Equations.append( genEqu( [ Ind( 2, offset ), Ind( 9, offset), Ind( 12, offset), Ind( 42, offset ), Ind( 66, offset), Ind( 73, offset), Ind( 83, offset) ],  key ) )

    Equations.append( genEqu( [ Ind( 18, offset ), Ind( 82, offset ) ],  key )  ) 
    Equations.append( genEqu( [ Ind( 60, offset ), Ind( 124, offset) ],  key )  ) 

    a = key[ Ind(18, offset) ] ^ key[ Ind( 82, offset ) ]
    b = key[ Ind(60, offset) ] ^ key[ Ind( 124, offset ) ]

    if (a, b ) == ( 0, 0):
        Equations.append( genEqu( [ Ind( 82, offset ), Ind( 124, offset) ], key ) )
    if (a, b ) == ( 0, 1):
        Equations.append( genEqu( [ Ind( 60, offset ), Ind( 82, offset), Ind( 124, offset ) ], key ) )
    if (a, b ) == ( 1, 0):
        Equations.append( genEqu( [ Ind( 18, offset ), Ind( 82, offset), Ind( 124, offset ) ], key ) )
    if (a, b ) == ( 1, 1):
        Equations.append( genEqu( [ Ind( 18, offset ), Ind( 60, offset), Ind( 82, offset), Ind( 124, offset ) ], key ) )


if __name__ == '__main__':
    N = 0
    TEST = 500

    for test in range( TEST ):
        Equations.clear()

        key = [ random.randint( 0, 1 ) for i in range(128) ]

        for i in range(64):
            if ( key[i],  key[ i + 64 ], key[ ( i + 7 ) % 64 ], key[ ( i + 7 ) % 64 + 64 ] ) == (0, 0, 0, 0):
                recover0725( key, i )

            if ( key[i],  key[ i + 64 ], key[ ( i + 14 ) % 64 ], key[ ( i + 14 ) % 64 + 64 ] ) == (0, 0, 0, 0):
                recover01451( key, i )

            #if ( key[i],  key[ i + 64 ], key[ ( i + 51 ) % 64 ], key[ ( i + 51 ) % 64 + 64 ] ) == (0, 0, 0, 0):
            #    recover05118( key, i )

            #if ( key[i],  key[ i + 64 ], key[ ( i + 57 ) % 64 ], key[ ( i + 57 ) % 64 + 64 ] ) == (0, 0, 0, 0):
            #    recover05718( key, i )

        
        R = len( Equations )

        if R == 0:
            continue

        C = len( Equations[0] )

        MS = MatrixSpace(GF(2), R, C )

        A = MS( Equations )

        rnk = A.rank() 

        N += rnk

    print( N / TEST )















