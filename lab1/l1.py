import numpy as np
import sys

def load(path):
    return np.loadtxt(path)

def verify(a_path, b_path, c_path, tol=1e-6):
    A = load(a_path)
    B = load(b_path)
    C = load(c_path)
    C_ref = A @ B
    diff = np.abs(C - C_ref).max() / np.abs(C_ref).max()
    status = "OK" if diff < tol else "FAIL"
    print(f"{c_path}: N={A.shape[0]}, max diff = {diff:.3e} {status}")
    return diff < tol

if __name__ == "__main__":
    sizes = [5, 50, 100, 300, 500]
    ok = True
    for n in sizes:
        ok &= verify(f'input/A_{n}.txt', f'input/B_{n}.txt', f'output/C_{n}.txt')
    sys.exit(0 if ok else 1)
