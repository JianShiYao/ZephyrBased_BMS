# C 代码缺陷检查条目(checkpatch)

> 本文档整理 pre-commit / CI 中 `checkpatch.pl` 对 **C 代码内容** 实际执行的检查条目。
> 权威来源:Zephyr `scripts/checkpatch.pl`(v4.4.0)的 `--list-types` 与本仓库根 `.checkpatch.conf`。
> 生成时间:2026-07-16。

## 1. 说明

- **数量**:checkpatch 全量 **208** 类;本项目 `.checkpatch.conf` 禁用 **25** 类(见 §2),启用 **186** 类。
- **文件模式过滤**:pre-commit 与 CI 均以 `checkpatch.pl -f`(整文件)模式运行,启用类中约 **19** 项属「提交/补丁元数据」检查(邮件、Signed-off-by、commit log、diff 格式等),**file 模式下不触发**,故不计入下方条目;真正作用于 C 代码内容的为 **167** 类(见 §3)。
- **规则文件位置**:`.checkpatch.conf` 必须置于仓库根(checkpatch 从当前工作目录查找它);`--max-line-length=100`、`--show-types`。
- **触发路径**:本地 `.githooks/pre-commit`(见 [gate.md](../gate.md) §2.4)与 CI 的 `check_compliance.py -m Checkpatch`。
- **定位提醒**:checkpatch 是「日常风格与常见缺陷底线」,**不等价于 MISRA C:2012**;功能安全的 MISRA 证据须以专用工具(Parasoft / Coverity)为准。

## 2. 已禁用的检查(25 项)

沿用 Zephyr 上游 `.checkpatch.conf` 的放宽项。标 † 者在本版 checkpatch 中已无对应类型,`--ignore` 为空操作(仅作与上游对照的占位)。

| 类型(type) | 禁用原因 |
| --- | --- |
| `PRINTK_WITHOUT_KERN_LEVEL` | Zephyr 用自有 logging,不用 KERN_ 级别 |
| `SPLIT_STRING` | 允许为可读性跨行拆分字符串字面量 |
| `VOLATILE` | 嵌入式寄存器/中断场景需要 volatile |
| `CONFIG_EXPERIMENTAL†` | 上游遗留类型,本版已无(空操作) |
| `PREFER_KERNEL_TYPES` | 用 C 标准 uintN_t,不强制内核 u32/s32 |
| `PREFER_SECTION` | 不强制 __section 写法 |
| `AVOID_EXTERNS` | 允许头文件外使用 extern |
| `NETWORKING_BLOCK_COMMENT_STYLE` | 放宽网络子系统专用注释风格 |
| `DATE_TIME` | 允许 `__DATE__` / `__TIME__` |
| `MINMAX` | 不强制 min()/max() 宏用法 |
| `CONST_STRUCT` | 不强制特定 struct 加 const |
| `FILE_PATH_CHANGES†` | 补丁元数据类,本版已无(空操作) |
| `SPDX_LICENSE_TAG` | app 有独立 LICENSE,不强制逐文件 SPDX 标签 |
| `C99_COMMENT_TOLERANCE` | 容忍 // 行注释 |
| `REPEATED_WORD` | 关闭重复词检测(误报多) |
| `UNDOCUMENTED_DT_STRING` | DT 绑定文档类,app 场景不适用 |
| `DT_SPLIT_BINDING_PATCH` | DT 绑定补丁拆分规则,不适用 |
| `DT_SCHEMA_BINDING_PATCH` | DT 绑定 schema 补丁规则,不适用 |
| `TRAILING_SEMICOLON` | 允许宏定义尾随分号 |
| `COMPLEX_MACRO` | 允许有意为之的复杂宏 |
| `MULTISTATEMENT_MACRO_USE_DO_WHILE` | 不强制多语句宏用 do{}while(0) |
| `ENOSYS` | 允许使用 ENOSYS 错误码 |
| `IS_ENABLED_CONFIG` | 不强制 `CONFIG_` 宏必须走 `IS_ENABLED()` |
| `EXPORT_SYMBOL` | EXPORT_SYMBOL 放置规则(内核模块概念) |
| `COMPARISON_TO_NULL` | 允许 == NULL / != NULL 显式比较 |

## 3. 启用的 C 代码缺陷检查条目(167 项,按主题)

### 3.1 空白与缩进(14)

| 类型(type) | 说明 |
| --- | --- |
| `BRACKET_SPACE` | '[' 前多余空格 |
| `CODE_INDENT` | 代码缩进应使用 Tab |
| `DEEP_INDENTATION` | 缩进层级过深(建议重构) |
| `INDENTED_LABEL` | 标签(label)不应缩进 |
| `LEADING_SPACE` | 行首用空格代替 Tab |
| `LINE_SPACING` | 声明块后缺少空行 |
| `QUOTED_WHITESPACE_BEFORE_NEWLINE` | 字符串内换行前有多余空白 |
| `SPACE_BEFORE_TAB` | Tab 之前混入空格 |
| `SPACING` | 运算符/关键字周围空格不规范 |
| `SUSPECT_CODE_INDENT` | 缩进与控制结构不匹配,疑似错误 |
| `SWITCH_CASE_INDENT_LEVEL` | switch/case 缩进层级不一致 |
| `TABSTOP` | 应使用 Tab 缩进处误用空格 |
| `TRAILING_WHITESPACE` | 行尾多余空白 |
| `WHITESPACE_AFTER_LINE_CONTINUATION` | 续行反斜杠后有多余空白 |

### 3.2 大括号 / 代码块 / 括号(12)

| 类型(type) | 说明 |
| --- | --- |
| `BRACES` | if/for/while 单语句大括号使用不当 |
| `ELSE_AFTER_BRACE` | else 应紧跟在 '}' 同一行 |
| `ONE_SEMICOLON` | 一行多个分号(多条语句) |
| `OPEN_BRACE` | '{' 位置不符(函数/控制块换行规则) |
| `OPEN_ENDED_LINE` | 行以运算符结尾的断行方式不当 |
| `PARENTHESIS_ALIGNMENT` | 换行后括号内参数未对齐 |
| `RETURN_PARENTHESES` | return 不需要外层括号 |
| `SIZEOF_PARENTHESIS` | sizeof 应写成 sizeof(...) |
| `TRAILING_STATEMENTS` | 同行尾随多条语句 |
| `UNNECESSARY_ELSE` | 前分支已 return,else 多余 |
| `UNNECESSARY_PARENTHESES` | 多余的括号 |
| `WHILE_AFTER_BRACE` | do-while 的 while 应紧跟 '}' |

### 3.3 行长度 / 续行 / 字符串(8)

| 类型(type) | 说明 |
| --- | --- |
| `ASSIGNMENT_CONTINUATIONS` | 赋值语句断行位置不当 |
| `CONCATENATED_STRING` | 相邻字符串字面量拼接缺空格 |
| `LINE_CONTINUATIONS` | 不必要或错误的行续接反斜杠 |
| `LOGICAL_CONTINUATIONS` | 逻辑运算符应置于行尾续行处 |
| `LONG_LINE` | 行超过最大长度(本项目 100) |
| `LONG_LINE_COMMENT` | 注释行超长 |
| `LONG_LINE_STRING` | 字符串行超长 |
| `STRING_FRAGMENTS` | 可合并的字符串被拆成多段 |

### 3.4 注释(3)

| 类型(type) | 说明 |
| --- | --- |
| `BLOCK_COMMENT_STYLE` | 块注释风格(收尾 '*/' 单独成行等) |
| `C99_COMMENTS` | 使用了 `//` 行注释(上游偏好 `/* */`) |
| `UNCOMMENTED_DEFINITION` | 锁等定义缺少说明注释 |

### 3.5 类型 / 常量 / 转换 / 位运算(17)

| 类型(type) | 说明 |
| --- | --- |
| `BOOL_COMPARISON` | 与 true/false 显式比较,多余 |
| `CONSTANT_COMPARISON` | 常量应置于比较式右侧 |
| `CONSTANT_CONVERSION` | 常量隐式转换可能溢出/截断 |
| `CONST_CONST` | 重复的 const |
| `HEXADECIMAL_BOOLEAN_TEST` | 对布尔结果做十六进制比较,疑误 |
| `MASK_THEN_SHIFT` | 先掩码后移位的写法建议调整 |
| `MISORDERED_TYPE` | 类型限定符顺序不当(如 int static) |
| `NEW_TYPEDEFS` | 新增 typedef(上游不鼓励) |
| `POINTER_LOCATION` | 指针 '*' 位置应贴变量名 |
| `STATIC_CONST_CHAR_ARRAY` | 静态字符串数组应加 const |
| `STORAGE_CLASS` | 存储类关键字位置不当(static/extern) |
| `TEST_NOT_TYPE` | checkpatch 自测类型(反例) |
| `TEST_TYPE` | checkpatch 自测类型(正例) |
| `TYPECAST_INT_CONSTANT` | 整型常量不必要的强制类型转换 |
| `UNNECESSARY_CASTS` | 多余的类型转换 |
| `UNNECESSARY_INT` | 多余的 int(如 unsigned int 可简写) |
| `UNSPECIFIED_INT` | 未显式写 int(如裸 unsigned) |

### 3.6 宏定义(11)

| 类型(type) | 说明 |
| --- | --- |
| `ALLOC_SIZEOF_STRUCT` | 分配大小应对指针解引用取 sizeof |
| `BIT_MACRO` | 位定义建议用 BIT() 宏 |
| `DO_WHILE_MACRO_WITH_TRAILING_SEMICOLON` | do{}while(0) 宏尾多余分号 |
| `KREALLOC_ARG_REUSE` | realloc 参数复用导致泄漏风险 |
| `MACRO_ARG_PRECEDENCE` | 宏参数未加括号,存在优先级风险 |
| `MACRO_ARG_REUSE` | 宏内多次引用同一参数(副作用风险) |
| `MACRO_WITH_FLOW_CONTROL` | 宏内含 return/goto 等控制流 |
| `SINGLE_STATEMENT_DO_WHILE_MACRO` | 单语句宏不必用 do{}while(0) |
| `SIZEOF_ADDRESS` | 对取地址结果做 sizeof,疑误 |
| `SIZEOF_CHAR_LITERAL` | sizeof('c') 结果非预期 |
| `WEAK_DECLARATION` | __weak 声明的使用限制 |

### 3.7 声明与命名(11)

| 类型(type) | 说明 |
| --- | --- |
| `ARRAY_SIZE` | 求数组元素个数应用 ARRAY_SIZE() |
| `CAMELCASE` | 标识符使用了驼峰命名(偏好下划线) |
| `EMBEDDED_FUNCTION_NAME` | 日志里硬编码函数名,应用 `__func__` |
| `FUNCTION_ARGUMENTS` | 函数原型应带参数名 |
| `FUNCTION_WITHOUT_ARGS` | 无参函数应声明为 (void) |
| `GLOBAL_INITIALISERS` | 全局变量显式初始化为 0/NULL,多余 |
| `INITIALISED_STATIC` | static 显式初始化为 0/NULL,多余 |
| `INLINE` | 行内 inline 关键字用法 |
| `INLINE_LOCATION` | inline 关键字位置不当 |
| `MULTIPLE_ASSIGNMENTS` | 一行多重赋值 |
| `MULTIPLE_DECLARATION` | 一行声明多个变量 |

### 3.8 控制流(6)

| 类型(type) | 说明 |
| --- | --- |
| `ASSIGN_IN_IF` | 在 if 条件里赋值 |
| `IF_0` | #if 0 包裹的死代码 |
| `IF_1` | #if 1 冗余条件 |
| `LIKELY_MISUSE` | likely()/unlikely() 误用 |
| `RETURN_VOID` | void 函数结尾多余的 return; |
| `UNNECESSARY_BREAK` | return 后多余的 break |

### 3.9 printf / 日志 格式(11)

| 类型(type) | 说明 |
| --- | --- |
| `AVOID_BUG` | 避免使用 BUG()/BUG_ON() 使系统崩溃 |
| `LOGGING_CONTINUATION` | 日志续行(拆分打印)不当 |
| `NAKED_SSCANF` | sscanf 未检查返回值 |
| `OOM_MESSAGE` | 内存分配失败无需再打日志 |
| `PREFER_PRINTF` | 建议用标准 printf 家族 |
| `PREFER_SCANF` | 建议用标准 scanf 家族 |
| `PRINTF_0XDECIMAL` | 0x 前缀却用了 %d(应 %x) |
| `PRINTF_L` | printf 长度修饰符 l 用法错误 |
| `PRINTF_Z` | size_t 应使用 %z 修饰符 |
| `VSPRINTF_POINTER_EXTENSION` | %p 扩展修饰符用法 |
| `VSPRINTF_SPECIFIER_PX` | %px 打印裸指针地址(慎用) |

### 3.10 内存 / 字符串 / 常见 API(7)

| 类型(type) | 说明 |
| --- | --- |
| `ALLOC_ARRAY_ARGS` | 数组分配参数顺序建议 |
| `ALLOC_WITH_MULTIPLY` | 分配用乘法应改数组分配 API 防溢出 |
| `CONSIDER_KSTRTO` | simple_strtol 等已过时,改 kstrto* |
| `DATA_RACE` | 疑似数据竞争访问 |
| `MEMORY_BARRIER` | 内存屏障应加注释说明 |
| `MEMSET` | memset 参数顺序/长度疑误 |
| `SSCANF_TO_KSTRTO` | 单值解析建议用 kstrto* |

### 3.11 预处理 / 头文件包含(8)

| 类型(type) | 说明 |
| --- | --- |
| `API_DEFINE` | API 宏定义方式不当 |
| `ARCH_DEFINES` | 架构相关宏定义使用限制 |
| `ARCH_INCLUDE_LINUX` | 架构头包含路径不当 |
| `DEFINE_ARCH_HAS` | 避免用 #define 定义 `ARCH_HAS_*` |
| `INCLUDE_LINUX` | 应包含 `<linux/xxx.h>` 而非 `<asm/xxx.h>` |
| `MALFORMED_INCLUDE` | #include 路径格式错误 |
| `MODIFIED_INCLUDE_ASM` | 直接包含 `<asm/>` 而非 `<linux/>` |
| `UAPI_INCLUDE` | UAPI 头不应包含非 UAPI 头 |

### 3.12 文件卫生 / 编码(6)

| 类型(type) | 说明 |
| --- | --- |
| `BYTE_ORDER` | 字节序处理应使用标准转换宏 |
| `CVS_KEYWORD` | 残留 CVS 关键字($Id$ 等) |
| `DOS_LINE_ENDINGS` | 含 CRLF(DOS)行尾 |
| `INVALID_UTF8` | 非法 UTF-8 字节 |
| `MISSING_EOF_NEWLINE` | 文件结尾缺少换行 |
| `TYPO_SPELLING` | 注释/字符串拼写疑似错误 |

### 3.13 上游继承:Linux 内核专用(BMS 应用基本不触发)(53)

| 类型(type) | 说明 |
| --- | --- |
| `CONFIG_TYPE_BOOLEAN` | Kconfig boolean 类型(内核构建) |
| `CONSIDER_COMPLETION` | 建议用 completion 而非手写等待(内核) |
| `CONST_READ_MOSTLY` | `__read_`mostly 与 const 冲突(内核) |
| `DEPRECATED_API` | 使用了已废弃的内核 API |
| `DEPRECATED_VARIABLE` | 使用了已废弃变量(内核) |
| `DEVICE_ATTR_FUNCTIONS` | sysfs DEVICE_ATTR 函数命名(内核) |
| `DEVICE_ATTR_PERMS` | sysfs 属性权限(内核) |
| `DEVICE_ATTR_RO` | 建议 DEVICE_ATTR_RO(内核) |
| `DEVICE_ATTR_RW` | 建议 DEVICE_ATTR_RW(内核) |
| `DEVICE_ATTR_WO` | 建议 DEVICE_ATTR_WO(内核) |
| `DUPLICATED_SYSCTL_CONST` | sysctl 常量重复(内核) |
| `ENOTSUPP` | 应使用 EOPNOTSUPP 而非 ENOTSUPP(内核) |
| `EXPORTED_WORLD_WRITABLE` | 导出的 sysfs 全局可写(内核安全) |
| `HOTPLUG_SECTION` | 热插拔段属性用法(内核) |
| `INIT_ATTRIBUTE` | __init 属性位置(内核) |
| `IN_ATOMIC` | 原子上下文中的非法调用(内核) |
| `JIFFIES_COMPARISON` | jiffies 应用 time_after 比较(内核) |
| `LINUX_VERSION_CODE` | 避免 LINUX_VERSION_CODE(内核) |
| `LOCKDEP` | lockdep 相关(内核) |
| `LOCKING` | 加锁/解锁配对问题(内核锁模型) |
| `LONG_UDELAY` | 过长 udelay 应改 mdelay(内核) |
| `MISPLACED_INIT` | __init 位置不当(内核) |
| `MODULE_LICENSE` | MODULE_LICENSE 取值(内核模块) |
| `MSLEEP` | msleep(<20) 精度不足(内核) |
| `MULTILINE_DEREFERENCE` | 跨行指针解引用可读性(内核风格) |
| `NON_OCTAL_PERMISSIONS` | 权限位应写八进制(内核) |
| `NR_CPUS` | NR_CPUS 比较用法(内核) |
| `OBSOLETE` | 过时的接口(内核) |
| `PREFER_ALIGNED` | 建议用 __aligned()(内核) |
| `PREFER_DEV_LEVEL` | 建议用 `dev_*` 日志(内核驱动) |
| `PREFER_ETHER_ADDR_COPY` | 建议 ether_addr_copy(内核网络) |
| `PREFER_ETHER_ADDR_EQUAL` | 建议 ether_addr_equal(内核网络) |
| `PREFER_ETH_BROADCAST_ADDR` | 建议 eth_broadcast_addr(内核网络) |
| `PREFER_ETH_ZERO_ADDR` | 建议 eth_zero_addr(内核网络) |
| `PREFER_IS_ENABLED` | 建议用 IS_ENABLED()(内核) |
| `PREFER_PACKED` | 建议用 __packed(内核) |
| `PREFER_PR_LEVEL` | 建议用 `pr_*` 日志(内核) |
| `PREFER_SEQ_PUTS` | seq_printf 无格式应改 seq_puts(内核) |
| `PRINTK_RATELIMITED` | 建议用 ratelimited 打印(内核) |
| `SYMBOLIC_PERMS` | 文件权限应用八进制而非符号(内核) |
| `TEST_ATTR` | 属性自测类型 |
| `TEST_NOT_ATTR` | 属性自测类型(反例) |
| `UNDOCUMENTED_SETUP` | __setup 缺少文档(内核) |
| `UNNECESSARY_KERN_LEVEL` | 多余的 KERN_ 日志级别(内核) |
| `USE_DEVICE_INITCALL` | 建议用 device_initcall(内核) |
| `USE_FUNC` | 建议使用现成辅助函数(内核) |
| `USE_LOCKDEP` | 建议用 lockdep 断言(内核) |
| `USE_NEGATIVE_ERRNO` | 应返回负 errno(内核约定) |
| `USE_RELATIVE_PATH` | 应使用相对路径(内核文档) |
| `USE_SPINLOCK_T` | 应使用 spinlock_t(内核) |
| `USLEEP_RANGE` | 短延时应用 usleep_range(内核) |
| `WAITQUEUE_ACTIVE` | waitqueue_active 用法(内核) |
| `YIELD` | 避免使用 yield()(内核) |

## 4. 使用与相关

手动对改动执行(仓库根目录):

```bash
# 单文件整文件检查
perl $ZEPHYR_BASE/scripts/checkpatch.pl --no-tree -f app/src/main.c
# 或经合规谐架(对本分支相对 main 的改动)
python3 $ZEPHYR_BASE/scripts/ci/check_compliance.py -m Checkpatch -c main..HEAD
```

- 查看某类型的完整定义:`perl $ZEPHYR_BASE/scripts/checkpatch.pl --list-types`。
- 门禁全景见 [gate.md](../gate.md);规则改造背景见 [脚手架搭建规范.md](../development/脚手架搭建规范.md) §6.1。
