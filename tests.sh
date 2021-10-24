#/bin/bash 

#Parametros de las hormigas
numberAnthill=100
numberAnts=50
e=10
basePenalty=100.0
constantForPenalty=4.0

ACO="${numberAnthill} ${numberAnts} ${e} ${basePenalty} ${constantForPenalty} ${alpha} ${beta}"

#Parametros de la actualizacion de feromona*/
alpha=2.0
beta=5.0
vapor=0.25
c=0.0005
maxPheromone=10.0
minPheromone=0.1

SOLUTIONCREATION="${alpha} ${beta} ${vapor} ${c} ${maxPheromone} ${minPheromone}"

#Parametros de mejora de soluciones*/
limitIteration=50
k=10

LOCALSEARCH="${limitIteration} ${k}"

instanceList="ebl/ebl_original.txt"
seedList="100 70 456 879 950 211"

make

for instance in ${instanceList}; do
    for seed in ${seedList}; do
        ./main ${instance} ${seed}
    done
done

./main ebl/ebl_original.txt

make clean
