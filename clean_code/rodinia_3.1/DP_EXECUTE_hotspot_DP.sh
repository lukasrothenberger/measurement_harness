cd openmp/hotspot
export TSAN_OPTIONS='ignore_noninstrumented_modules=1'
./hotspot 64 64 2 4 ../../data/hotspot/temp_64 ../../data/hotspot/power_64 output.out
