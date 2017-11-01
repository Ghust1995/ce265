#!/bin/bash
#SBATCH --nodes=3
#SBATCH --ntasks-per-node=24
#SBATCH --ntasks=64
#SBATCH --cpus-per-task=1
#SBATCH -p cpu_dev
#SBATCH -J SP_64
#SBATCH --time=00:03:00
#SBATCH --output=/scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio12/SomaPar_64MPI.out
#SBATCH --exclusive

cd $SLURM_SUBMIT_DIR
ulimit -s unlimited
export OMP_NUM_THREADS=1
srun -n $SLURM_NTASKS --mpi=openmpi -c $SLURM_CPUS_PER_TASK /scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio12/SomaPar.exe
