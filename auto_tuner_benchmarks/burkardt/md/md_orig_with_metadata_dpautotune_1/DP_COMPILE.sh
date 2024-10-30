rm -rf dp_autotuner_build
mkdir dp_autotuner_build
cd dp_autotuner_build
cmake -DCMAKE_CXX_FLAGS="-fopenmp -fsanitize=thread -g" ..
make