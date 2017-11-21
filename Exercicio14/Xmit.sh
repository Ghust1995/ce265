#!/bin/bash
# 
# script to run Cholesky.exe on SantosDumont
#
# usage: Xmit.sh <threadsOMP>
#
#
#
# script arguments
#
export TotOmp=$1
#
# hadware script arguments
#
BaseRunName=Cholesky
ExecutableName=${BaseRunName}.exe
#
# job name 
#
RunName=Cho_${TotOmp}
OutName=Cho_${TotOmp}.out
#
# directories
# executable full path
#
DirBase=`pwd`
export Exec=${DirBase}/${ExecutableName}

#
# starts producing queue script file qsub.sh
#
cat <<EOF0> ${DirBase}/qsub.sh
#!/bin/bash
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=${TotOmp}
#SBATCH -p cpu_dev
#SBATCH -J $RunName
#SBATCH --time=00:20:00
#SBATCH --output=${OutName}
#SBATCH --exclusive

cd \$SLURM_SUBMIT_DIR
ulimit -s unlimited
export OMP_NUM_THREADS=${TotOmp}
srun -n \$SLURM_NTASKS ${Exec}
EOF0
#
# finishes producing file qsub.sh and moves to executable directory
#
chmod +x ${DirBase}/qsub.sh
cd ${DirBase}
#
# finishes producing file qsub.sh 
#
chmod +x ${DirBase}/qsub.sh
#
# qsub with variable # PEs per node
#
sbatch qsub.sh

exit
