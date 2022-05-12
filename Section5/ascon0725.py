from __future__ import print_function, division

from sage.all import *
from ascon import *
from sage.rings.polynomial.pbori.pbori import *
from sage.rings.polynomial.pbori import *

import sys
from forceBias import *

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
    if p != 0:
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

def findInsectionTerm( p0,  p2 ):
    if p2 == 0:
        return 0
    V0 = set( p0.variables() )
    p = 0
    for term in list( iter( p2 ) ): 
        if set( term.variables() ).intersection( V0 ):
            p = p + term
    return p

def reducePolynomial( p0, p1 ): 
    return sum ( BooleSet(p0).diff( BooleSet( p1 ) ) )

def Separate( p, L ):
    if p == 1:
        L.append ( p )
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

def Sub( X, Q, A, B, C, D ):
    AA = [0] * 320
    BB = [0] * 320
    CC = [0] * 320
    DD = [0] * 320
    for i in range(320):

        y11 = X[i] / ( R( u ) * R ( v ) )

        y = X[i] + y11 * R( u ) * R ( v )  

        y10 = y / ( R( u ) )

        y01 = y / ( R ( v ) )

        y00 = y + y10 * R( u ) + y01 * R(v) 


        # check yc
        if len ( y00.variables() ) >= 2:
            # generate a new variable
            AA[i] = A[i]
            Q[ str( AA[i] ) ] = y00

        else:
            AA[i] = y00

        if len ( y10.variables() ) >= 2:
            # generate a new variable
            BB[i] = B[i]
            Q[ str( BB[i] ) ] = y10
        else:
            BB[i] = y10

        if len ( y01.variables() ) >= 2:
            # generate a new variable
            CC[i] = C[i]
            Q[ str( CC[i] ) ] = y01
        else:
            CC[i] = y01

        if len ( y11.variables() ) >= 2:
            # generate a new variable
            DD[i] = D[i]
            Q[ str( DD[i] ) ] = y11
        else:
            DD[i] = y11


        X[i] = AA[i] + BB[i] * R( u ) + CC[i] * R( v ) + DD[i] * R( u ) * R( v )

def isolate( f ):
    el = 0
    for z in list( iter( f.graded_part(1)) ) :
        if f / z == 1:
            el += z
    return f - el, el

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


def disjointBias( f ):

    F = transform(f)
    deg = F.graded_part ( 2 )

    if len( deg.monomials() ) == 0:
        return 0

    return 2 ** ( ( -1 * len( deg.monomials() ) ) - 1 ) 

if __name__  == '__main__':
    R = declare_ring( [Block( 'x', 128 ), 
                   Block( 'k', 128 ), 
                   Block( 'A0', 320 ), 
                   Block( 'A1', 320 ), 
                   Block( 'A2', 320 ), 
                   Block( 'A3', 320 ), 
                   Block( 'A4', 320 ), 
                   Block( 'A5', 320 ), 
                   Block( 'B0', 320 ), 
                   Block( 'B1', 320 ), 
                   Block( 'B2', 320 ), 
                   Block( 'B3', 320 ), 
                   Block( 'B4', 320 ), 
                   Block( 'B5', 320 ), 
                   Block( 'C0', 320 ), 
                   Block( 'C1', 320 ), 
                   Block( 'C2', 320 ), 
                   Block( 'C3', 320 ), 
                   Block( 'C4', 320 ), 
                   Block( 'C5', 320 ), 
                   Block( 'D0', 320 ), 
                   Block( 'D1', 320 ), 
                   Block( 'D2', 320 ), 
                   Block( 'D3', 320 ), 
                   Block( 'D4', 320 ), 
                   Block( 'D5', 320 ), 
                   'u', 'v', 'w', 't' ], globals() )

    #index = int( sys.argv[1] )


    print( "*******************************" * 3 )

    X = [0] * 320 # whole state
    # set the initial state
    #IV = 0x80400c0600000000
    #IV = 0x80400c0600000000
    #IV = IV >> 1
    IV = 0


    Q = {} # the substitute dictionary

    for i in range(64):
        X[i] = R ( IV >> (63 - i) & 0x1 )

    for i in range(128):
        X[64 + i] = R( k(i) )

    for i in range(128):
        X[192 + i] = R( x(i) )


    X[192 + 0 ] += R( u )
    X[256 + 0 ] += R( u )

    X[192 + 7 ] += R( v )
    X[256 + 7 ] += R( v )

    I = set()

    # First Round
    X = PA ( X, 0 )

    print( X[0] )

    # substitute
    A = [ R( A0(i) ) for i in range(320) ] 
    B = [ R( B0(i) ) for i in range(320) ] 
    C = [ R( C0(i) ) for i in range(320) ] 
    D = [ R( D0(i) ) for i in range(320) ] 

    Sub( X, Q, A, B, C, D ) 
    # variable substitution

    X = RF( X, 1 )

    for i in range(320):
        df = X[i] / R ( u * v )
        if df not in ( 0, 1 ):
            I.add( df )

            ID = ideal( list( I ) )
            for j in range(320):
                X[j] = X[j].reduce( ID )

    for i in range(320):
        df = X[i] / R ( u )
        if df not in ( 0, 1 ):
            I.add( df )

            ID = ideal( list( I ) )
            for j in range(320):
                X[j] = X[j].reduce( ID )

    for i in range(320): 
        df = X[i] / R ( v )
        if df not in (0, 1):
            I.add( df )

            ID = ideal( list( I ) )
            for j in range(320):
                X[j] = X[j].reduce( ID )

    # substitute
    A = [ R( A1(i) ) for i in range(320) ] 
    B = [ R( B1(i) ) for i in range(320) ] 
    C = [ R( C1(i) ) for i in range(320) ] 
    D = [ R( D1(i) ) for i in range(320) ] 

    Sub( X, Q, A, B, C, D)

    # Round 2
    X = RF( X, 2 )

    for i in range(320):
        df = X[i] / R ( u * v )
        if df not in ( 0, 1 ):
            I.add( df )

            ID = ideal( list( I ) )
            for j in range(320):
                X[j] = X[j].reduce( ID )


    for i in range(320):
        df = X[i] / R ( u )
        if df not in ( 0, 1 ):
            I.add( df )

            ID = ideal( list( I ) )
            for j in range(320):
                X[j] = X[j].reduce( ID )

    for i in range(320): 
        df = X[i] / R ( v )
        if df not in (0, 1 ):
            I.add( df )

            ID = ideal( list( I ) )
            for j in range(320):
                X[j] = X[j].reduce( ID )

    #for i in range(320):
    #    print( X[i] / R( u * v ), X[i] / R ( u ), X[i] / R ( v ) )

    # substitute
    A = [ R( A2(i) ) for i in range(320) ] 
    B = [ R( B2(i) ) for i in range(320) ] 
    C = [ R( C2(i) ) for i in range(320) ] 
    D = [ R( D2(i) ) for i in range(320) ] 

    Sub( X, Q, A, B, C, D )

    X = RF( X, 3 )

    # substitute
    A = [ R( A3(i) ) for i in range(320) ] 
    B = [ R( B3(i) ) for i in range(320) ] 
    C = [ R( C3(i) ) for i in range(320) ] 
    D = [ R( D3(i) ) for i in range(320) ] 

    Sub( X, Q, A, B, C, D )

    X = RF( X, 4 )

    # substitute
    A = [ R( A4(i) ) for i in range(320) ] 
    B = [ R( B4(i) ) for i in range(320) ] 
    C = [ R( C4(i) ) for i in range(320) ] 
    D = [ R( D4(i) ) for i in range(320) ] 

    Sub( X, Q, A, B, C, D )

    '''
    #X = SboxPN( X )
    X = RF( X, 5 )

    # substitute
    A = [ R( A5(i) ) for i in range(320) ] 
    B = [ R( B5(i) ) for i in range(320) ] 
    C = [ R( C5(i) ) for i in range(320) ] 
    D = [ R( D5(i) ) for i in range(320) ] 

    Sub( X, Q, A, B, C, D )
    #X = SboxPN( X )
    '''

    X = SboxPN( X )

    f = X[ 25 ] / R ( u * v )

    print ( f )

    exit(0)

    if f == 0:
        print ( 0.5 )

    e = 0.5

    while True:
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
            f = fl 
            for val in f.variables():
                if 'A' in str( val ) or 'B' in str( val ) or 'C' in str( val ) or 'D' in str( val ): # need substitute
                    f = f.subs( { str(val): Q[ str( val ) ] } ) 
                    flag = True # do substitution

            if not flag:
                e = 2 * e * bias ( f )
                break
        else:
            break

    print ( 'overall bias: ', end = ' ' )
    if e == 0:
        print ( '-inf' )
    else:
        print ( math.log( abs( e ), 2 ) )
        #fout = open( name, 'a' )
        #print ( index0, index1, index2,  math.log( abs( e ) , 2 ), file = fout )

    II = set()
    sys.stdout.flush()

    for idl in I:
        print ( idl )
        xxx = idl.subs( Q )
        II.add( xxx )

    III = set()

    for idl in II:
        print ( idl )
        xxx = idl.subs( Q )
        III.add( xxx )

    print ( "final" )
    for idl in III:
        print ( idl )









