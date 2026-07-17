# Changelog

本文件记录 BMS 应用(`bms-app`)的版本变更。
格式遵循 [Keep a Changelog](https://keepachangelog.com/zh-CN/1.1.0/),版本号遵循 [语义化版本](https://semver.org/lang/zh-CN/)。
版本号与 `app/VERSION` 保持一致;每个正式 release tag 需可追溯到:锁定的 manifest 版本组合 + Twister 测试报告 + MISRA 扫描报告(见 `doc/development/脚手架搭建规范.md` §7)。

## [Unreleased]

### Added

- 工程骨架(框架层):`tests/bms_core/` 最小 ztest(Twister 在 native_sim 跑通)、
  `app/prj_release.conf` / `app/prj_debug.conf` 配置覆盖层、`doc/requirements/`(EARS 模板)、
  `include/` `lib/` `drivers/` 占位。
- 提交规范:采用 Conventional Commits(规范 §7.2),提供 `.gitmessage` 提交模板。
- 需求管理:引入 StrictDoc `0.27.0`(经 `zephyr/module.yml` + `west packages pip` 锁版本安装);
  `strictdoc_config.py` + `doc/requirements/bms.sdoc`(自定义 grammar)+ `doc/design/bms_design.sdoc`,
  打通 需求→设计→测试 追溯(见规范 §9)。

## [0.1.0] - 2026-07-10

### Added

- ART-Pi2(STM32H7R7L8)点灯基线应用:GPIO + printk + 日志双后端(UART / SEGGER RTT)。
- 工程脚手架:T3 拓扑(独立 `bms-manifest`),`app/` 应用 + 仓库根 Zephyr module(`zephyr/module.yml`)。
- 代码合规基线(阶段 1):`.clang-format`、`.checkpatch.conf`、`Compliance Check (app)` VS Code task。
- 开发文档:构建与烧录指南、脚手架搭建规范。
