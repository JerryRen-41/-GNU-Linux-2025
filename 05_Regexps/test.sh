#!/usr/bin/env bash
# test.sh — 轻量测试框架（与 sed -E 对比）
# 需要 esub 的参数顺序：<regexp> <substitution> <string>

set -u
: "${ESUB_BIN:=./esub}"  # 可从 Makefile 注入 build/esub

GREEN=$'\033[32m'
RED=$'\033[31m'
YEL=$'\033[33m'
NC=$'\033[0m'

pass=0; fail=0; xpass=0; xfail=0

tmpdir="$(mktemp -d 2>/dev/null || mktemp -d -t esubtest)"
trap 'rm -rf "$tmpdir"' EXIT

have_sed_E() {
  printf '%s\n' 'a1' | sed -E 's/([a-z])([0-9])/\2\1/' >/dev/null 2>&1
}

if ! have_sed_E; then
  echo "${RED}[FATAL]${NC} 本机 sed 不支持 -E，测试无法进行"
  exit 2
fi

# 对比 esub 与 sed 的输出与退出码（期望通过）
t_ok() {
  local in="$1" pat="$2" sub="$3" desc="$4"
  local sed_out sed_rc esub_out esub_rc

  sed_out="$(printf '%s\n' "$in" | sed -E "s/$pat/$sub/")"; sed_rc=$?
  esub_out="$("$ESUB_BIN" "$pat" "$sub" "$in" 2>/dev/null)"; esub_rc=$?

  if [[ "$sed_out" == "$esub_out" && $sed_rc -eq $esub_rc ]]; then
    printf "%s✔%s %s\n" "$GREEN" "$NC" "$desc"
    ((pass++))
  else
    printf "%s✖%s %s\n" "$RED" "$NC" "$desc"
    echo "  input:       $in"
    echo "  pattern:     $pat"
    echo "  replacement: $sub"
    echo "  sed out(rc): [$sed_rc] $sed_out"
    echo "  esub out(rc):[$esub_rc] $esub_out"
    ((fail++))
  fi
}

# 预期“失败”的测试（例如：引用不存在分组 -> esub 设计成报错）
t_xfail() {
  local in="$1" pat="$2" sub="$3" desc="$4"
  if "$ESUB_BIN" "$pat" "$sub" "$in" >/dev/null 2>&1; then
    printf "%sXPASS%s %s (本应失败却通过)\n" "$YEL" "$NC" "$desc"
    ((xpass++))
  else
    printf "%sXFAIL%s %s\n" "$YEL" "$NC" "$desc"
    ((xfail++))
  fi
}

echo "[info] 使用二进制: $ESUB_BIN"
echo

# ----------------- 正常对齐 sed -E 的用例 -----------------
t_ok "hello world"                  "world"                       "universe"     "简单替换"
t_ok "123-456-789"                  "[0-9]+"                      "X"            "仅替换第一个数字段"
t_ok "abc123def456"                 "([a-z]+)([0-9]+)"           "\\2-\\1"      "分组换位"
t_ok "test@example.com"             "(.*)@(.*)"                  "\\1 at \\2"   "邮箱分拆"
t_ok "aaabbbccc"                    "(.)\\1+"                    "\\1"          "压缩重复字符（贪婪）"
t_ok "CamelCase"                    "([A-Z])"                    "_\\1"         "首个大写前加下划线"
t_ok "test test test"               "test"                       "OK"           "只替换第一个匹配"
t_ok "no match here"                "xyz"                        "replaced"     "无匹配 => 原样输出"
t_ok ""                             ".*"                         "empty"        "空串匹配"
t_ok "abc"                          "(.*)"                       "[\\1]"        "整体捕获为一组"

# ----------------- 预期失败（严格语义：不存在分组 -> 报错） -----------------
t_xfail "x" "(x)" "\9" "引用不存在的分组 \\9 -> 应报错"

echo
echo "==============================="
echo "PASS : $pass"
echo "FAIL : $fail"
echo "XFAIL: $xfail"
echo "XPASS: $xpass"
echo "==============================="

# 退出码：有失败或误通过则置 1
if (( fail > 0 || xpass > 0 )); then
  exit 1
fi

