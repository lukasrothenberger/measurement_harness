cd openmp/kmeans
export TSAN_OPTIONS='ignore_noninstrumented_modules=1'
./kmeans_openmp/kmeans -n $(nproc --all) -i ../../data/kmeans/100