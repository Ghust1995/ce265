#!/bin/bash
# 
# script para executar SomaSeq.exe no Santos Dumont
#
# uso: XmitSeq.sh
#
# argumentos
#
export RanksMPI=1
#
# nomes executavel, tarefa, arquivo de saida
#
DirBase=`pwd`
Exec=${DirBase}/SomaSeq.exe
Job=SS
ArqSaida=${DirBase}/SomaSeq.out
#
# nos
#
NODES=1
#
# gera arquivo ssub.sh
#
cat <<EOF0> ${DirBase}/ssub.sh
#!/bin/bash
#SBATCH --nodes=${NODES}
#SBATCH --ntasks-per-node=24
#SBATCH --ntasks=${RanksMPI}
#SBATCH --cpus-per-task=1
#SBATCH -p cpu_dev
#SBATCH -J $Job
#SBATCH --time=00:03:00
#SBATCH --output=${ArqSaida}
#SBATCH --exclusive

cd \$SLURM_SUBMIT_DIR
ulimit -s unlimited
export OMP_NUM_THREADS=1
srun -n \$SLURM_NTASKS --mpi=openmpi -c \$SLURM_CPUS_PER_TASK ${Exec}
EOF0
#
# finishes producing file ssub.sh 
#
chmod +x ${DirBase}/ssub.sh
#
# envia ssub.sh para execucao
#
sbatch ssub.sh
exit
