/*
 * Copyright (c) 2026 EnerVenue
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file    examples.c
 * @author  EnerVenue
 * @date    2026-07-16 (date of creation)
 * @updated 2026-07-17 (date of last update)
 * @version v1.0.0
 * @ingroup EXAMPLES
 * @prefix  BMS_CELL
 *
 * @brief   单体电压统计示例模块的实现
 * @details 演示 foxBMS-2 源文件组织规范在源文件中的应用。
 */

/*========== Includes =======================================================*/
#include "examples.h"

#include <errno.h>

/*========== Macros and Definitions =========================================*/

/*========== Static Constant and Variable Definitions =======================*/

/*========== Extern Constant and Variable Definitions =======================*/

/*========== Static Function Prototypes =====================================*/

/*========== Static Function Implementations ================================*/

/*========== Extern Function Implementations ================================*/
int bms_cell_compute_stats(const uint16_t *cells_mv, size_t count, struct bms_cell_stats *stats)
{
	if ((cells_mv == NULL) || (stats == NULL) || (count == 0U)) {
		return -EINVAL;
	}

	uint16_t min_mv = cells_mv[0];
	uint16_t max_mv = cells_mv[0];
	uint32_t sum_mv = 0U;

	for (size_t i = 0; i < count; i++) {
		uint16_t v = cells_mv[i];

		if (v < min_mv) {
			min_mv = v;
		}

		if (v > max_mv) {
			max_mv = v;
		}

		sum_mv += v;
	}

	stats->min_mv = min_mv;
	stats->max_mv = max_mv;
	stats->avg_mv = (uint16_t)(sum_mv / count);
	stats->delta_mv = (uint16_t)(max_mv - min_mv);

	return 0;
}

/*========== Externalized Static Function Implementations (Unit Test) =======*/
