#! /bin/sh
# The original perl script seems not working anymore

success_msg=""
success_count=0
fail_msg=""
fail_count=0

function test()
{
  filename=$1;
  bash mycoolc $filename;
  compiled="$(echo $filename | cut -d '.' -f 1).s"
  if [ ! -f "$compiled" ]
  then
    fail_msg="FAIL $filename\n$fail_msg"
    let fail_count=$fail_count+1
    return 0
  fi
  mycoolc_ret=$(bash /usr/class/bin/spim "$compiled" 2>&1);
  
  bash /usr/class/bin/coolc $filename;
  answer_ret=$(bash /usr/class/bin/spim "$compiled" 2>&1);

  if [[ $mycoolc_ret == $answer_ret ]];
  then
    success_msg="SUCC $filename\n$success_msg"
    let success_count=$success_count+1
  else
    fail_msg="FAIL $filename\n$fail_msg"
    let fail_count=$fail_count+1
  fi
}


# main body
for file in `find grading -iname "*.s"`
do
  rm $file
done

#test grading/init-order-super.cl
for file in `find grading -iname "*.cl" | sort`
do
  test $file
done

echo -e $success_msg
echo -e $fail_msg
let all_count=$success_count+$fail_count
echo "$success_count/$all_count"
