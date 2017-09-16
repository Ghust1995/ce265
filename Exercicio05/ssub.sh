#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH -p nvidia_dev
#SBATCH -J V_16X16
#SBATCH --time=00:02:00
#SBATCH --output=Vida_16X16.out
#SBATCH --exclusive

cd $SLURM_SUBMIT_DIR
ulimit -s unlimited
export OMP_NUM_THREADS=1
srun -n $SLURM_NTASKS /scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio05/Vida.exe 16 16
