#!/bin/bash
# 
# script para multiplicacao de matrizes de tamanho Tamanho 
#
# uso: Xmit.sh 
#
# argumentos
#
export RanksMPI=1
export NODES=1
export Tamanho=1024
#
# nomes executavel, tarefa, arquivo de saida
#
DirBase=`pwd`
export Exec=${DirBase}/MM.exe
export Job=MM
export ArqSaida=${DirBase}/MM.out
#
# produz arquivo com comandos batch
#
cat <<EOF0> ${DirBase}/ssub.sh
#!/bin/bash
#SBATCH --nodes=${NODES}
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=${RanksMPI}
#SBATCH --cpus-per-task=1
#SBATCH -p cpu_dev
#SBATCH -J $Job
#SBATCH --time=00:05:00
#SBATCH --output=${ArqSaida}
#SBATCH --exclusive

cd ${DirBase}
ulimit -s unlimited
export OMP_NUM_THREADS=1

srun -n \$SLURM_NTASKS ${Exec} ${Tamanho}
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
