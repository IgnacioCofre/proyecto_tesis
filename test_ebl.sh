#/bin/bash 

dir="ebl"
instPercent="30 40 50 60 70 80 90 100 110 120 130 140 150 160 170 180 190 200"
instanceType="1 2 3 4 5"

seedList="96 235 512 815 921"

#Parametros de las hormigas
numberAnthill=500
numberAnts=100
e=10
basePenalty=100.0
constantForPenalty=4.0

ACO="${numberAnthill} ${numberAnts} ${e} ${basePenalty} ${constantForPenalty}"

#Parametros de la actualizacion de feromona*/
alpha=2.0
beta=5.0
vapor=0.25
c=0.00005
maxPheromone=10.0
minPheromone=0.1

SOLUTIONCREATION="${alpha} ${beta} ${vapor} ${c} ${maxPheromone} ${minPheromone}"

#Parametros de mejora de soluciones*/
limitIteration=100
k=10

LOCALSEARCH="${limitIteration} ${k}"

resultsDir=Results_${dir}_${numberAnthill}_${numberAnts}_${e}_${vapor}_${c}_${limitIteration}_${k}
rm -rf ${resultsDir}
mkdir ${resultsDir}

for percent in ${instPercent}; do
    for type in ${instanceType}; do
        sumarizedOut="Out_${dir}_${percent}_${type}"
        rm -rf ${sumarizedOut}
        best=1000000.0 
        for seed in ${seedList}; do
            detailedOut=out_${dir}_${percent}_${type}_${seed}_${numberAnthill}_${numberAnts}_${e}_${limitIteration}.csv
            rm -rf ${detailedOut}
            echo "./main ${dir}/${dir}_${percent}_${type}.txt ${seed} ${ACO} ${SOLUTIONCREATION} ${LOCALSEARCH} > ${detailedOut}"
            ./main ${dir}/${dir}_${percent}_${type}.txt ${seed} ${ACO} ${SOLUTIONCREATION} ${LOCALSEARCH} > ${detailedOut}
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
done    