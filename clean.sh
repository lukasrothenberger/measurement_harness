 HOME_DIR=$(pwd)
 BENCHMARKS_DIR=$HOME_DIR/benchmarks

# iterate over all benchmarks and clean the environment
cd $BENCHMARKS_DIR
for bm in * ; do
    echo "Cleaning benchmark environment: ${bm}"
    cd $bm
    ./clean.sh
    cd $BENCHMARKS_DIR
done