#!/bin/bash
# 
# script para executar versao sequencial de Crivo.exe no Santos Dumont
#
# uso: Xmit.sh Ultimo 
#    onde Ultimo = limite superior da sequencia de inteiros (long)
#
# argumentos
#
export Ultimo=$1
#
# nomes executavel, tarefa, arquivo de saida
#
DirBase=`pwd`
Exec=${DirBase}/Crivo.exe
Job=C_Seq
ArqSaida=${DirBase}/Out_Crivo
#
# gera arquivo ssub.sh
#
cat <<EOF0> ${DirBase}/ssub.sh
#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH -p cpu_dev
#SBATCH -J $Job
#SBATCH --time=00:02:00
#SBATCH --output=${ArqSaida}
#SBATCH --exclusive

cd \$SLURM_SUBMIT_DIR
ulimit -s unlimited
export OMP_NUM_THREADS=1
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
