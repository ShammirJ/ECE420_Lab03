''' A Python program to test the result of the linear solver process '''

import sympy as sp

def Read_Mat():
    with open("data_input", "r") as file:
        lines = file.readlines()

    n = int(lines[0].strip())

    A = list()
    for i in range(1, n+1):
        A.append([float(num) for num in lines[i].strip().split()])
    print(A)

    b = list()

    for i in range(n + 1, 2 * n + 1):
        b.append([float(lines[i].strip())])

    A = sp.Matrix(A)
    b = sp.Matrix(b)
    Ab = A.row_join(b)  # Augmented Matrix [A | b]

if __name__ == "__main__":
    M = Read_Mat()
    print(M)