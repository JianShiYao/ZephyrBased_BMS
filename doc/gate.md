# 项目门禁现状(Gate Status)

> 本文档汇总 bms-app 仓库**当前实际存在**的质量门禁及其落地状态,并在 §3 对照
> `DevOps` 仓库《质量门禁体系》要求做差距分析。
> 更新时间:2026-07-17。规则与阶段规划的权威来源见 `development/脚手架搭建规范.md`。

## 1. 总览

| 门禁 | 检查内容 | 载体 / 工具 | 触发时机 | 状态 |
| --- | --- | --- | --- | --- |
| ClangFormat | C 代码风格 | `.clang-format`(pre-commit + `check_compliance.py`) | `git commit` / 手动 | 已落地 |
| Checkpatch | C 代码常见缺陷 | `.checkpatch.conf` + `checkpatch.pl` | `git commit` / 手动 | 已落地 |
| cppcheck / MISRA | 静态分析 + MISRA C:2012 | `cppcheck --addon=misra`(pre-commit) | `git commit` | 已落地(本地/尽力) |
| Markdownlint(编辑) | Markdown 规范 | `.markdownlint.json` + Claude Hook | 编辑 `.md` 时自动 | 已落地 |
| Markdownlint(提交) | Markdown 规范 | `.githooks/pre-commit` | `git commit` | 已落地 |
| 提交信息 | Conventional Commits | `.gitmessage`(commit.template) | `git commit` 填模板 | 部分(仅模板) |
| 忽略产物 | 合规/构建产物不入库 | `.gitignore` | `git add` | 已落地 |
| CI 门禁 | PR 级自动化 | GitHub Actions | PR / push | 未落地 |
| StrictDoc | 需求追溯 / 覆盖率 | `strictdoc export` | PR / 版本 / 发布 | 规划中 |

## 2. 已落地门禁

### 2.1 ClangFormat(C 代码风格)

- 配置:仓库根 `.clang-format`,`BasedOnStyle: LLVM`,`ColumnLimit: 100`,
  `IndentWidth: 8`,`UseTab: ForContinuationAndIndentation`(从 Zephyr 原样拷贝)。
- 本体来自 Zephyr:`$ZEPHYR_BASE/scripts/ci/check_compliance.py` 的 `ClangFormat` 项。
- 生效范围:clang-format 逐级向上查找配置,放仓库根即被整棵源码树(含 `tests/`)继承。
- 本地依赖:`clang-format` 与 `clang-format-diff.py`;Ubuntu 打包差异的软链见
  `development/构建指南.md` §2.1。

```bash
# 对本分支相对 main 的改动运行(app 仓库根目录执行)
python3 $ZEPHYR_BASE/scripts/ci/check_compliance.py -m ClangFormat -c main..HEAD
```

### 2.2 Checkpatch(C 代码常见缺陷)

- 配置:仓库根 `.checkpatch.conf`,从 Zephyr 沿用并改造。
- 关键参数:`--max-line-length=100`、`--show-types`,并 `--ignore` 了 25 项上游规则
  (如 `PREFER_KERNEL_TYPES`、`COMPLEX_MACRO`、`SPDX_LICENSE_TAG` 等);逐项清单与启用条目见
  [check/C代码缺陷检查条目.md](check/C代码缺陷检查条目.md)。
- 已移除上游 `--typedefsfile=...`(该相对路径在 app 仓库不存在)。
- 本体来自 Zephyr:`$ZEPHYR_BASE/scripts/checkpatch.pl`;必须放仓库根
  (checkpatch 只在当前工作目录查找 `.checkpatch.conf`)。

```bash
# checkpatch 与 clang-format 可一并跑
python3 $ZEPHYR_BASE/scripts/ci/check_compliance.py -m Checkpatch -m ClangFormat -c main..HEAD
```

> 定位:checkpatch 是「日常风格底线」,不等价于 MISRA C:2012;功能安全的 MISRA
> 证据须以专用工具(Parasoft / Coverity)为准。

### 2.3 Markdownlint(Markdown 规范)—— 唯一「自动强制」门禁

- 配置:仓库根 `.markdownlint.json`(`MD040` 强制代码块标语言、`MD060` compact 表格、
  `MD024` siblings_only、`MD013` 关闭行宽)。
- 强制机制:`.claude/settings.json` 的 `PostToolUse` Hook —— 每次 `Write`/`Edit` 到
  `*.md` 后自动跑 `markdownlint-cli2`,**失败即 `exit 2` 阻断**。这是目前唯一在
  「动作发生时自动拦截」的门禁。
- 配套:`markdown-style` 技能(编辑 `.md` 前按需加载规则)。

```bash
# 手动自查单个文件
npx --yes markdownlint-cli2 "doc/gate.md"
```

补充:Claude Hook 只在**我(Claude)**用 Write/Edit 改 `.md` 时触发,覆盖不到人工手改;
人工提交由 §2.4 的 git pre-commit 门禁兜底。

### 2.4 pre-commit hook(提交层,Markdown + C 代码)

- 载体:仓库内跟踪的 `.githooks/pre-commit`(Bash 脚本),对本次暂存
  (`--diff-filter=ACMR`)的文件按类型分派检查,复用仓库根既有规则文件:

| 文件类型 | 检查 | 工具 |
| --- | --- | --- |
| `*.md` | Markdown 规范 | `markdownlint-cli2`(`.markdownlint.json`) |
| `*.c` / `*.h` | 代码风格 | `clang-format --dry-run --Werror`(`.clang-format`) |
| `*.c` / `*.h` | 常见缺陷 | `checkpatch.pl -f`(`.checkpatch.conf`) |
| `*.c` / `*.h` | 静态分析 + MISRA C:2012 | `cppcheck --addon=misra` |

- 行为:任一检查失败即非零退出、**阻断提交**;无相应文件变更则零开销跳过。
- 容错:未找到 `clang-format` / `checkpatch.pl` / `cppcheck` 时**告警跳过、不挡提交**
  (如未配 `ZEPHYR_BASE` 的机器);只有检查真失败才阻断。
- C 文件按「整文件」检查(非 diff),不依赖 `clang-format-diff.py` 软链。
- cppcheck 为**单文件、无构建上下文**的尽力扫描:已抑制 `misra-config`/`unknownMacro`
  等「缺 Zephyr 头/宏」的伪报(否则会误挡正常提交);**完整 MISRA/静态分析须靠 CI 的
  项目级 `cppcheck --project=compile_commands.json`(不加这些抑制)**。
- 启用(每个克隆一次性,`core.hooksPath` 属本地配置、不随仓库分发):

```bash
git config core.hooksPath .githooks
```

> 定位:pre-commit 是「快而可绕过的第一道」(`--no-verify` 可跳、依赖各人本机工具版本);
> C 合规的**权威最终门禁**仍应由 CI 的 `check_compliance.py` 承担(见 §4)。
> 局限:校验的是工作区磁盘文件,若某文件同时存在未暂存改动,检查到的是混合内容。

### 2.5 提交信息规范(部分落地)

- 采用 Conventional Commits:`<type>(<scope>): <subject>`。
- 已落地:`.gitmessage` 模板 + `git config commit.template=.gitmessage`,`git commit`
  时自动带出 type/scope/footer 提示。
- 尚未强制:无 commitlint、无 hook 校验,格式全靠人工遵守。
- 与 `check_compliance.py` 的取舍:上游 `GitLint` 检查按 Zephyr 规范(要求 `area:
  summary` 且强制 `Signed-off-by`),与本项目 Conventional Commits 冲突,故合规检查
  **需排除 GitLint**(`-e GitLint`),改由未来 commitlint 校验。

### 2.6 产物忽略(`.gitignore`)

拦截以下产物入库:`compliance.xml`(check_compliance JUnit 输出)、`build/`、
`twister-out/`、StrictDoc `output/`。

## 3. 对照《质量门禁体系》的差距

> 要求来源:`DevOps` 仓库 `docs/05-质量与合规/质量门禁体系.md`(三道门禁:MR / 版本 / 发布,
> 多数检查项由 CI 流水线**自动**执行)。本节逐项对照体系要求与 bms-app 实况。
> 状态:✅ 已满足 · 🟡 部分 · ❌ 缺失。

### 3.1 ① MR 门禁

| 要求项 | 阈值 | 现状 | 状态 |
| --- | --- | --- | --- |
| 编译告警 | 零新增 | 无 CI 构建,未统计告警 | ❌ |
| 静态分析(增量) | 零新违规 | pre-commit 有 cppcheck/MISRA + checkpatch,但仅本地单文件、可 `--no-verify` 绕过,无 CI 增量比对 | 🟡 |
| 单元测试 | 全通过,覆盖率≥基线 | 无单测框架与覆盖率统计 | ❌ |
| 代码评审 | ≥1 人,安全模块双人 | 未在 GitHub 配分支保护 / 必需评审 | ❌ |
| 追溯信息 | commit 带编号,MR 关联条目 | `.gitmessage` 有 `Refs:` 位但不强制;无 MR 关联校验 | 🟡 |

### 3.2 ② 版本门禁

| 要求项 | 阈值 | 现状 | 状态 |
| --- | --- | --- | --- |
| 分层回归 | 集成/SIL/HIL 全通过 | 无测试分层 | ❌ |
| 覆盖率 | 达 ASIL 等级(语句/分支/MC-DC) | 无覆盖率统计 | ❌ |
| 全量静态分析 | 无未处理阻断项,偏差留痕 | 仅 pre-commit 单文件,无项目级 `--project` 全量分析 | ❌ |
| 资源占用 | Flash/RAM 余量、栈深度 | 无资源/栈分析 | ❌ |
| 已知缺陷评估 | 无未评估高严重度缺陷 | 无缺陷管理流程 | ❌ |

### 3.3 ③ 发布门禁

| 要求项 | 阈值 | 现状 | 状态 |
| --- | --- | --- | --- |
| 质量审批 | 质量代表 + 软件经理签署 | 无审批流程 | ❌ |
| 签名与哈希 | 完成且校验通过 | 无(MCUboot 验签规划中) | ❌ |
| SBOM / CVE / 许可证 | SBOM 归档,无未评估高危 | 无 SBOM/供应链扫描 | ❌ |
| 发布物完整性 | Release Note / 追溯矩阵 / 升级说明齐备 | 仅有 `CHANGELOG.md`,其余缺 | ❌ |
| 追溯差距 | 孤立/无测试/未实现需求=0 或逐项豁免 | StrictDoc 追溯规划中,未落地 | ❌ |
| 回滚方案 | A/B 分区或回退版本确认 | 无回滚方案 | ❌ |

### 3.4 阈值原则与豁免流程

| 体系要求 | 现状 | 状态 |
| --- | --- | --- |
| 阈值即代码(写进配置、随码入库、变更走 MR) | 规则文件已入库,但无「阈值」概念(覆盖率/告警数),无 CI 配置 | 🟡 |
| 只紧不松 / 新增严于存量 / 从基线起步 | 尚无基线与阈值机制 | ❌ |
| 豁免流程(警告级 / 阻断级例外放行 + 记录) | 无豁免流程与证据留痕 | ❌ |

### 3.5 差距小结

- **结构性缺口是「无 CI」**:体系里 ① 5 项中 4 项、② 全部、③ 6 项中 5 项标注「自动」,
  其执行载体是 CI 流水线;bms-app 目前无 `.github/`,导致绝大多数门禁无处落地。
- **当前仅覆盖 ① MR 门禁的一角**:代码风格 + 单文件静态分析(本地、可绕过),约等于体系
  「静态分析(增量)」的一个弱化本地版。
- **② 版本门禁、③ 发布门禁基本空白**:无测试/覆盖率/资源分析/SBOM/签名/回滚/追溯闭环。
- **阈值即代码、豁免流程尚未建立**。
- **补强次序(对齐三道门禁)**:①先建 GitHub Actions,把 `check_compliance.py` +
  项目级 cppcheck/MISRA 变成 MR 门禁,并开启分支保护(评审);②引入 Twister 单测 + 覆盖率,
  补齐版本门禁;③再叠加签名、SBOM、追溯矩阵、回滚,构成发布门禁。

## 4. 未落地 / 规划中

| 项 | 现状 | 说明 |
| --- | --- | --- |
| GitHub Actions CI | 无 `.github/` | 目前所有 C 合规检查仅能本地手动执行,无 PR 级自动门禁 |
| commit-msg hook | 未落地 | pre-commit 已管 `.md`,但提交信息(Conventional Commits)仍无本地校验 |
| commitlint | 未安装 | Conventional Commits 仅有模板,无自动校验 |
| StrictDoc 门禁 | 规划中 | 见规范 §9.4:MR 门禁(UID 唯一/无断链 + 覆盖率不回退)、版本门禁、发布门禁 |
| MISRA / 覆盖率 gate | 规划中 | 见规范 §8 Checklist;项目级 MISRA 待随 CI 落地 |

## 5. 结论

- **已落地的都在「提交前本地」这一层**:Markdownlint(编辑 + 提交两道)、ClangFormat、
  Checkpatch、cppcheck/MISRA —— 均为 pre-commit / 本地手动,`--no-verify` 可绕过。
- **对照《质量门禁体系》,当前只覆盖 ① MR 门禁的「增量静态分析」一角**(且是本地弱化版);
  ② 版本门禁、③ 发布门禁基本空白(见 §3)。
- **最大结构性缺口:无 CI**。体系中绝大多数项要求「自动执行」,载体是 CI 流水线,
  而 bms-app 尚无 `.github/`,合规检查只能靠人自觉本地跑。
- 补强次序(对齐三道门禁,详见 §3.5):
  ①**建 GitHub Actions**——把 `check_compliance.py` + 项目级 cppcheck/MISRA 变 MR 门禁,
  开启分支保护(评审)、加 commitlint 强制提交格式;
  ②**Twister 单测 + 覆盖率**补版本门禁;
  ③**签名 / SBOM / 追溯矩阵 / 回滚**构成发布门禁。

## 6. 相关文档

- `DevOps/docs/05-质量与合规/质量门禁体系.md` —— 三道门禁要求总表、阈值原则、豁免流程(§3 对照依据)。
- `development/脚手架搭建规范.md` —— §6.1 代码风格基线、§7 提交规范、§9.4 门禁挂载规格。
- `development/构建指南.md` —— §2.1 clang-format 安装与软链前置。
- [check/C代码缺陷检查条目.md](check/C代码缺陷检查条目.md) —— checkpatch 启用/禁用条目明细。
