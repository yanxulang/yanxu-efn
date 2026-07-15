#!/usr/bin/env sh
set -eu

YANXU_REPO="${YANXU_REPO:-https://github.com/YanXuLang/yanxu.git}"
YANXU_REF="${YANXU_REF:-}"
OUT_DIR="${OUT_DIR:-./yanxu-runtime}"
KEEP_SOURCE="${KEEP_SOURCE:-0}"

command -v cargo >/dev/null 2>&1 || {
  echo "未找到 cargo。请先安装 Rust 工具链：https://www.rust-lang.org/tools/install" >&2
  exit 1
}

command -v git >/dev/null 2>&1 || {
  echo "未找到 git。请先安装 Git。" >&2
  exit 1
}

WORK_ROOT="$(mktemp -d)"
SOURCE_DIR="$WORK_ROOT/yanxu"

cleanup() {
  if [ "$KEEP_SOURCE" != "1" ]; then
    rm -rf "$WORK_ROOT"
  fi
}
trap cleanup EXIT

git clone --depth 1 "$YANXU_REPO" "$SOURCE_DIR"
if [ -n "$YANXU_REF" ]; then
  (
    cd "$SOURCE_DIR"
    git fetch --depth 1 origin "$YANXU_REF"
    git checkout FETCH_HEAD
  )
fi

(
  cd "$SOURCE_DIR"
  cargo build --release
)

mkdir -p "$OUT_DIR"
case "$(uname -s)" in
  Darwin)
    cp "$SOURCE_DIR/target/release/libyanxu.dylib" "$OUT_DIR/libyanxu.dylib"
    echo "已生成 libyanxu.dylib: $OUT_DIR/libyanxu.dylib"
    ;;
  *)
    cp "$SOURCE_DIR/target/release/libyanxu.so" "$OUT_DIR/libyanxu.so"
    echo "已生成 libyanxu.so: $OUT_DIR/libyanxu.so"
    ;;
esac
