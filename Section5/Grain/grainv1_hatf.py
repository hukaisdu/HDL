from __future__ import division, print_function
from sage.all import *
from brial import *
from forceBias import forceBias, writeC
from math import log


ROUND = 140

def h( x0, x1, x2, x3, x4 ) :
    return x1 + x4 + x0 * x3 + x2 * x3 + x3 * x4 + x0 * x1 * x2 + x0 * x2 * x3 + x0 * x2 * x4 + x1 * x2 * x4 + x2 * x3 * x4

def generate( S, B ):
    a = h( S[3], S[25], S[46], S[64], B[63] )
    b = B[1] + B[2] + B[4] + B[10] + B[31] + B[43] + B[56]

    return a + b

def update( S, B ):

    f = S[0] + S[13] + S[23] + S[38] + S[51] + S[62] + generate ( S, B )

    g = S[0] + \
        B[62] + B[60] + B[52] + B[45] + B[37] + B[33] + B[28] + B[21] + B[14] + B[9] + B[0] + \
        B[63] * B[60] + B[37] * B[33] + B[15] * B[9] + \
        B[60] * B[52] * B[45] + B[33] * B[28] * B[21] + \
        B[63] * B[45] * B[28] * B[9] + B[60] * B[52] * B[37] * B[33] + B[63] * B[60] * B[21] * B[15] + \
        B[63] * B[60] * B[52] * B[45] * B[37] + B[33] * B[28] * B[21] * B[15] * B[9] + \
        B[52] * B[45] * B[37] * B[33] * B[28] * B[21] + generate( S, B )

    for i in range( 79 ):
        S[i] = S[i + 1]
    S[79] = f

    for i in range( 79 ):
        B[i] = B[i + 1]
    B[79] = g

def sub( S, B, Q, r ):
    for index, s in enumerate( S ):
        f11 = s / R( u * v )
        f = f11 * R( u * v ) + s

        f10 = f / R( u )
        f01 = f / R( v )

        f00 = f + f10 * R( u ) + f01 * R( v )

        if len( f11.variables() ) > 2:
            Q[ str( AA[r][index] ) ] = f11
            f11 = AA[r][index]

        if len( f10.variables() ) > 2:
            Q[ str( BB[r][index] ) ] = f10
            f10 = BB[r][index]

        if len( f01.variables() ) > 2:
            Q[ str( CC[r][index] ) ] = f01
            f01 = CC[r][index]

        if len( f00.variables() ) > 2:
            Q[ str( DD[r][index] ) ] = f00
            f00 = DD[r][index]

        S[index] = f11 * R( u * v ) + f10 * R( u ) + f01 * R( v ) + f00

    for index, s in enumerate( B ):
        f11 = s / R( u * v )
        f = f11 * R( u * v ) + s

        f10 = f / R( u )
        f01 = f / R( v )

        f00 = f + f10 * R( u ) + f01 * R( v )

        if len( f11.variables() ) > 2:
            Q[ str( AA[r][index + 80] ) ] = f11
            f11 = AA[r][index + 80]

        if len( f10.variables() ) > 2:
            Q[ str( BB[r][index + 80] ) ] = f10
            f10 = BB[r][index + 80]

        if len( f01.variables() ) > 2:
            Q[ str( CC[r][index + 80] ) ] = f01
            f01 = CC[r][index + 80]

        if len( f00.variables() ) > 2:
            Q[ str( DD[r][index + 80] ) ] = f00
            f00 = DD[r][index + 80]

        B[index] = f11 * R( u * v ) + f10 * R( u ) + f01 * R( v ) + f00

def findInsectionTerm( p0,  p2 ):
    if p2 == 0:
        return 0

    V0 = set( p0.variables() )
    p = 0
    for term in list( iter( p2 ) ): 
        if set( term.variables() ).intersection( V0 ):
            p += term
    return p

def reducePolynomial( p0, p1 ): 
    return sum ( BooleSet(p0).diff( BooleSet( p1 ) ) )

def Separate( p, L ):
    if p == R ( 1 ) or p == 1:
        L.append( p )

    elif p != 0:
        I = p
        I0 = I.lm()
        pp = 0
        while True:
            I0 = findInsectionTerm( I0, I )
            pp += I0
            if I0:
                I = reducePolynomial( I, I0 )
            else:
                L.append( pp )
                break

        Separate( I, L )


def findV( p ):
    V = p.variables()
    m = 2 * len( V )
    O1, O2 = 0,0
    for v in V:
        p0 = p.subs( { str( v ): 0 } ) 
        p1 = p.subs( { str( v ): 1 } ) 

        if len( p0.variables() ) + len( p1.variables() ) < m:
            O1 = p0
            O2 = p1
            m = len( p0.variables() ) + len( p1.variables() )
    
    return O1, O2


def isolate( f ):
    el = 0
    for z in list( iter( f.graded_part(1)) ) :
        if f / z == 1:
            el += z
    return f + el, el

def force_bias( f, d ):
    V = f.variables()
    assert d == len( V ), 'd and V length doesn\'t agree'
    weight = 0
    for i in range( 2 ** d ):
        D = {}
        for j in range( len( V ) ):
            D[ str( V[j] ) ] = i >> ( d - 1 - j ) & 0x1 
        if ( f.subs( D ) == 1 ):
            weight += 1
    return weight / 2 ** d - 0.5

def bias( f ):
    if f == 1:
        return -0.5
    F = []
    Separate( f , F)
    print ( 'Separated', len( F ) )

    for f in F:
        print ( f )
    e = 0.5
    for index, value in enumerate( F ):
        ei = 0
        if len( value.variables() ) < 32:
            #ei = force_bias( value, len( value.variables() ) )
            ei = writeC( str( value ) )
        else:
            F0, F1 = findV( value )
            e1 = bias( F0 )
            e2 = bias( F1 )
            ei = 0.5 * e1 + 0.5 * e2 

        e = 2 * e * ei 

        if e == 0:
            break
    return e



if __name__ == '__main__':

    R = declare_ring( [ Block( 'x', 64 ), 
                        Block( 'k', 80 ), 
                        Block( 'T0', 160 * ROUND ),
                        Block( 'T1', 160 * ROUND ),
                        Block( 'T2', 160 * ROUND ),
                        Block( 'T3', 160 * ROUND ),
                        'u', 'v' ], globals() )

    AA = [ [0 for i in range(160) ] for j in range( ROUND ) ]
    BB = [ [0 for i in range(160) ] for j in range( ROUND ) ]
    CC = [ [0 for i in range(160) ] for j in range( ROUND ) ]
    DD = [ [0 for i in range(160) ] for j in range( ROUND ) ]

    for i in range( ROUND ):
        for j in range(160):
            BB[i][j] = R( T0(i * 160 + j) ) 
            AA[i][j] = R( T1(i * 160 + j) ) 

            CC[i][j] = R( T2(i * 160 + j) ) 
            DD[i][j] = R( T3(i * 160 + j) ) 


    B = [ 0 for i in range(80) ]
    S = [ 0 for i in range(80) ]

    Q = {}

    for i in range(80):
        B[i] = R( k(i) )

    for i in range(64):
        S[i] = R( x(i) )

    for i in range(64, 80):
        S[i] = R( 1 )

    S[21] += R( u )
    S[46] += R( u )

    S[19] += R( v )
    S[44] += R( v )

    I = set()

    for r in range(0, ROUND):
        sys.stdout.flush()

        print ( '*' * 100 )
        sys.stdout.flush()

        sub( S, B, Q, r )

        update( S, B )

        if r < 50:
            for i in range( 80 ):

                df = S[i] / R( u * v ) 
                if df not in (0, 1): 
                    I.add( df )

                    for j in range(80):
                        S[j] = S[j].reduce( ideal ( list(I) ) )
                        B[j] = S[j].reduce( ideal ( list(I) ) )

                df = S[i] / R( u ) 

                if df not in (0, 1): 
                    I.add( df )

                    for j in range(80):
                        S[j] = S[j].reduce( ideal ( list(I) ) )
                        B[j] = B[j].reduce( ideal ( list(I) ) )

                df = S[i] / R( v ) 

                if df not in (0, 1): 
                    I.add( df )

                    for j in range(80):
                        S[j] = S[j].reduce( ideal ( list(I) ) )
                        B[j] = B[j].reduce( ideal ( list(I) ) )

                df = B[i] / R( u * v ) 
                if df not in (0, 1): 
                    I.add( df )

                    for j in range(80):
                        B[j] = B[j].reduce( ideal ( list(I) ) )
                        S[j] = S[j].reduce( ideal ( list(I) ) )


                df = B[i] / R( u ) 
                if df not in (0, 1): 
                    I.add( df )

                    for j in range(80):
                        B[j] = B[j].reduce( ideal ( list(I) ) )
                        S[j] = S[j].reduce( ideal ( list(I) ) )

                df = B[i] / R( v ) 
                if df not in (0, 1): 
                    I.add( df )

                    for j in range(80):
                        B[j] = B[j].reduce( ideal ( list(I) ) )
                        S[j] = S[j].reduce( ideal ( list(I) ) )


        print ( r + 1 )

        sys.stdout.flush()

        if r >= 50: 
            print ( "Computation", r + 1 )
            sys.stdout.flush()

            o = generate( S, B )

            f = o / R( u * v )

            if f == 0:
                print ( 0.5 )
                continue

            e = 0.5

            while True:
                print ( "*****************" )
                print ( f )
                fn, fl = isolate( f )
                print ( "nonlinear part: ",  fn )

                print ( "linear part : ", fl )

                if fn != 0:
                    e = 2 * e * bias ( fn )

                print ( "current bias :", e )

                if e == 0:
                    break

                flag = False # 
                if fl != 0:
                    f = 0
                    for val in fl.variables():
                        if 'T' in str( val ) in str( val ): # need substitute
                            f += fl.subs( { str(val): Q[ str( val ) ] } ) 
                            flag = True # do substitution
                            break

                    if not flag:
                        e = 2 * e * bias ( f )
                        break
                else:
                    break

            if e == 0:
                print ( e )
            else:
                print ( '2^', log( abs( e ), 2 )  ) 

            sys.stdout.flush()

    '''
    fout = open( "ideal.txt", 'w' )
    for i in I:
        print ( i, file = fout )
    fout.close()

    fout = open( "substitution.txt", 'w' )
    for q in Q:
        print ( q, '=',  Q[q], file = fout )
    fout.close()
    '''



