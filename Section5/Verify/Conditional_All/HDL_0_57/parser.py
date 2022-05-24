import sys

def parseValue( line ):
    v = line.split( ':' )[1]

    if 'inf' in v:
        v = '0'

    return round( eval( v ), 1 )

if __name__ == '__main__':
    filename = sys.argv[1] 

    f = open( filename, 'r' )

    lines = f.readlines()

    T = []
    for line in lines:
        v = parseValue( line )
        T.append( v )

    f.close()
        

    filename = sys.argv[2] 

    f = open( filename, 'r' )

    lines = f.readlines()

    E = []
    for line in lines:
        v = parseValue( line )
        E.append( v )


    for i in range(7):
        print( "Bit ", end = ' ' )
        for j in range(10):
            print( "& %d " % ( 10 * i + j ),  end = '' )

            if 10 * i + j == 63:
                break
        print( r'\\' )

        print( "Theory ", end = ' ' )
        for j in range(10):
            if T[10 * i + j ] != 0:
                print( "& $2^{%0.1f}$ " % ( T[10 * i + j] ),  end = '' )
            else:
                print( "& $%d$ " % ( T[10 * i + j] ),  end = '' )

            if 10 * i + j == 63:
                break
        print( r'\\' )

        print( "Expr. ", end = ' ' )
        for j in range(10):
            if E[10 *i + j ] < -10:
                print( "& \color{lightgray}{$2^{%0.1f}$}" % ( E[10 * i + j] ),  end = '' )
            else:
                print( "& $2^{%0.1f}$ " % ( E[10 * i + j] ),  end = '' )

            if 10 * i + j == 63:
                break
        print( r'\\' )
        print( r'\midrule' )
        


