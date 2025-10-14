set pagination off
set confirm off
file ./range

set logging file /dev/null
set logging enabled on

# 如果你的 printf 不在第 42 行，请把下面的 42 改成实际行号
set $step_num = 0
break range.c:42

commands
  silent
  set $step_num = $step_num + 1
  if $step_num >= 28 && $step_num <= 35
    printf "@@@ start=%d\n", start
    printf "@@@ stop=%d\n",  stop
    printf "@@@ step=%d\n",  step
    printf "@@@ cur=%d\n",   current
  end
  continue
end

run -100 100 3 >/dev/null
quit

