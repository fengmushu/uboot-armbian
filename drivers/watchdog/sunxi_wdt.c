// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2012 Samsung Electronics
 * Minkyu Kang <fengmushu@gmail.com>
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/watchdog.h>

// #define SUNXI_TIMER_BASE 	(0x01c20c00)
#define SUNXI_WDT_BASE 		(SUNXI_TIMER_BASE + 0xa0)

/* 0x10 TIMER0 */
/* 7: TIMER0[coutinue|single] */
/* 6:4 TIMER0 CLK PRES /1/2/4/8/16/32/64/128 */
/* 3:2 TIMER0 CLK Source InOSC/n(32Khz) | OSC24Mhz | N | N */
/* 1: TIMER0 reload internal value */
/* 0: TIMER0 0: stop/pause, 1: running */

#define SUNXI_WDT_IRQ_EN 	(SUNXI_WDT_BASE)
#define SUNXI_WDT_IRQ_PD 	(SUNXI_WDT_BASE + 0x04)
#define SUNXI_WDT_CTRL 		(SUNXI_WDT_BASE + 0x10)
#define SUNXI_WDT_CONF 		(SUNXI_WDT_BASE + 0x14)
#define SUNXI_WDT_MODE 		(SUNXI_WDT_BASE + 0x18)

#define SUNXI_WDT_KEY		(0xA57 < 1)

// struct sunxi_watchdog_reg {
// 	unsigned int irq_en; 		/* 0x00 1:0 - IRQ enable for TIMER1, TIMER0 */
// 	unsigned int irq_pd; 		/* 0x04 1:0 - Pending for TIMER1, TIMER0 */
// 	unsigned int reserved[2];	/* 0x08, 0x0C */
// 	unsigned int ctrl;			/* 0x10 */
// 									/* 12:1 WDT0 KEY filed, MUST: 0xA57, any other abort */
// 									/* 0: 0 - nothing, 1 - restart watchdog */
// 	unsigned int config;		/* 0x14 */
// 									/* 1:0 01 - whole system, 10 - only interrupt */
// 	unsigned int mode;			/* 0x18 */
// 									/* 7:4 WDT0 interval Source: 24M/750, Clock source must turned on */
// 										/* 0.5/1/2/3/4/5/.../16s */
// 									/* 0: 0 - nothing, 1 - Enable WDT0 */
// };

#define SUNXI_WDT_HZ 		(24000000 / 750) /* 32000 */

void hw_watchdog_stop(void)
{
	unsigned mode = readl(SUNXI_WDT_MODE);

	mode &= ~0x01;
	writel(mode, SUNXI_WDT_MODE); //stop
}

static void sunxi_wdt_start(unsigned int sec)
{
	writel(0x1, SUNXI_WDT_CONF);
	writel(0x80, SUNXI_WDT_MODE); //10s
	writel((readl(SUNXI_WDT_MODE) | 0x01), SUNXI_WDT_MODE); //start
}

/* watchdog.h api */
void hw_watchdog_init(void)
{
	printf("sunxi hw-watchdog init...\n");

	sunxi_wdt_start(10);
}

void hw_watchdog_reset(void)
{
	// puts("Zz");
	writel((readl(SUNXI_WDT_CTRL) | (SUNXI_WDT_KEY | 0x01)), SUNXI_WDT_CTRL); /* restart */
}