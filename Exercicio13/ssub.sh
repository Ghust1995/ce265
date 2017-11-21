#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH -p cpu_dev
#SBATCH -J C_Seq
#SBATCH --time=00:02:00
#SBATCH --output=/scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio13/Out_Crivo
#SBATCH --exclusive

cd $SLURM_SUBMIT_DIR
ulimit -s unlimited
export OMP_NUM_THREADS=1
srun -n $SLURM_NTASKS /scratch/ce2652016/gustavo.guimaraes/ce265/Exercicio13/Crivo.exe 1000000000
