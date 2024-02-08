DP_PATCH_GENERATOR_FLAGS="--log INFO"
#  --only-optimizer-output-patterns"

BASEDIR=$(pwd)

# get original code and clean environment
rm -rf original_code
rm -rf code
cp -r ../../clean_code/CoMD original_code


# get discopop suggestions
if test -d original_build
then
    echo "original_build directory found. Skip generation of suggestions."
else
    echo "Generate DiscoPoP suggestions..."
    cp -r original_code code
    cd code 
    # hotspot detection
#    cd src-mpi
#    cp Makefile.hotspot Makefile
#    DOT_DISCOPOP=$(pwd)/../bin/.discopop make
#    make clean
#    rm Makefile
#    cd ../bin
#    ./CoMD-serial --nx 3 --ny 3 --nz 3
#    ./CoMD-serial --nx 4 --ny 4 --nz 4
#    cd .discopop
#    hotspot_analyzer
#    cd ../..
    # discopop pattern detection
    cd src-mpi
    cp Makefile.discopop Makefile
    DOT_DISCOPOP=$(pwd)/../bin/.discopop make
    rm Makefile
    cd ../bin
    ./CoMD-serial --nx 3 --ny 3 --nz 3
    cd .discopop
    discopop_explorer --enable-patterns doall,reduction
    discopop_optimizer -v -o1 -p2 --doall-microbench-file $BASEDIR/../../configuration/doall_1.json --system-configuration $BASEDIR/../../configuration/prefer_gpu_system_configuration.json -v
    discopop_patch_generator -a optimizer/patterns.json ${DP_PATCH_GENERATOR_FLAGS}
    cd $BASEDIR
    mv code original_build
fi

# create sequential code for measurement
rm -rf code_sequential
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
    cd ${BASEDIR}/$d/src-mpi
    if [ -f cohmm ]
    then
        echo "--> Executable exists. Skipping."
    else    
        cp Makefile.openmp Makefile
        make
        rm Makefile
    fi
done

# execute and measure codes
cd $BASEDIR
rm -f measurements.csv
rm -rf logs
for d in $(find -maxdepth 1 -name "code_*" -type d) 
do 
    cd $BASEDIR
    if [ -f $d/bin/CoMD-serial ]
    then
        echo "Measuring $d/bin/CoMD-serial ..."
        LOGDIR=$BASEDIR/logs/$d
        mkdir -p $LOGDIR
        cd $d/bin
        /usr/bin/time --format="$d;%e;%x;" --append --output=$BASEDIR/measurements.csv timeout 120 ./CoMD-serial --nx 4 --ny 4 --nz 4 1>> $LOGDIR/stdout.txt 2>> $LOGDIR/stderr.txt ;
    else
        echo "Executable $d/bin/CoMD-serial does not exist. Skipping."
    fi
    
done
