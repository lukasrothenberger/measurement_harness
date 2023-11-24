 HOME_DIR=$(pwd)
### SETTINGS ###
DISCOPOP_PATH=/home/lukasrothenberger/git/discopop
DISCOPOP_BUILD=$DISCOPOP_PATH/build
PROJECT_PATH=/home/lukasrothenberger/code/benchmarks/daxpy
PROJECT_BUILD_PATH=$PROJECT_PATH
################

### DEPENDENT SETTINGS
PROJECT_DP_FOLDER=$PROJECT_BUILD_PATH/.discopop
PATCH_GENERATOR_FOLDER=$PROJECT_DP_FOLDER/patch_generator
################

# Check prerequisites
# check if .discopop folder exists
if [ -d $PROJECT_DP_FOLDER ]; then
    echo "Directory exists."
else
    echo "Directory not found: ${PROJECT_DP_FOLDER}"
    exit 1
fi
# check if .discopop/patch_generator folder exists
if [ -d $PATCH_GENERATOR_FOLDER ]; then
    echo "Directory exists."
else
    echo "Directory not found: ${PATCH_GENERATOR_FOLDER}"
    exit 1
fi

# create code copy 
mkdir -p $HOME_DIR/code_copy
cp -r $PROJECT_PATH/* $HOME_DIR/code_copy

# for easier browsing of the results, copy the identified patches
mkdir -p $HOME_DIR/patch_generator_output
cp -r $PATCH_GENERATOR_FOLDER/* $HOME_DIR/patch_generator_output


# clean environment
rm -rf logs
mkdir logs
rm -f measurements.csv
echo "suggestion_id;time;exit_code;" >> measurements.csv

# get baseline measurement
# prepare output
    mkdir $HOME_DIR/logs/baseline
    # clean environment
    cd $PROJECT_BUILD_PATH
    make -f Makefile.discopop clean
    # compile program
    cmake -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ .
    make -j15 1>> $HOME_DIR/logs/baseline/log.txt 2>> $HOME_DIR/logs/baseline/log.txt
    # execute program
    COMMAND="timeout 120 ./prog"
    /usr/bin/time --format="baseline;%e;%x;" --append --output=$HOME_DIR/measurements.csv $COMMAND 1>> $HOME_DIR/logs/baseline/stdout.txt 2>> $HOME_DIR/logs/baseline/stderr.txt

    # restore original state
    rm -r $PROJECT_PATH/*
    cp -r $HOME_DIR/code_copy/* $PROJECT_PATH



# iterate over all identified suggestions
cd $PATCH_GENERATOR_FOLDER
for d in * ; do
    echo "CURRENT SUGGESTION: ${d}"
    # prepare output
    mkdir $HOME_DIR/logs/$d

    # clean environment
    cd $PROJECT_BUILD_PATH
    make -f Makefile.discopop clean

    # apply suggestion
    cd $PROJECT_DP_FOLDER
    discopop_patch_applicator -a ${d} -v >> $HOME_DIR/logs/$d/log.txt

    # compile program
    cd $PROJECT_BUILD_PATH
    cmake . -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_FLAGS="-fopenmp -fopenmp-targets=nvptx64" -DCMAKE_CXX_FLAGS="-fopenmp -fopenmp-targets=nvptx64"
    make -j 15 1>> $HOME_DIR/logs/$d/log.txt 2>> $HOME_DIR/logs/$d/log.txt

    # execute program
    /usr/bin/time --format="${d};%e;%x;" --append --output=$HOME_DIR/measurements.csv $COMMAND 1>> $HOME_DIR/logs/$d/stdout.txt 2>>$HOME_DIR/logs/$d/stderr.txt

    # rollback suggestions
    cd $PROJECT_DP_FOLDER
    discopop_patch_applicator -C -v >> $HOME_DIR/logs/$d/log.txt

    # restore original state
    rm -r $PROJECT_PATH/*
    cp -r $HOME_DIR/code_copy/* $PROJECT_PATH
done

# delete code copy 
#rm -r $HOME_DIR/code_copy

