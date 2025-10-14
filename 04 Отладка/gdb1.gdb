set pagination off
set confirm off
file ./range

set logging file /dev/null
set logging enabled on

# 在 printf 那一行设置条件断点：current % 5 == 0
break range.c:42 if (current % 5 == 0)

commands
  silent
  printf "@@@ start=%d\n", start
  printf "@@@ stop=%d\n",  stop
  printf "@@@ step=%d\n",  step
  printf "@@@ cur=%d\n",   current
  continue
end

run 1 12 >/dev/null
quit

