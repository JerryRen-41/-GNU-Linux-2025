#!/bin/sh
# POSIX / dash compatible: randomized ASCII-art painter (refactored variant)
# Usage: ./randomize.sh [delay_seconds]

set -eu
export LC_ALL=C

DELAY="${1:-0}"
TMP_LIST=""

# ---------- utilities ----------
die() { printf '%s\n' "$*" >&2; exit 1; }

# robust float-like validation (allow "0", "0.03", ".5", "10.")
case "$DELAY" in
  ''|*[!0-9.]*|*.*.*) [ "$DELAY" = "0" ] || die "Invalid delay: $DELAY" ;;
  .) die "Invalid delay: $DELAY" ;;
esac

# prefer hexdump; fallback to od+sed
bytes_stream() {
  if command -v hexdump >/dev/null 2>&1; then
    # 1 byte per line as unsigned decimal
    hexdump -v -e '1/1 "%u\n"'
  else
    # od 输出前有空格，去掉前导空白并丢弃空行
    od -An -tu1 -v -w1 | sed 's/^[[:space:]]*//;/^$/d'
  fi
}

cleanup() {
  [ -n "$TMP_LIST" ] && rm -f "$TMP_LIST" 2>/dev/null || true
  if term_sz="$(stty size </dev/tty 2>/dev/null)"; then
    set -- $term_sz
    rows="$1"
    # 把光标挪回到最后一行，避免停留在中间
    [ -n "$rows" ] && [ "$rows" -gt 0 ] 2>/dev/null && tput cup "$((rows - 1))" 0 2>/dev/null || true
  fi
}
trap cleanup EXIT INT TERM HUP

tput clear 2>/dev/null || true
TMP_LIST="$(mktemp)"

# ---------- parse stdin into (row col byte) ----------
# ASCII only (32..126), LF=10 -> next row; CR=13 ignored
{
  row=0
  col=0
  bytes_stream | while IFS= read -r b; do
    # 保守过滤非数字行（极少数情况下 hexdump/od 异常输出）
    case "$b" in ''|*[!0-9]*) continue ;; esac

    case "$b" in
      10) # LF
        row=$((row + 1))
        col=0
        ;;
      13) # CR
        : # ignore
        ;;
      *)
        # 可见 ASCII
        if [ "$b" -ge 32 ] && [ "$b" -le 126 ] 2>/dev/null; then
          # 输出：row col byte
          printf '%d %d %d\n' "$row" "$col" "$b"
        fi
        col=$((col + 1))
        ;;
    esac
  done
} >"$TMP_LIST"

# 若无有效字符，直接退出（保持与原行为一致）
[ -s "$TMP_LIST" ] || { printf '%s\n' "No valid characters found" >&2; exit 0; }

# ---------- randomized paint ----------
# 说明：不使用 process substitution 以保持 POSIX 兼容
shuf "$TMP_LIST" | while IFS=' ' read -r R C B; do
  # 移动光标；失败则跳过该点
  tput cup "$R" "$C" 2>/dev/null || continue

  # 把十进制字节转为八进制并用 %b 展开：便于 dash/printf 可靠输出
  oct="$(printf '%03o' "$B")"
  printf '%b' "\\$oct"

  # 延迟（非零才 sleep）
  [ "$DELAY" = "0" ] || sleep "$DELAY"
done

# 保证末尾换行（不影响最终画面）
printf '\n'

