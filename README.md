# 言序 EF 类库

这是给易语言.飞扬（EF）的本地类库示例，通过动态加载 yanxu 的 C ABI 动态库来运行言序源码。

## 下载

推荐从 GitHub Actions 或 Releases 下载预构建包：

- 最新构建产物：打开 [Actions / Build EF library](https://github.com/yanxulang/yanxu-efn/actions/workflows/build.yml)，选择最近一次成功运行，在页面底部下载 `yanxu-efn-windows-x86_64` 或 `yanxu-efn-linux-x86_64` artifact。
- 稳定版本：打开 [Releases](https://github.com/yanxulang/yanxu-efn/releases)，下载对应平台的压缩包。

Windows 用户通常下载 `yanxu-efn-windows-x86_64.zip`，解压后会得到：

- `言序.efn`：EF 本地类库文件
- `yanxu.dll`：yanxu C ABI 运行时
- `README.md`：使用说明

建议把 `言序.efn` 与 `yanxu.dll` 放在同一目录，或把 `yanxu.dll` 所在目录加入系统 `PATH`。在 EF 程序里载入运行时时，传入 `yanxu.dll` 的完整路径或同目录相对路径。

## GitHub Actions 产物

本仓库的 workflow 会自动构建：

- Windows x86_64: `yanxu-efn-windows-x86_64.zip`，包含 `言序.efn` 与 `yanxu.dll`
- Linux x86_64: `yanxu-efn-linux-x86_64.tar.gz`，包含 `言序.efn` 与 `libyanxu.so`

推送 tag（例如 `v0.1.0`）时，workflow 会把两个平台压缩包发布到 GitHub Releases。普通分支 push 会上传为 Actions artifact。

## 本地编译

先在 yanxu 核心仓库中生成 C ABI 动态库：

```sh
cargo build --release
```

运行时动态库通常位于：

- Windows: `target/release/yanxu.dll`
- Linux: `target/release/libyanxu.so`
- macOS: `target/release/libyanxu.dylib`

再在本仓库根目录编译 EF 类库：

```sh
make
```

产物为 `言序.efn`。仓库已带上构建所需的 EF NDK C++ `include/source` 子集。若要改用本机 NDK，传入：

```sh
make EFNDK=/path/to/efndk_cpp/efn/cpp
```

Windows 交叉编译示例：

```sh
make TARGET=windows CXX=x86_64-w64-mingw32-g++ OUTPUT_FILE=言序.efn
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

Windows 示例流程：

1. 下载并解压 `yanxu-efn-windows-x86_64.zip`。
2. 在 EF 工程中引用 `言序.efn`。
3. 调用 `言序.引擎.载入运行时("yanxu.dll")`，或传入 `yanxu.dll` 的完整路径。
4. 创建 `言序.引擎` 对象，调用 `新建沙箱引擎("")`。
5. 调用 `运行("言 1 加 2；")`，读取返回的 JSON。

同一个对象的多次 `运行` 会复用持久 yanxu 引擎，因此变量和函数定义可跨调用保留。不受限引擎会授予 yanxu 宿主权限，只应执行可信源码。

## 第三方代码与版权

`third_party/efndk_cpp` 包含 EF NDK C++ 的构建所需子集，来源于用户提供的 EF NDK 示例包，并已统一转换为 UTF-8。为支持 64 位 CI 编译，仓库内副本对少量旧式指针整数转换做了兼容调整。

EF NDK 源文件保留其原始 license 文本，见 [`third_party/efndk_cpp/license.txt`](third_party/efndk_cpp/license.txt)。根据该 license，使用、复制、分发源码或二进制时应保留原作者版权声明和许可协议；若分发修改版本，应说明修改内容；相关代码仅应在易语言.飞扬类库、工具或相关程序中使用。EF NDK 版权归其原作者所有，本项目不主张对该第三方代码拥有版权。
