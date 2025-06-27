for file in *.cpp; do
    mv $file ${file/gemm-/}
done
