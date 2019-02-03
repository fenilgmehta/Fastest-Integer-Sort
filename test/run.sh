if [[ $# -eq 1 ]]
then
    # if [[ "a.out" -ot "${1}" || "a.out" -nt "${1}" ]]
    # then
        echo "Compiling ..." > "/dev/stderr"
        g++ -O2 "${1}"
    # fi;
    echo "Running ..." > "/dev/stderr"
    ./a.out
else
    # if [[ "$2" -ot "${1}" || "$2" -nt "${1}" ]]
    # then
        echo "Compiling ..." > "/dev/stderr"
        g++ -O2 "${1}" -o "${2}"
    # fi;
    echo "Running ..." > "/dev/stderr"
    ./${2}
fi;
