rm -f measurements.csv
rm -rf buffer
rm -rf logs
rm -rf working_copy

HOME_DIR=$(pwd)

# iterate over all benchmarks and clean the environment
for bm in */ ; do
    echo "Cleaning benchmark environment: ${bm}"
    cd $bm
    ./clean.sh
    cd $HOME_DIR
done