DP_PATCH_GENERATOR_FLAGS="--log INFO"
# --only-maximum-id-pattern"
#  --only-optimizer-output-patterns"

BASEDIR=$(pwd)

# get original code and clean environment
rm -rf original_code
rm -rf code
cp -r ../../clean_code/miniFE/ref original_code

# get discopop suggestions
if test -d original_build
then
    echo "original_build directory found. Skip generation of suggestions."
else
    echo "Generate DiscoPoP suggestions..."
    cp -r original_code code
    cd code 
    # discopop hotspot detection
    cd src 
    ls 
    
    make -f Makefile.discopop.hotspots
    
    ./miniFE.x -nx 10 -ny 10 -nz 10
    ./miniFE.x -nx 25 -ny 25 -nz 25
#    ./miniFE.x -nx 40 -ny 40 -nz 40
    cd .discopop
    hotspot_analyzer
    cd ..
    make -f Makefile.discopop.hotspots clean
    cd ..
    # discopop pattern detection
    cd src
    make -f Makefile.discopop.patterns
    ./miniFE.x
    cd .discopop
    discopop_explorer --enable-patterns doall,reduction --log INFO
#    discopop_optimizer -v -p2 -o1 -s --doall-microbench-file $BASEDIR/../../configuration/doall_1.json --system-configuration $BASEDIR/../../configuration/cpu_only_system_configuration.json 
#    discopop_patch_generator -a optimizer/patterns.json ${DP_PATCH_GENERATOR_FLAGS}
    discopop_patch_generator ${DP_PATCH_GENERATOR_FLAGS}
    cd $BASEDIR
    mv code original_build
fi

# create sequential code for measurement
rm -rf code_sequential
cp -r original_code code_sequential

# create modified codes
for d in $(ls original_build/src/.discopop/patch_generator)
do
    cd $BASEDIR
    CODEDIR="code_$d"
    if test -d ${CODEDIR}
    then
        echo "Skipping creation of modified code ${CODEDIR}"
    else
        echo "Creating modified code ${CODEDIR}"
        cp -r original_code code
        cd original_build/src/.discopop
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
    cd ${BASEDIR}/$d/src
    if [ -f miniFE.x ]
    then
        echo "--> Executable exists. Skipping."
    else    
        make -f Makefile.discopop.openmp -j 14
    fi
done

# execute and measure codes
cd $BASEDIR
rm -f measurements.csv
rm -rf logs
for d in $(find -maxdepth 1 -name "code_*" -type d) 
do 
    cd $BASEDIR
    if [ -f $d/src/miniFE.x ]
    then
        echo "Measuring $d/src/miniFE.x ..."
        LOGDIR=$BASEDIR/logs/$d
        mkdir -p $LOGDIR
        cd $d/src
        /usr/bin/time --format="$d;%e;%x;" --append --output=$BASEDIR/measurements.csv timeout 60 ./miniFE.x -nx 160 -ny 160 -nz 160 1>> $LOGDIR/stdout.txt 2>> $LOGDIR/stderr.txt ;
    else
        echo "Executable $d/src/miniFE.x does not exist. Skipping."
    fi
    
done
