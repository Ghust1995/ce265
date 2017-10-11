#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=24
#SBATCH --ntasks=2
#SBATCH --cpus-per-task=1
#SBATCH -p cpu_dev
#SBATCH -J Func_2
#SBATCH --time=00:01:00
#SBATCH --output=/scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio08/SolFunc_2MPI.out
#SBATCH --exclusive

cd $SLURM_SUBMIT_DIR
ulimit -s unlimited
export OMP_NUM_THREADS=1
srun -n $SLURM_NTASKS --mpi=openmpi -c $SLURM_CPUS_PER_TASK /scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio08/Funciona.exe
