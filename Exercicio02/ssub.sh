#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=16
#SBATCH -p cpu_dev
#SBATCH -J Tempo
#SBATCH --time=00:01:00
#SBATCH --output=/scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio02/OutTempo_16OMP
#SBATCH --exclusive

cd $SLURM_SUBMIT_DIR
export OMP_NUM_THREADS=16
srun -n $SLURM_NTASKS /scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio02/Tempo.exe
