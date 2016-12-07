#!/bin/bash
#SBATCH --nodes=1
#SBATCH --cpus-per-task=10
#SBATCH --output=hw92.txt 
nproc=$(expr $SLURM_JOB_CPUS_PER_NODE \* $SLURM_NNODES)
echo $nproc cores available
cp hw92.c $PFSDIR/.
cd $PFSDIR
module load gcc
gcc hw92.c -o hw92 -fopenmp -lm
./hw92