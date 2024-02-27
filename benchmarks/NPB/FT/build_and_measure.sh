DP_PATCH_GENERATOR_FLAGS="--log INFO"
#  --only-maximum-id-pattern"

BASEDIR=$(pwd)

# get original code and clean environment
rm -rf original_code
rm -rf code
cp -r ../../../clean_code/NPB-SER original_code

# get discopop suggestions
if test -d original_build
then
    echo "original_build directory found. Skip generation of suggestions."
else
    echo "Generate DiscoPoP suggestions..."
    cp -r original_code code
    cd code
    # hotspot detection
#    CC=discopop_hotspot_cc make
#    ./cohmm 1
#    cd .discopop
#    hotspot_analyzer
#    cd ..
#    make clean
    # discopop pattern detection
    
    DOT_DISCOPOP=$BASEDIR/code/bin/.discopop CXX=discopop_cxx CC=discopop_cc make ft CLASS=S
    
    cd bin
    ./ft.S

    cd .discopop
    discopop_explorer --enable-patterns doall,reduction
    discopop_optimizer -v -s --doall-microbench-file $BASEDIR/../../../configuration/doall_1.json --system-configuration $BASEDIR/../../../configuration/cpu_only_system_configuration.json
    discopop_patch_generator -a optimizer/patterns.json ${DP_PATCH_GENERATOR_FLAGS}
    cd $BASEDIR
    mv code original_build
fi

# create sequential code for measurement
cp -r original_code code_sequential

# create modified codes
for d in $(ls original_build/bin/.discopop/patch_generator)
do
    cd $BASEDIR
    CODEDIR="code_$d"
    if test -d ${CODEDIR}
    then
        echo "Skipping creation of modified code ${CODEDIR}"
    else
        echo "Creating modified code ${CODEDIR}"
        cp -r original_code code
        cd original_build/bin/.discopop
        discopop_patch_applicator -a $d -v
        echo "{\"applied\": []}" > patch_applicator/applied_suggestions.json
        cd $BASEDIR
        mv code ${CODEDIR}
    fi
done 

# compile modified codes
for d in $(find -maxdepth 1 -name "code_*" -type d) 
do
    echo "compiling modified: $d"
    cd ${BASEDIR}/$d
    if [ -f bin/ft.B ]
    then
        echo "--> Executable exists. Skipping."
    else
        CC=clang++ CXX=clang++ CC_FLAGS=-fopenmp make ft CLASS=B
    fi
done

# execute and measure codes
cd $BASEDIR
rm -f measurements.csv
rm -rf logs
for d in $(find -maxdepth 1 -name "code_*" -type d) 
do 
    cd $BASEDIR
    if [ -f $d/bin/ft.B ]
    then
        echo "Measuring $d/bin/ft.B ..."
        LOGDIR=$BASEDIR/logs/$d
        mkdir -p $LOGDIR
        cd $d/bin
        /usr/bin/time --format="$d;%e;%x;" --append --output=$BASEDIR/measurements.csv timeout 60 ./ft.B 1>> $LOGDIR/stdout.txt 2>> $LOGDIR/stderr.txt ;
    else
        echo "Executable $d/bin/ft.B does not exist. Skipping."
    fi
    
done