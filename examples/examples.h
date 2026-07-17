/*
 * Copyright (c) 2026 EnerVenue
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file    examples.h
 * @author  EnerVenue
 * @date    2026-07-16 (date of creation)
 * @updated 2026-07-17 (date of last update)
 * @version v1.0.0
 * @ingroup EXAMPLES
 * @prefix  BMS_CELL
 *
 * @brief   单体电压统计示例模块的接口
 * @details 演示 foxBMS-2 源文件组织规范在头文件中的应用。
 */

#ifndef BMS_APP_EXAMPLES_EXAMPLES_H_
#define BMS_APP_EXAMPLES_EXAMPLES_H_

/*========== Includes =======================================================*/
#include <stddef.h>
#include <stdint.h>

/*========== Macros and Definitions =========================================*/
/** 单体电压统计结果(单位:毫伏)。 */
struct bms_cell_stats {
	uint16_t min_mv;   /**< 最低单体电压 */
	uint16_t max_mv;   /**< 最高单体电压 */
	uint16_t avg_mv;   /**< 平均单体电压 */
	uint16_t delta_mv; /**< 极差(max - min) */
};

/*========== Extern Constant and Variable Declarations ======================*/

/*========== Extern Function Prototypes =====================================*/
/**
 * @brief 计算一组单体电压的统计值。
 *
 * @param cells_mv 单体电压数组(毫伏),非 NULL。
 * @param count    数组元素个数,须大于 0。
 * @param stats    输出统计结果,非 NULL。
 *
 * @retval 0        成功。
 * @retval -EINVAL  参数非法。
 */
int bms_cell_compute_stats(const uint16_t *cells_mv, size_t count, struct bms_cell_stats *stats);

/*========== Getter for static Variables (Unit Test) ========================*/

/*========== Externalized Static Functions Prototypes (Unit Test) ===========*/

#endif /* BMS_APP_EXAMPLES_EXAMPLES_H_ */
