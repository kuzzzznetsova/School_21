COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
FILENAME1="test_files/1.txt"
FILENAME2="test_files/2.txt"
FILENAME3="test_files/3.txt"
echo "" > test_files/log.txt
SHABLON="a"
SHABLON2="abc"
PATTERN1="test_files/pattern.txt"
PATTERN2="test_files/pattern2.txt"

echo "----------------STANDART-TEST---------------------------------------------------"
for i in -v -c -l -n -i -e 
do
          TEST1="$i $SHABLON $FILENAME1"
          ./s21_grep $TEST1 > s21_grep$i.txt
          grep $TEST1 > grep$i.txt
          DIFF_RES="$(diff -s s21_grep$i.txt grep$i.txt)"
          if [ "$DIFF_RES" == "Files s21_grep$i.txt and grep$i.txt are identical" ]
            then
              echo "SUCCES $TEST1"
              (( COUNTER_SUCCESS++ ))
            else
              echo "FAIL $TEST1"
              echo "$TEST1" >> test_files/log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep$i.txt grep$i.txt

done


echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"