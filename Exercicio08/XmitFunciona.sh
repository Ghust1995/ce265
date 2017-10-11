#!/bin/bash
# 
# script para executar Funciona.exe em MPI no Santos Dumont
#
# uso: XmitFunciona.sh RanksMPI
#    onde:
#    RanksMPI:  numero de processos MPI (inteiro de 1 a 6)
#
# argumentos
#
export RanksMPI=$1
if ((${RanksMPI} > 6))
then {
echo "SCRIPT ERROR: first argument (MPI ranks) > 6 is an error"
exit
}
fi
if ((${RanksMPI} < 1))
then {
echo "SCRIPT ERROR: first argument (MPI ranks) < 1 is an error"
exit
}
fi
#
# nomes executavel, tarefa, arquivo de saida
#
DirBase=`pwd`
Exec=${DirBase}/Funciona.exe
Job=Func_${RanksMPI}
ArqSaida=${DirBase}/SolFunc_${RanksMPI}MPI.out
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
