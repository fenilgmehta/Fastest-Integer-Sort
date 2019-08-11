#!/usr/bin/env bash
# $1: name of the object file to be used
# $2: folder location to write the data generated
# $3: array length
# $4: test cases

object_file=$1

if [[ ! -d $2 ]]; then
    mkdir -p $2
    echo "Folder created"
fi

cd "${2}"
output_folder=`pwd`
cd - > /dev/null

echo "Object file name       : \"${object_file}\""
echo "Output folder location : \"${output_folder}\""

file_name="raw_data_generator_integer.cpp"
echo "Compiling \"${file_name}\" ..."
g++ -std=c++14 -O2 "${file_name}" -o "${1}" || exit
echo 'Compilation successfully completed' && echo

len=${3}
test_cases=${4}

echo "test_cases = ${test_cases}" && echo

echo "Array size = ${len}" && echo
echo "------------------------"
./${object_file} <<< "$len $test_cases" > "${output_folder}/rawData_${test_cases}_${len}.csv"

rm ${object_file}