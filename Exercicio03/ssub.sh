#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=4
#SBATCH -p cpu_dev
#SBATCH -J C_4
#SBATCH --time=00:02:00
#SBATCH --output=/scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio03/Out_Crivo_4_OMP_O3
#SBATCH --exclusive

cd $SLURM_SUBMIT_DIR
ulimit -s unlimited
export OMP_NUM_THREADS=4
srun -n $SLURM_NTASKS /scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio03/Crivo.exe 1000000000
