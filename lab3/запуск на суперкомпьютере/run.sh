#!/bin/bash
#SBATCH --job-name=matMulMPI_ostritsova
#SBATCH --time=0:05:00
#SBATCH --ntasks=16
#SBATCH --partition batch

module load intel/psxe
module load intel/mpi4
export I_MPI_CXX=g++

mpirun -np 1 -r ssh ./mpi_matrix input/A_300.txt input/B_300.txt output/C_300.txt
mpirun -np 4 -r ssh ./mpi_matrix input/A_300.txt input/B_300.txt output/C_300.txt

mpirun -np 1 -r ssh ./mpi_matrix input/A_500.txt input/B_500.txt output/C_500.txt
mpirun -np 4 -r ssh ./mpi_matrix input/A_500.txt input/B_500.txt output/C_500.txt

mpirun -np 1 -r ssh ./mpi_matrix input/A_600.txt input/B_600.txt output/C_600.txt
mpirun -np 4 -r ssh ./mpi_matrix input/A_600.txt input/B_600.txt output/C_600.txt
mpirun -np 8 -r ssh ./mpi_matrix input/A_600.txt input/B_600.txt output/C_600.txt

mpirun -np 1 -r ssh ./mpi_matrix input/A_1200.txt input/B_1200.txt output/C_1200.txt
mpirun -np 4 -r ssh ./mpi_matrix input/A_1200.txt input/B_1200.txt output/C_1200.txt
mpirun -np 8 -r ssh ./mpi_matrix input/A_1200.txt input/B_1200.txt output/C_1200.txt
mpirun -np 16 -r ssh ./mpi_matrix input/A_1200.txt input/B_1200.txt output/C_1200.txt

mpirun -np 1 -r ssh ./mpi_matrix input/A_2000.txt input/B_2000.txt output/C_2000.txt
mpirun -np 4 -r ssh ./mpi_matrix input/A_2000.txt input/B_2000.txt output/C_2000.txt
mpirun -np 8 -r ssh ./mpi_matrix input/A_2000.txt input/B_2000.txt output/C_2000.txt
mpirun -np 16 -r ssh ./mpi_matrix input/A_2000.txt input/B_2000.txt output/C_2000.txt

