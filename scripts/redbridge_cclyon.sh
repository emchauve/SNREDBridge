#!/bin/bash

RUN=${1}
run=${RUN}

BASE_OUTPUT_PATH="/sps/nemo/scratch/golivier/Commissioning/test_redbridge/"
OUTPUT_RUN_PATH="/sps/nemo/scratch/golivier/Commissioning/test_redbridge/snemo_run-${run}"

#SBATCH --account nemo
#SBATCH --job-name run_RUN
#SBATCH --licenses sps
#SBATCH --mem 1024M
#SBATCH --time 02:00:00
#SBATCH --output ${OUTPUT_RUN_PATH}/process-run-RUN.log


# Personal environment, loading all softwares
load_mysnemo_dev_setup

# for run in {730..735} ; do

NUMBER_OF_EVENTS=100000000

SNFEE_RTD2RED_PATH="/sps/nemo/scratch/golivier/software/SNFEE/install.d/bin"
SNFEE_RTD2RED_SOFT="${SNFEE_RTD2RED_PATH}/snfee-rtd2red"
SNREDBRIDGE_PATH="/sps/nemo/scratch/golivier/software/SNREDBridge/install.d/bin/"
SNREDBRIDGE_SOFT="${SNREDBRIDGE_PATH}/red_bridge"
SNREDBRIDGE_VALIDATION_SOFT="${SNREDBRIDGE_PATH}/red_bridge_validation"

RTD_PATH="/sps/nemo/snemo/snemo_data/raw_data/v1/RTD/run_${run}/output_data.d"
RTD_FILE="${RTD_PATH}/snemo_run-${run}_rtd.data.gz"

echo "RTD_PATH=${RTD_PATH}"
echo "RTD_FILEATH=${RTD_FILE}"


ls ${RTD_FILE} -lh
if [ $? -eq 0 ]; then
	mkdir -p ${OUTPUT_RUN_PATH}
    mkdir -p ${OUTPUT_RUN_PATH}/delta-tdc-10us
    mkdir -p ${OUTPUT_RUN_PATH}/soft-trigger
fi
RTD2RED_OUTPUT_DELTATDC="${OUTPUT_RUN_PATH}/delta-tdc-10us"
RTD2RED_OUTPUT_SOFTTRIGGER="${OUTPUT_RUN_PATH}/soft-trigger"

RTD2RED_BASE_CONFIG_DELTATDC_FILE="${BASE_OUTPUT_PATH}/rtd2red-RUN_deltatdc.config"
RTD2RED_BASE_CONFIG_SOFTTRIGGER_FILE="${BASE_OUTPUT_PATH}/rtd2red-RUN_softtrigger.config"

RTD2RED_RUN_CONFIG_DELTATDC_FILE="${RTD2RED_OUTPUT_DELTATDC}/rtd2red-${run}.config"
RTD2RED_RUN_CONFIG_SOFTTRIGGER_FILE="${RTD2RED_OUTPUT_SOFTTRIGGER}/rtd2red-${run}.config"

RTD2RED_DELTATDC_LOG_FILE="${RTD2RED_OUTPUT_DELTATDC}/snemo_run-${run}_rtd2red.log"
RTD2RED_SOFTTRIGGER_LOG_FILE="${RTD2RED_OUTPUT_SOFTTRIGGER}/snemo_run-${run}_rtd2red.log"

ls ${RTD_FILE} -lh
if [ $? -eq 0 ]; then
    cat ${RTD2RED_BASE_CONFIG_DELTATDC_FILE} | sed "s/RUN/${run}/g" > "${RTD2RED_RUN_CONFIG_DELTATDC_FILE}"
    cat ${RTD2RED_BASE_CONFIG_SOFTTRIGGER_FILE} | sed "s/RUN/${run}/g" > "${RTD2RED_RUN_CONFIG_SOFTTRIGGER_FILE}"
    echo "INFO: Launch SNFEE-RTD2RED for DELTATDC algorithm"
    snfee-rtd2red -c ${RTD2RED_RUN_CONFIG_DELTATDC_FILE} > "${RTD2RED_DELTATDC_LOG_FILE}" 2>&1

    echo "INFO: Launch SNFEE-RTD2RED for SOFT TRIGGER algorithm"
    snfee-rtd2red -c ${RTD2RED_RUN_CONFIG_SOFTTRIGGER_FILE} > "${RTD2RED_SOFTTRIGGER_LOG_FILE}" 2>&1
else
    echo
    echo "*** skipping run $run ***"
    echo
fi

RED_DELTATDC_FILE="${RTD2RED_OUTPUT_DELTATDC}/snemo_run-${run}_red.data.gz"
RED_SOFTTRIGGER_FILE="${RTD2RED_OUTPUT_SOFTTRIGGER}/snemo_run-${run}_red.data.gz"

# echo "Touching RED files for debug purpose"
# touch ${RED_DELTATDC_FILE}
# touch ${RED_SOFTTRIGGER_FILE}

UDD_DELTATDC_FILE="${RTD2RED_OUTPUT_DELTATDC}/snemo_run-${run}_udd.brio"
UDD_SOFTTRIGGER_FILE="${RTD2RED_OUTPUT_SOFTTRIGGER}/snemo_run-${run}_udd.brio"

REDBRIDGE_DELTATDC_LOG_FILE="${RTD2RED_OUTPUT_DELTATDC}/snemo_run-${run}_redbridge.log"
REDBRIDGE_VALIDATION_DELTATDC_LOG_FILE="${RTD2RED_OUTPUT_DELTATDC}/snemo_run-${run}_redbridge_validation.log"

REDBRIDGE_SOFTTRIGGER_LOG_FILE="${RTD2RED_OUTPUT_SOFTTRIGGER}/snemo_run-${run}_redbridge.log"
REDBRIDGE_VALIDATION_SOFTTRIGGER_LOG_FILE="${RTD2RED_OUTPUT_SOFTTRIGGER}/snemo_run-${run}_redbridge_validation.log"


ls ${RED_DELTATDC_FILE} -lh
if [ $? -eq 0 ]; then
    ${SNREDBRIDGE_SOFT} -i ${RED_DELTATDC_FILE} -o ${UDD_DELTATDC_FILE} -n ${NUMBER_OF_EVENTS} > "${REDBRIDGE_DELTATDC_LOG_FILE}" 2>&1
fi

ls ${RED_SOFTTRIGGER_FILE} -lh
if [ $? -eq 0 ]; then
    ${SNREDBRIDGE_SOFT} -i ${RED_SOFTTRIGGER_FILE} -o ${UDD_SOFTTRIGGER_FILE} -n ${NUMBER_OF_EVENTS} > "${REDBRIDGE_SOFTTRIGGER_LOG_FILE}" 2>&1
fi

# echo "Touching UDD files for debug purpose"
# touch ${UDD_DELTATDC_FILE}
# touch ${UDD_SOFTTRIGGER_FILE}


ls ${UDD_DELTATDC_FILE} -lh
if [ $? -eq 0 ]; then
    ${SNREDBRIDGE_VALIDATION_SOFT} -ired ${RED_DELTATDC_FILE} -iudd ${UDD_DELTATDC_FILE} -n ${NUMBER_OF_EVENTS} > "${REDBRIDGE_VALIDATION_DELTATDC_LOG_FILE}" 2>&1
fi

ls ${UDD_SOFTTRIGGER_FILE} -lh
if [ $? -eq 0 ]; then
    ${SNREDBRIDGE_VALIDATION_SOFT} -ired ${RED_SOFTTRIGGER_FILE} -iudd ${UDD_SOFTTRIGGER_FILE} -n ${NUMBER_OF_EVENTS} > "${REDBRIDGE_VALIDATION_SOFTTRIGGER_LOG_FILE}" 2>&1
fi
