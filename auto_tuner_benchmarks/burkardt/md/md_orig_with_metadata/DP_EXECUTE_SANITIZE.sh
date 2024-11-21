cd dp_autotuner_build
export TSAN_OPTIONS='ignore_noninstrumented_modules=1'
./prog 3 40 40
return $?
