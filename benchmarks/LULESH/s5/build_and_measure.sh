CC=clang
CXX=clang++
CPU_CC_FLAGS="-fopenmp"
CPU_CXX_FLAGS="-fopenmp"
DP_PATCH_GENERATOR_FLAGS="--log INFO --only-maximum-id-pattern"

####

BASEDIR=$(pwd)

# get original code
rm -rf original_code
cp -r ../../../clean_code/LULESH_SEQ original_code

# get discopop suggestions
if test -d original_build
then
    echo "original_build directory found. Skip generation of suggestions."
else
    echo "Generate DiscoPoP suggestions..."
    cp -r original_code code
    cd code 
    # hotspot profiling
    discopop_hotspot_cmake -DWITH_MPI=Off -DWITH_OPENMP=Off .
    make 
    ./lulesh2.0 -s 5
    ./lulesh2.0 -s 8
    ./lulesh2.0 -s 10
    # discopop profiling
    discopop_cmake -DWITH_MPI=Off -DWITH_OPENMP=Off .
    make 
    ./lulesh2.0 -s 5
    cd .discopop
    hotspot_analyzer
    discopop_explorer --enable-patterns doall,reduction
    discopop_optimizer -v -o1 -p2 --doall-microbench-file $BASEDIR/../../../configuration/doall_1.json --system-configuration $BASEDIR/../../../configuration/cpu_only_system_configuration.json
    discopop_patch_generator -a optimizer/patterns.json ${DP_PATCH_GENERATOR_FLAGS}
    cd $BASEDIR
    mv code original_build
fi

# create sequential code for measurement
cp -r original_code code_sequential

# create modified codes
for d in $(ls original_build/.discopop/patch_generator)
do
    cd $BASEDIR
    CODEDIR="code_$d"
    if test -d ${CODEDIR}
    then
        echo "Skipping creation of modified code ${CODEDIR}"
    else
        echo "Creating modified code ${CODEDIR}"
        cp -r original_code code
        cd original_build/.discopop
        discopop_patch_applicator -a $d -v
        echo "{\"applied\": []}" > patch_applicator/applied_suggestions.json
        cd ../..
        mv code ${CODEDIR}
    fi
done 


# compile modified codes
for d in $(find -maxdepth 1 -name "code_*" -type d) 
do
    echo "compiling modified: $d"
    cd ${BASEDIR}/$d
    if [ -f lulesh2.0 ]
    then
        echo "--> Executable exists. Skipping."
    else
        mkdir build
        cd build 
        cmake -DCMAKE_C_COMPILER=${CC} -DCMAKE_CXX_COMPILER=${CXX} -DCMAKE_C_FLAGS=${CPU_CC_FLAGS} -DCMAKE_CXX_FLAGS=${CPU_CXX_FLAGS} ..
        make -j 8
    fi
done

# execute and measure codes
cd $BASEDIR
rm -f measurements.csv
rm -rf logs
for d in $(find -maxdepth 1 -name "code_*" -type d) 
do 
    cd $BASEDIR
    if [ -f $d/build/lulesh2.0 ]
    then
        echo "Measuring $d/build/lulesh2.0 ..."
        LOGDIR=$BASEDIR/logs/$d
        mkdir -p $LOGDIR
        cd $d/build
        /usr/bin/time --format="$d;%e;%x;" --append --output=$BASEDIR/measurements.csv timeout 300 ./lulesh2.0 -s 12 1>> $LOGDIR/stdout.txt 2>> $LOGDIR/stderr.txt ;
    else
        echo "Executable $d/build/lulesh2.0 does not exist. Skipping."
    fi
    
done