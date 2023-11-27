 HOME_DIR=$(pwd)

# iterate over all benchmarks and clean the environment
cd $HOME_DIR
for bm in */ ; do
    echo "Cleaning benchmark environment: ${bm}"
    cd $bm
    ./clean.sh
    cd $HOME_DIR
done

# iterate over all benchmarks and execute the included harness
cd $HOME_DIR
for bm in */ ; do
    echo "Executing benchmark: ${bm}"
    cd $bm
    ./cpu_harness.sh
    cd $HOME_DIR
done

