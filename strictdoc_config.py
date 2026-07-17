# StrictDoc 项目配置 —— 需求管理与全链追溯。
# 生成:strictdoc export . --output-dir <dir>;本地编辑:strictdoc server .
# 说明:0.27 起 strictdoc.toml 已废弃,统一用本 Python 配置。
from strictdoc.core.project_config import ProjectConfig


def create_config() -> ProjectConfig:
    return ProjectConfig(
        project_title="EnerVenue BMS —— 需求与追溯",
        # 启用的界面/能力(均为 stable 特性)
        project_features=[
            "TABLE_SCREEN",
            "TRACEABILITY_SCREEN",
            "DEEP_TRACEABILITY_SCREEN",
            "TRACEABILITY_MATRIX_SCREEN",
            "SEARCH",
            "REQUIREMENT_TO_SOURCE_TRACEABILITY",  # 需求 ↔ 源码(@relation 标记)
        ],
        # 需求/设计文档扫描路径(相对本仓库根,前导 / 表示项目根)
        include_doc_paths=[
            "/doc/requirements/**",
            "/doc/design/**",
        ],
        # 源码追溯扫描路径:含 @relation(UID, scope=...) 标记的源码与测试
        include_source_paths=[
            "/app/src/**",
            "/tests/**",
            "/drivers/**",
            "/lib/**",
        ],
        # sdoc头文件路径
        grammars={
            "@req-grammar": "doc/requirements/REQ-GRAMMAR.sgra",   # 路径相对项目根;别名必须以 @ 开头、不含点
        },
        # 只想让 StrictDoc 管 .sdoc
        exclude_doc_paths=[
            "**/*.md",
        ],
    )
