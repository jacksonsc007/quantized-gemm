set -e
set -u

n_threads=$1

rm -rf build
cmake -B build -DNTHREADS=$n_threads -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build -j
# build/gemv