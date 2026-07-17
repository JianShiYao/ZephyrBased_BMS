/*
 * bms_core 单元测试骨架(ztest)。
 * 当前为占位用例,验证 Twister 集成可跑通;待 src/bms_core/ 实现后替换为真实断言。
 *
 * StrictDoc 追溯:本测试文件验证过压断充需求(骨架关联,真实用例落地后细化)。
 * @relation(ENV-SWE-SAFETY-001, scope=file)
 */
#include <zephyr/ztest.h>

/* 测试套件:名字建议与被测模块(src/bms_core)镜像对应 */
ZTEST_SUITE(bms_core, NULL, NULL, NULL, NULL, NULL);

/* 占位用例:恒真,仅用于打通编译/运行/报告链路 */
ZTEST(bms_core, test_placeholder)
{
	zassert_true(true, "skeleton test should always pass");
}
