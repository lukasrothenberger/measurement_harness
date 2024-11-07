cd openmp/nn
export TSAN_OPTIONS='ignore_noninstrumented_modules=1'
./nn filelist_4 5 30 90