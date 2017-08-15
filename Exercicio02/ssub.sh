#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=
#SBATCH -p cpu_dev
#SBATCH -J Tempo
#SBATCH --time=00:01:00
#SBATCH --output=/c/Users/gusta/Desktop/CE265/Exercicio02/OutTempo_OMP
#SBATCH --exclusive

cd $SLURM_SUBMIT_DIR
export OMP_NUM_THREADS=
srun -n $SLURM_NTASKS /c/Users/gusta/Desktop/CE265/Exercicio02/Tempo.exe
