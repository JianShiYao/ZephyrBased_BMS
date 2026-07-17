# CLAUDE.md — ZephyrBased_BMS

## Markdown 编写规范

Markdown 编写规则已独立为 `markdown-style` 技能(见 `.claude/skills/markdown-style/`),
编写或修改 `.md` 文件时会按需自动加载,不再常驻本文件。

## C 源文件编写规范

C 源文件组织规范(foxBMS-2 风格:许可头 + doxygen `@file` 块 + 固定分节栏)已独立为
`c-source-style` 技能(见 `.claude/skills/c-source-style/`),创建或修改 `.c`/`.h`
文件时会按需自动加载。模板见 `examples/examples.c` 与 `examples/examples.h`。

## 命令执行规范

1、当执行命令时(如bash)将命令的详细解释注释在上方；
2、对配置类文件做详细注释；
