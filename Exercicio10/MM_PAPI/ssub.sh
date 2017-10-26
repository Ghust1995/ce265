#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH -p cpu_dev
#SBATCH -J MM
#SBATCH --time=00:05:00
#SBATCH --output=/scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio10/MM_PAPI/MM.out
#SBATCH --exclusive

cd /scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio10/MM_PAPI
ulimit -s unlimited
export OMP_NUM_THREADS=1

module load bullxde &> /dev/null
module load papi

srun -n $SLURM_NTASKS /scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio10/MM_PAPI/MM.exe 1024
