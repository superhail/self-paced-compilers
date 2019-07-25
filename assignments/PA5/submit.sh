#! /bin/sh
# The original perl script seems not working anymore

success_msg=""
success_count=0
fail_msg=""
fail_count=0
option="-gt"

function test()
{
  filename=$1;
  bash mycoolc $option $filename;
  compiled="$(echo $filename | cut -d '.' -f 1).s"
  if [ ! -f "$compiled" ]
  then
    fail_msg="FAIL $filename\n$fail_msg"
    let fail_count=$fail_count+1
    return 0
  fi
  mycoolc_ret=$(bash /usr/class/bin/spim "$compiled" | grep -vE 'Garbage collecting|Major');
  
  bash /usr/class/bin/coolc $option $filename;
  answer_ret=$(bash /usr/class/bin/spim "$compiled" | grep -vE 'Garbage collecting|Major');

  if [[ "$mycoolc_ret" == "$answer_ret" ]];
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

#test grading/lam.cl
for file in `find grading -iname "*.cl" | sort`
do
  echo $file
  test $file
done

echo -e $success_msg
echo -e $fail_msg
let all_count=$success_count+$fail_count
echo "$success_count/$all_count"


