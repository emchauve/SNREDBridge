#!/bin/bash


for run in {740..745} ; do
    sbatch redbridge.sh ${run}
done
