HOME_DIR=$(pwd)
### SETTINGS ###
DISCOPOP_PATH=/home/lukas/git/discopop
DISCOPOP_BUILD=$DISCOPOP_PATH/build
PROJECT_PATH=/home/lukas/git/Benchmarks/untested/miniFE/ref/src
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

# clean environment
rm -rf logs
mkdir logs
rm -f measurements.txt
echo "suggestion_id;time;exit_code;" >> measurements.txt

# get baseline measurement
# prepare output
    mkdir $HOME_DIR/logs/baseline
    # clean environment
    cd $PROJECT_PATH
    make clean 
    # compile program
    make -f Makefile.gnu.serial 1> $HOME_DIR/logs/baseline/log.txt 2> $HOME_DIR/logs/baseline/log.txt
    # execute program
    COMMAND="./miniFE.x --nx 40 --ny 40 --nz 40"
    /usr/bin/time --format="baseline;%e;%x;" --append --output=$HOME_DIR/measurements.txt $COMMAND 1>> $HOME_DIR/logs/baseline/stdout.txt 2>> $HOME_DIR/logs/baseline/stderr.txt



# iterate over all identified suggestions
cd $PATCH_GENERATOR_FOLDER
for d in * ; do
    echo "CURRENT SUGGESTION: ${d}"
    # prepare output
    mkdir $HOME_DIR/logs/$d

    # clean environment
    cd $PROJECT_PATH
    make clean 

    # apply suggestion
    cd $PROJECT_DP_FOLDER
    discopop_patch_applicator -a ${d} -v >> $HOME_DIR/logs/$d/log.txt

    # compile program
    cd $PROJECT_PATH
    make -f Makefile.discopop.openmp 1> $HOME_DIR/logs/$d/log.txt 2> $HOME_DIR/logs/$d/log.txt

    # execute program
    /usr/bin/time --format="${d};%e;%x;" --append --output=$HOME_DIR/measurements.txt $COMMAND 1>> $HOME_DIR/logs/$d/stdout.txt 2>>$HOME_DIR/logs/$d/stderr.txt

    # rollback suggestions
    cd $PROJECT_DP_FOLDER
    discopop_patch_applicator -C -v >> $HOME_DIR/logs/$d/log.txt
done

