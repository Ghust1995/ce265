#!/bin/bash
# 
# script para executar versao OpenMP de Crivo.exe no Santos Dumont
#
# uso: Xmit.sh Ultimo OmpMpi
#    onde Ultimo = limite superior da sequencia de inteiros (long)
#    onde OmpMpi = numero de threads OpenMP (inteiro de 1 a 16)
#
# argumentos
#
export Ultimo=$1
export OmpMpi=$2
#
# nomes executavel, tarefa, arquivo de saida
#
DirBase=`pwd`
Exec=${DirBase}/Crivo.exe
Job=C_${OmpMpi}
ArqSaida=${DirBase}/Out_Crivo_${OmpMpi}_OMP
#
# gera arquivo ssub.sh
#
cat <<EOF0> ${DirBase}/ssub.sh
#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=${OmpMpi}
#SBATCH -p cpu_dev
#SBATCH -J $Job
#SBATCH --time=00:02:00
#SBATCH --output=${ArqSaida}
#SBATCH --exclusive

cd \$SLURM_SUBMIT_DIR
ulimit -s unlimited
export OMP_NUM_THREADS=${OmpMpi}
srun -n \$SLURM_NTASKS ${Exec} ${Ultimo}
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
