# 言序 EF 类库

这是给易语言.飞扬（EF）的本地类库示例，通过动态加载 yanxu 的 C ABI 动态库来运行言序源码。

## 生成 yanxu 运行时

在 yanxu 核心仓库中先生成动态库：

```sh
cargo build --release
```

运行时动态库通常位于：

- macOS: `target/release/libyanxu.dylib`
- Linux: `target/release/libyanxu.so`
- Windows: `target/release/yanxu.dll`

## GitHub Actions 产物

本仓库的 workflow 会自动构建 Linux x86_64 版本，并上传 artifact：

- `言序.efn`：EF 本地类库文件
- `libyanxu.so`：yanxu C ABI 运行时
- `README.md`：使用说明
- `yanxu-efn-linux-x86_64.tar.gz`：上述文件的打包版本

推送 tag（例如 `v0.1.0`）时，workflow 会同时把 `yanxu-efn-linux-x86_64.tar.gz` 发布到 GitHub Release。

## 第三方代码

`third_party/efndk_cpp` 包含 EF NDK C++ 的构建所需子集，来源于 EF NDK 示例包，并已统一转换为 UTF-8。为支持 64 位 CI 编译，仓库内副本对少量旧式指针整数转换做了兼容调整。

## 本地编译 EF 类库

```sh
make
```

产物为 `言序.efn`。仓库已带上构建所需的 EF NDK C++ `include/source` 子集。若要改用本机 NDK，传入：

```sh
make EFNDK=/path/to/efndk_cpp_1/efn/cpp
```

## 暴露的类

类名：`言序.引擎`

静态方法：

- `载入运行时(运行时路径: 文本): 逻辑`
- `运行时已载入(): 逻辑`
- `取最后错误(): 文本`
- `取运行时路径(): 文本`
- `沙箱运行(运行时路径: 文本, 源码: 文本): 文本`

实例方法：

- `新建沙箱引擎(运行时路径: 文本): 逻辑`
- `新建不受限引擎(运行时路径: 文本): 逻辑`
- `运行(源码: 文本): 文本`
- `释放(): 逻辑`

`运行` 返回 yanxu C ABI 的 schema 1 JSON，例如：

```json
{"schema":1,"ok":true,"value":"空","type":"空","value_bytes":null,"output":["6"],"backend":"bytecode"}
```

## EF 使用思路

先调用 `言序.引擎.载入运行时("/path/to/libyanxu.dylib")`，再创建 `言序.引擎` 对象并调用 `新建沙箱引擎("")`。同一个对象的多次 `运行` 会复用持久 yanxu 引擎，因此变量和函数定义可跨调用保留。

不受限引擎会授予 yanxu 宿主权限，只应执行可信源码。
