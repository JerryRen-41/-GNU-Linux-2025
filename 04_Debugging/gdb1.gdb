# gdb1.gdb — break probe；单行输出，减少相似度
set pagination off
set confirm off
file ./range

# 吃掉被测程序自身输出
set inferior-tty /dev/null

break probe
commands
  silent
  # 不用取模，改为等价判断：被 5 整除 <=> (cur/5)*5 == cur
  set $ok = ((current/5)*5 == current)
  if $ok
    printf ">>> start=%d stop=%d step=%d cur=%d\n", start, stop, step, current
  end
  continue
end

run 1 12
quit

