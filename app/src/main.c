/*
 * BMS 最小点灯示例:验证工程能编译、烧录并在 ART-Pi2 上运行。
 */
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

/* 日志模块:输出经 log 子系统分发到 UART 与 RTT 两个 backend(见 prj.conf) */
LOG_MODULE_REGISTER(bms_blinky, LOG_LEVEL_INF);

/* 板级 devicetree 里 led0 = &red_led */
#define LED0_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
	if (!gpio_is_ready_dt(&led)) {
		LOG_ERR("LED device not ready");
		return 0;
	}

	if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) {
		LOG_ERR("Failed to configure LED");
		return 0;
	}

	LOG_INF("BMS blinky start on ART-Pi2");

	uint32_t tick = 0;

	while (1) {
		gpio_pin_toggle_dt(&led);
		LOG_INF("blink #%u", tick++);
		k_msleep(3000);
	}

	return 0;
}
