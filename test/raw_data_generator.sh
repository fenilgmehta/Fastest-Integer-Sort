#!/usr/bin/env bash
# $1: name of the object file to be used
# $2: folder location to write the data generated

object_file=$1

if [ ! -d $2 ]; then
    mkdir -p $2
    echo "Folder created"
fi

cd "${2}"
output_folder=`pwd`
cd - > /dev/null

echo "Object file name       : \"${object_file}\""
echo "Output folder location : \"${output_folder}\""

file_name="data_generator_integer.cpp"
echo "Compiling \"${file_name}\""
g++ "data_generator_integer.cpp" -o "${1}" || exit
echo 'Compilation successfully completed' && echo

i=10
test_cases=5000

echo "test_cases = ${test_cases}" && echo

while [[ i -le 100000 ]]
do
    echo "Array size = ${i}"
    ./${object_file} <<< "$i $test_cases" > "${output_folder}/rawData_${test_cases}_${i}.csv"
    # ./${1} <<< "$i $test_cases" > "../Graphs/All_Comparisons/rawData_int8_t/rawData_${test_cases}_${i}.csv"
    ((i = i * 10))
done
