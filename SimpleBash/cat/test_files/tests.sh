SUCCESS=0
FAIL=0
COUNTER=0
DIFF=""

s21_command=(
    "./s21_cat"
    )
sys_command=(
    "cat"
    )

flags=(
    "b"
    "e"
    "n"
    "s"
    "t"
    "v"
)   
manual=(
"-s test_files/test_1_cat.txt"
"-b test_files/test_1_cat.txt"
"-n test_files/test_1_cat.txt nofile.txt" 
"-t test_files/test_3_cat.txt"
"no_file.txt"
"-n test_files/test_1_cat.txt"
"-n test_files/test_1_cat.txt test_files/test_2_cat.txt"
"-v test_files/test_5_cat.txt"
)

gnu=(
"-T test_files/test_1_cat.txt"
"-E test_files/test_1_cat.txt"
"--number test_files/test_2_cat.txt"
"--squeeze-blank test_files/test_1_cat.txt"
"--number-nonblank test_files/test_4_cat.txt"
)
run_test() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
    "${s21_command[@]}" $param > "${s21_command[@]}".log
    "${sys_command[@]}" $param > "${sys_command[@]}".log
    DIFF="$(diff -s "${s21_command[@]}".log "${sys_command[@]}".log)"
    let "COUNTER++"
    if [ "$DIFF" == "Files "${s21_command[@]}".log and "${sys_command[@]}".log are identical" ]
    then
        let "SUCCESS++"
        echo "$COUNTER - Success $param"
    else
        let "FAIL++"
        echo "$COUNTER - Fail $param"
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}
run_gnu_test() {
    param=$(echo "$@" | sed "s/FLAGS/$var/")
	param_cat=$(echo "$param" | sed "s/T/t/g")
	param_cat=$(echo "$param_cat" | sed "s/E/e/g")
	param_cat=$(echo "$param_cat" | sed "s/-number-nonblank/b/g")
	param_cat=$(echo "$param_cat" | sed "s/-number/n/g")
	param_cat=$(echo "$param_cat" | sed "s/-squeeze-blank/s/g")
	${s21_command[@]} $param > "${s21_command[@]}".log
    ${sys_command[@]} $param_cat > "${sys_command[@]}".log

    DIFF="$(diff -s "${s21_command[@]}".log "${sys_command[@]}".log)"
    let "COUNTER++"
    if [ "$DIFF" == "Files "${s21_command[@]}".log and "${sys_command[@]}".log are identical" ]
    then
        let "SUCCESS++"
        echo "$COUNTER - Success $param"
    else
        let "FAIL++"
        echo "$COUNTER - Fail $param"
    fi
    rm -f "${s21_command[@]}".log "${sys_command[@]}".log
}

echo "^^^^^^^^^^^^^^^^^^^^^^^"
echo "TESTS WITH NORMAL FLAGS"
echo "^^^^^^^^^^^^^^^^^^^^^^^"
printf "\n"
echo "#######################"
echo "MANUAL TESTS"
echo "#######################"
printf "\n"
for i in "${manual[@]}"
do
    var="-"
    run_test "$i"
done
for i in "${gnu[@]}"
do
    var="-"
    run_gnu_test "$i"
done
printf "\n"