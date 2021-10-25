#/bin/bash 

#instanceList="ebl/ebl_40_1.txt ebl/ebl_40_2.txt ebl/ebl_40_3.txt ebl/ebl_40_4.txt ebl/ebl_40_5.txt"
dir="lars"
instanceList="lars_30_1.txt lars_30_2.txt lars_30_3.txt lars_30_4.txt lars_30_5.txt"
seedList="96 235 512 815 921"

#Parametros de las hormigas
numberAnthill=10
numberAnts=50
e=10
basePenalty=100.0
constantForPenalty=4.0

ACO="${numberAnthill} ${numberAnts} ${e} ${basePenalty} ${constantForPenalty}"

#Parametros de la actualizacion de feromona*/
alpha=2.0
beta=5.0
vapor=0.25
c=0.0001
maxPheromone=10.0
minPheromone=0.1

SOLUTIONCREATION="${alpha} ${beta} ${vapor} ${c} ${maxPheromone} ${minPheromone}"

#Parametros de mejora de soluciones*/
limitIteration=50
k=10

LOCALSEARCH="${limitIteration} ${k}"

resultsDir=Results_${dir}_${numberAnthill}_${numberAnts}_${e}_${vapor}_${c}_${limitIteration}_${k}
rm -rf ${resultsDir}
mkdir ${resultsDir}

make

for instance in ${instanceList}; do
    sumarizedOut="Out_${instance}"
    rm -rf ${sumarizedOut}
    best=1000000.0 
    for seed in ${seedList}; do
        detailedOut=out_${instance}_${seed}_${numberAnthill}_${numberAnts}_${e}_${limitIteration}.out
        rm -rf ${detailedOut}
        echo "./main ${dir}/${instance} ${seed} ${ACO} ${SOLUTIONCREATION} ${LOCALSEARCH} > ${detailedOut}"
        ./main ${dir}/${instance} ${seed} ${ACO} ${SOLUTIONCREATION} ${LOCALSEARCH} > ${detailedOut}
        quality=`tail -1 ${detailedOut} |awk -F ' ' '{print $3}'`
        if (( $(echo "${quality} < ${best}" |bc ) )); then 
           best=${quality}
        fi
        quality=`tail -1 ${detailedOut}`
        echo "${quality}" >> ${sumarizedOut}
        mv ${detailedOut} ${resultsDir}
    done
    echo "best: ${best}" >> ${sumarizedOut}
    mv ${sumarizedOut} ${resultsDir}
done

make clean
