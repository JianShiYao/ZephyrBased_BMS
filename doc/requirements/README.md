# 需求管理(StrictDoc + EARS)

本目录用 **StrictDoc** 管理需求。需求的**事实源是本目录下的 `.sdoc` 文件**(如 `bms.sdoc`),
本 README 只是"如何写好一条需求"的写作指南。需求采用 **EARS** 句式写在 `STATEMENT` 字段里,
面向 ISO 26262 / IEC 61508 的可追溯性:每条需求有唯一 UID,可被设计、代码、测试追溯。

工程配置见仓库根 `strictdoc_config.py`;生成/查看见本文末。

## 条目 ID(UID)规范

`ENV-REQ-<子系统>-<三位序号>`,例如 `ENV-REQ-SAFETY-001`。子系统与 `src/` 模块对应
(bms_core / can_if / safety_monitor …)。设计文档用 `ENV-DSN-*`。

## STATEMENT 的 EARS 五种句式

| 类型 | 模板 | 示例 |
| --- | --- | --- |
| 通用(Ubiquitous) | 系统应 <响应> | 系统应持续监测每节电芯电压 |
| 事件驱动(Event) | 当 <触发> 时,系统应 <响应> | 当检测到过压时,系统应断开充电回路 |
| 状态驱动(State) | 在 <状态> 期间,系统应 <响应> | 在充电状态期间,系统应每 100ms 上报 SOC |
| 非期望行为(Unwanted) | 如果 <条件>,那么系统应 <响应> | 如果 CAN 通信中断超过 1s,那么系统应进入安全态 |
| 可选特性(Optional) | 在 <特性存在> 处,系统应 <响应> | 在配备加热膜的电池包上,系统应支持低温加热 |

## 自定义 grammar 字段(见 `bms.sdoc` 的 `[GRAMMAR]`)

| 字段 | 说明 |
| --- | --- |
| `UID` | 唯一标识,见上 ID 规范 |
| `TITLE` | 简短标题 |
| `STATEMENT` | EARS 句式的需求正文 |
| `RATIONALE` | 理由/来源(安全分析、上位标准条款) |
| `ASIL` | QM / A / B / C / D |
| `VERIFICATION_METHOD` | Test / Analysis / Review / Inspection |
| `STATUS` | Draft / Reviewed / Approved |

## 一条需求的写法(片段)

```text
[REQUIREMENT]
UID: ENV-REQ-SAFETY-001
TITLE: 过压断充保护
STATEMENT: >>>
当任一电芯电压超过过压阈值时,系统应在 10 ms 内断开充电回路。
<<<
RATIONALE: >>>
防止电芯过充导致热失控;源自电池安全分析(FMEA)。
<<<
ASIL: C
VERIFICATION_METHOD: Test
STATUS: Draft
```

## 追溯关系

- **设计 → 需求**:设计条目(`doc/design/*.sdoc`)用 `RELATIONS: - TYPE: Parent / VALUE: <需求UID>`。
- **代码/测试 → 需求**:在源码或测试里加注释标记 `@relation(<需求UID>, scope=file|function|class|range)`,
  扫描路径见 `strictdoc_config.py` 的 `include_source_paths`。

## 生成与查看

```bash
# 生成 HTML + 追溯矩阵(或用 VS Code task「StrictDoc Export」)
strictdoc export . --output-dir output

# 本地 Web 编辑/浏览(或用 task「StrictDoc Server」)
strictdoc server .
```
