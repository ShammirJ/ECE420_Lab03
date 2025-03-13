''' A Python program to test the result of the linear solver process 
    IMPORTANT: Need to install sympy
'''

import sympy as sp

def PrintMat(M:list):
    '''Print a matrix given the matrix in the form of [ [], ...]'''

    for row in M:
        print(row)

def Read_Mat(fname:str):
    '''Read the input matrix from an input file
   
       Para: fname -> Name of this input file
       Return: The matrix M and the the number of row n
    '''

    with open(fname, "r") as file:
        lines = file.readlines()

    n = int(lines[0].strip())

    A = list()
    for i in range(2, n+2):
        A.append([float(num) for num in lines[i].strip().split('\t')])

    b = list()
    for i in range(13, n + 13):
        b.append([float(lines[i].strip())])

    A = sp.Matrix(A)
    b = sp.Matrix(b)
    Ab = A.row_join(b)

    return Ab, n

def RREF(M:sp.Matrix):
    '''Perform the RREF of the matrix M
       Para: M -> The matrix M

       Return: A RREF Matrix and the pivot column
    '''
    R = M.rref()


if __name__ == "__main__":
    M, n = Read_Mat("data_input")
    R = RREF(M)
    sp.pprint(R)
