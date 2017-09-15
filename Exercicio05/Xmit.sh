#!/bin/bash
# 
# script para executar Vida na GPU no SDumont
#
# uso: Xmit.sh ThreadsPerBlock NumberOfBlocks
#
#
#
# script arguments
#
export TB=$1
export NB=$2
#
# hadware script arguments
#
BaseRunName=Vida
ExecutableName=Vida.exe
#
# job name 
#
RunName=V_${TB}X${NB}
OutName=${BaseRunName}_${TB}X${NB}.out
#
# directories
# executable full path
#
DirBase=`pwd`
export Exec=${DirBase}/${ExecutableName}

#
# starts producing queue script file qsub.sh
#
cat <<EOF0> ${DirBase}/ssub.sh
#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH -p nvidia_dev
#SBATCH -J ${RunName}
#SBATCH --time=00:02:00
#SBATCH --output=${OutName}
#SBATCH --exclusive

cd \$SLURM_SUBMIT_DIR
ulimit -s unlimited
export OMP_NUM_THREADS=1
srun -n \$SLURM_NTASKS ${Exec} ${TB} ${NB}
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
