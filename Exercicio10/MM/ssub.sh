#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH -p cpu_dev
#SBATCH -J MM
#SBATCH --time=00:05:00
#SBATCH --output=/scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio10/MM/MM.out
#SBATCH --exclusive

cd /scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio10/MM
ulimit -s unlimited
export OMP_NUM_THREADS=1

srun -n $SLURM_NTASKS /scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio10/MM/MM.exe 1024
