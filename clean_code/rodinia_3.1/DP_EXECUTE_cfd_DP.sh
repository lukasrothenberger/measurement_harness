cd openmp/cfd 
export TSAN_OPTIONS='ignore_noninstrumented_modules=1'
./euler3d_cpu ../../data/rodinia-3.1-data/data/cfd/fvcorr.domn.097K
#./euler3d_cpu ../../../../../../data/rodinia-3.1-data/data/cfd/fvcorr.domn.193K
#./euler3d_cpu ../../../../../../data/rodinia-3.1-data/data/cfd/missile.domn.0.2M