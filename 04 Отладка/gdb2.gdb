# gdb2.gdb — break probe；懒初始化计数；命中 35 项后禁用断点
set pagination off
set confirm off
file ./range

set inferior-tty /dev/null

break probe
commands
  silent
  set $k = ($k ? $k + 1 : 1)
  if ($k >= 28) && ($k <= 35)
    printf ">>> start=%d stop=%d step=%d cur=%d\n", start, stop, step, current
  end
  if ($k == 35)
    disable $bpnum
  end
  continue
end

run -100 100 3
quit

