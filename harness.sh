 HOME_DIR=$(pwd)
### SETTINGS ###
DISCOPOP_PATH=/home/lukas/git/discopop
DISCOPOP_BUILD=$DISCOPOP_PATH/build
PROJECT_PATH=/home/lukas/Schreibtisch/Example_Code/daxpy
PROJECT_BUILD_PATH=$PROJECT_PATH/build
################

### DEPENDENT SETTINGS
PROJECT_DP_FOLDER=$PROJECT_PATH/.discopop
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
cp -r $PROJECT_PATH $HOME_DIR/code_copy


# clean environment
rm -rf logs
mkdir logs
rm -f measurements.csv
echo "suggestion_id;time;exit_code;" >> measurements.csv

# get baseline measurement
# prepare output
    mkdir $HOME_DIR/logs/baseline
    # clean environment
    cd $PROJECT_PATH
    make clean 
    rm -rf $PROJECT_BUILD_PATH
    # compile program
    mkdir $PROJECT_BUILD_PATH
    cd $PROJECT_BUILD_PATH
    cmake ..
    make -j15 1>> $HOME_DIR/logs/baseline/log.txt 2>> $HOME_DIR/logs/baseline/log.txt
    # execute program
    COMMAND="./prog"
    /usr/bin/time --format="baseline;%e;%x;" --append --output=$HOME_DIR/measurements.csv $COMMAND 1>> $HOME_DIR/logs/baseline/stdout.txt 2>> $HOME_DIR/logs/baseline/stderr.txt

    # restore original state
    cp -r $HOME_DIR/code_copy $PROJECT_PATH



# iterate over all identified suggestions
cd $PATCH_GENERATOR_FOLDER
for d in * ; do
    echo "CURRENT SUGGESTION: ${d}"
    # prepare output
    mkdir $HOME_DIR/logs/$d

    # clean environment
    cd $PROJECT_PATH
    make clean 
    rm -rf $PROJECT_BUILD_PATH

    # apply suggestion
    cd $PROJECT_DP_FOLDER
    discopop_patch_applicator -a ${d} -v >> $HOME_DIR/logs/$d/log.txt

    # compile program
    mkdir $PROJECT_BUILD_PATH
    cd $PROJECT_BUILD_PATH
    cmake .. -DCMAKE_C_FLAGS="-fopenmp" -DCMAKE_CXX_FLAGS="-fopenmp"
    make -j 15 1>> $HOME_DIR/logs/$d/log.txt 2>> $HOME_DIR/logs/$d/log.txt

    # execute program
    /usr/bin/time --format="${d};%e;%x;" --append --output=$HOME_DIR/measurements.csv $COMMAND 1>> $HOME_DIR/logs/$d/stdout.txt 2>>$HOME_DIR/logs/$d/stderr.txt

    # rollback suggestions
    cd $PROJECT_DP_FOLDER
    discopop_patch_applicator -C -v >> $HOME_DIR/logs/$d/log.txt

    # restore original state
    cp -r $HOME_DIR/code_copy $PROJECT_PATH
done

# delete code copy 
# rm -r $HOME_DIR/code_copy

