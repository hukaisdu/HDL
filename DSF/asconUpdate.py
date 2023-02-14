from __future__ import print_function, division

def two( a, b ):
    if a == -1 or b == -1:
        return -1
    else:
        return a + b

def SingleSbox( X ):
    Y = [0 for i in range(5) ]
    Y[0] = max( two ( X[4],  X[1] ), X[3],  two( X[2], X[1] ), X[2], two(
        X[1], X[0] ), X[1], X[0] ) 
    Y[1] = max( X[4],        two( X[3], X[2] ), two( X[3] , X[1] ), X[3], two(
        X[2], X[1] ), X[2],
            X[1], X[0]) 
    Y[2] = max( two ( X[4], X[3] ), X[4], X[2], X[1], 0 ) 
    Y[3] = max( two( X[4], X[0] ), X[4], two( X[3], X[0] ), X[3], X[2], X[1], X[0] ) 
    Y[4] = max( two( X[4], X[1] ),  X[4], X[3], two( X[1],  X[0] ), X[1] ) 

    return Y


def Sbox( X ):
    for i in range(64):
        Y = SingleSbox( [ X[0 + i], X[64 + i], X[128 + i], X[192 + i], X[256 +i] ] )  
        for j in range(5):
            X[j * 64 + i] = Y[j]
    return X

def SingleMatrix( X, offset1, offset2 ):
    Y = [0 for i in range(64) ]
    for i in range(64):
        Y[i] = max( X[i], X[( i + 64  - offset1 ) % 64], X[ ( i + 64 - offset2 ) % 64 ] ) 

    return Y

def Matrix( X ):
    Y = [ 0 for i in range(5) ]
    offset = ( (19, 28), (61, 39), (1, 6), (10, 17), (7, 41) )
    for i in range(5):
        Y[i] = SingleMatrix( X[ 64 *i: 64 * (i + 1)], offset[i][0], offset[i][1] )

    return Y[0] + Y[1] + Y[2] + Y[3] + Y[4]

constant = [ 0xf0, 0xe1, 0xd2, 0xc3, 0xb4, 0xa5, 0x96, 0x87, 0x78, 0x69, 0x5a,
    0x4b ]

def Constant( X, const ):
    for i in range(5):
        if const >> ( 5 - i ) & 0x1:
            if X[187 + i] == -1:
                X[187 + i] = 0
    return X

def printD( X ):
    for i in range(5):
        for j in range(64):
            print ( X[64 * i + j], end = ' ' )
        print ()

def update ( X, rounds ): 
    
    for i in range(320):
        X[i] = max(0, X[i])

    for i in range( rounds - 1):
        X = Sbox( X )
        X = Matrix( X )

    X = Sbox( X )

    return X

def updateWDP ( X, rounds ): 
    for i in range(rounds - 1):
        #X = Constant( X, constant[i + 2] )
        X = SingleSbox( X )
        #X = Matrix( X )
        #print( 'Round' )

    #X = Constant( X, constant[rounds + 1] )
    X = SingleSbox( X )

    return X

if __name__ == '__main__':
    X = [0 for i in range(64) ] +\
        [1 for i in range(64) ] +\
        [1 for i in range(64) ] +\
        [1 for i in range(64) ] +\
        [0 for i in range(64) ]

    X = update( X, 6 )

    for i in range(5):
        for j in range(64):
            print ( X[64 * i + j], end = ' ' )
        print ()

