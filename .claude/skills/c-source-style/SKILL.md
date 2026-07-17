---
name: c-source-style
description: 本仓库 C 源文件组织规范(foxBMS-2 风格)。在创建或修改任何 .c/.h 文件之前调用,确保文件按「许可头 + doxygen @file 块 + 固定分节栏」组织,并符合仓库根 .clang-format。以 examples/examples.c|.h 为复制模板。
---

# C 源文件组织规范(foxBMS-2 风格)

创建或修改任何 `.c` / `.h` 文件时,必须遵循本规范。
唯一事实源是两份活模板 `examples/examples.c` 与 `examples/examples.h`,以及仓库根
`.clang-format`(排版)。**新建文件时优先复制模板再改**,不要凭记忆手写骨架。

## 文件整体结构(自上而下三块)

1. **许可头**:保留本仓库自己的许可声明,原样使用:

   ```c
   /*
    * Copyright (c) 2026 EnerVenue
    *
    * SPDX-License-Identifier: Apache-2.0
    */
   ```

   **严禁**套用 foxBMS 的 BSD-3-Clause / Fraunhofer 版权头——那是上游的版权声明,
   不得出现在本仓库源码里。

2. **doxygen `@file` 块**:字段固定,按模块实际信息填写。

   ```c
   /**
    * @file    <文件名,如 examples.c>
    * @author  <作者>
    * @date    <创建日期> (date of creation)
    * @updated <最后更新日期> (date of last update)
    * @version <版本,如 v1.0.0>
    * @ingroup <doxygen 分组,如 EXAMPLES>
    * @prefix  <模块符号前缀,如 BMS_CELL>
    *
    * @brief   <一句话简述>
    * @details <详述,可写 TODO>
    */
   ```

3. **分节栏**:见下。格式为 `/*========== <标题> ===…===*/`,**可见宽度严格 79 列**
   (等号补齐)。**空节也保留占位**,顺序固定不可打乱。

## `.h` 分节顺序(6 节)

include guard(`#ifndef BMS_APP_..._H_`)放在许可头与 `@file` 块之后、第一个分节栏之前。

```c
/*========== Includes =======================================================*/
/*========== Macros and Definitions =========================================*/
/*========== Extern Constant and Variable Declarations ======================*/
/*========== Extern Function Prototypes =====================================*/
/*========== Getter for static Variables (Unit Test) ========================*/
/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/
```

文件末尾以 `#endif /* BMS_APP_..._H_ */` 收尾,尾注与 guard 宏名一致。

## `.c` 分节顺序(8 节)

```c
/*========== Includes =======================================================*/
/*========== Macros and Definitions =========================================*/
/*========== Static Constant and Variable Definitions =======================*/
/*========== Extern Constant and Variable Definitions =======================*/
/*========== Static Function Prototypes =====================================*/
/*========== Static Function Implementations ================================*/
/*========== Extern Function Implementations ================================*/
/*========== Externalized Static Function Implementations (Unit Test) =======*/
```

## 落地步骤

1. 复制 `examples/examples.h` / `examples/examples.c` 为目标文件名。
2. 替换 `@file`、`@prefix`、`@ingroup`、include guard 宏名、示范符号名(`bms_cell_*` /
   `struct bms_cell_stats`)为目标模块的对应名称。
3. 按分节栏归位代码,空节保留占位。
4. 缩进用 Tab(对齐 `.clang-format` 的 `IndentWidth: 8` / `UseTab`)。
5. 写完运行 `clang-format -i <file>` 自动修排版,或 `clang-format --dry-run --Werror <file>`
   自查(仓库根 `.clang-format` 会被逐级向上自动查找)。

> 注意:分节栏是注释,clang-format **不会**校验其 79 列宽度,需人工/复制模板保证;
> 保存后 `.claude` 的编辑时 hook 与 `.githooks/pre-commit` 会跑 clang-format(+ checkpatch)。
