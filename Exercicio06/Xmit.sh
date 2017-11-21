#!/bin/bash
# 
# script para executar versao MPI de Pi no Santos Dumont
#
# uso: Xmit.sh <expoente aleatorios base 2> <ranks MPI>
#
# argumentos
#
export Expo=$1
export RanksMPI=$2
#
# nomes executavel, tarefa, arquivo de saida
#
DirBase=`pwd`
Exec=${DirBase}/Pi
Job=Pi_${RanksMPI}
ArqSaida=${DirBase}/SolPi_${RanksMPI}MPI_${Expo}.out
#
# nos
#
NODES=$[${RanksMPI}/24]
REM=$[${RanksMPI}%24]
if [ ${REM} -gt 0 ]; then export NODES=$[${NODES}+1]; fi
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
#SBATCH --time=00:01:00
#SBATCH --output=${ArqSaida}
#SBATCH --exclusive

cd \$SLURM_SUBMIT_DIR
ulimit -s unlimited
export OMP_NUM_THREADS=1
srun -n \$SLURM_NTASKS --mpi=openmpi -c \$SLURM_CPUS_PER_TASK ${Exec} ${Expo}
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
