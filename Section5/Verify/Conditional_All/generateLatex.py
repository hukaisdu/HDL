import sys

def display( line ):
    line = line.replace( ' ' , '' )
    line = line.replace( '=', '=\n' )
    line = line.replace( '\oplus', '\n\oplus ' )
    return r'\begin{autobreak}' + '\n' + line + '\n' + r'\end{autobreak} \\' 
    

if __name__ == '__main__':
    filename = sys.argv[1]

    f = open( filename )

    lines = f.readlines()

    for line in lines:
        line = line.strip()
        line = line.replace( '[', '_{' )
        line = line.replace( ']', '}' )
        line = line.replace( ';', '' )
        line = line.replace( '*', '' )
        line = line.replace( '^', ' \oplus ' )
        print( display( line ) )




