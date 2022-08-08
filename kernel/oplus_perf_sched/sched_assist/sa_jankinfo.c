// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2020 Oplus. All rights reserved.
 */
#if IS_ENABLED(CONFIG_OPLUS_FEATURE_CPU_JANKINFO)

#include <linux/kallsyms.h>
#include <linux/trace_events.h>
#include "sa_jankinfo.h"

enum trace_type {
	INVALID_TRACE_TYPE = -1,
	TRACE_RUNNING = 0,	/* returned by ct_state() if unknown */
	TRACE_RUNNABLE,
	TRACE_SLEEPING,
	TRACE_SLEEPING_INBINDER,
	TRACE_SLEEPING_INFUTEX,
	TRACE_DISKSLEEP,
	TRACE_DISKSLEEP_INIOWAIT,

	TRACE_DEAD,
	TRACE_CNT,
};

jank_callback_t jank_update_task_status;
EXPORT_SYMBOL(jank_update_task_status);

void jankinfo_android_rvh_enqueue_task_handler(void *unused,
			struct rq *rq, struct task_struct *p, int flags)
{
	if (!p || !jank_update_task_status)
		return;

	jank_update_task_status(p, TRACE_RUNNABLE);
}

void jankinfo_android_rvh_schedule_handler(void *unused,
			struct task_struct *prev, struct task_struct *next, struct rq *rq)
{
	unsigned long nowtype = INVALID_TRACE_TYPE;

	if (!jank_update_task_status)
		return;

	if (prev) {
		if (prev->state == TASK_RUNNING) {
			nowtype = TRACE_RUNNABLE;
		} else if (prev->state & TASK_UNINTERRUPTIBLE) {
			nowtype = TRACE_DISKSLEEP;
		} else if (prev->state & TASK_INTERRUPTIBLE) {
			nowtype = TRACE_SLEEPING;
		} else if (prev->state & TASK_DEAD) {
			/* TODO */
			nowtype = TRACE_DEAD;
		}
		jank_update_task_status(prev, nowtype);
	}

	if (next)
		jank_update_task_status(next, TRACE_RUNNING);
}

#endif
