#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=2
#SBATCH -p cpu_dev
#SBATCH -J C_2
#SBATCH --time=00:02:00
#SBATCH --output=/scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio03/Out_Crivo_2_OMP_O3
#SBATCH --exclusive

cd $SLURM_SUBMIT_DIR
ulimit -s unlimited
export OMP_NUM_THREADS=2
srun -n $SLURM_NTASKS /scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio03/Crivo.exe 1000000000
