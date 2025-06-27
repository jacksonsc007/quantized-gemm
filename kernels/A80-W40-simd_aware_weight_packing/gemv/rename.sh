for file in *.c; do
    # echo $file | awk -F "." '{print $1}' | xargs -I {} mv {}.c {}.cpp
    mv "$file" "${file%.c}.cpp"
done