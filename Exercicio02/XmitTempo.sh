#!/bin/bash
# 
# script para executar Tempo.exe em OpenMP no SantosDumont
#
# uso: XmitTempo.sh <no. threadsOpenMP>
#
# argumentos
#
export OmpMpi=$1
#
# nomes executavel, tarefa, arquivo de saida
#
Exec=`pwd`/Tempo.exe
Job=Tempo
ArqSaida=`pwd`/OutTempo_${OmpMpi}OMP
#
# gera arquivo ssub.sh
#
cat <<EOF0> ssub.sh
#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=${OmpMpi}
#SBATCH -p cpu_dev
#SBATCH -J $Job
#SBATCH --time=00:01:00
#SBATCH --output=$ArqSaida
#SBATCH --exclusive

cd \$SLURM_SUBMIT_DIR
export OMP_NUM_THREADS=${OmpMpi}
srun -n \$SLURM_NTASKS $Exec
EOF0
#
# envia ssub.sh para execucao
#
sbatch ssub.sh
exit


