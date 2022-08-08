/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2020 Oplus. All rights reserved.
 */

#ifndef _FRAME_GROUP_H
#define _FRAME_GROUP_H

enum DYNAMIC_TRANS_TYPE {
	DYNAMIC_TRANS_BINDER = 0,
	DYNAMIC_TRANS_TYPE_MAX,
};

enum freq_update_flags {
	FRAME_FORCE_UPDATE = (1 << 0),
	FRAME_NORMAL_UPDATE = (1 << 1),
};

struct group_time {
	u64 curr_window_scale;
	u64 curr_window_exec;
	u64 prev_window_scale;
	u64 prev_window_exec;
	unsigned long normalized_util;
};

#define FRAME_ZONE       (1 << 0)
#define USER_ZONE        (1 << 1)

#define SCHED_CPUFREQ_DEF_FRAMEBOOST    (1U << 6)
#define SCHED_CPUFREQ_SF_FRAMEBOOST     (1U << 7)

extern int frame_group_init(void);
extern u64 fbg_ktime_get_ns(void);
extern void register_frame_group_vendor_hooks(void);
extern int rollover_frame_group_window(bool composition);
extern void set_frame_group_window_size(unsigned int window);
extern void set_ui_thread(int pid, int tid);
extern void set_render_thread(int pid, int tid);
extern void set_sf_thread(int pid, int tid);
extern void set_renderengine_thread(int pid, int tid);
extern int get_frame_group_ui(void);
extern bool default_group_update_cpufreq(void);

extern bool fbg_freq_policy_util(unsigned int policy_flags, const struct cpumask *query_cpus,
	unsigned long *util);
extern bool set_frame_group_task_to_perfer_cpu(struct task_struct *p, int *target_cpu);
extern bool fbg_need_up_migration(struct task_struct *p, struct rq *rq);
extern bool fbg_skip_migration(struct task_struct *tsk, int src_cpu, int dst_cpu);
extern bool fbg_skip_rt_sync(struct rq *rq, struct task_struct *p, bool *sync);
extern bool check_putil_over_thresh(unsigned long thresh);
extern bool fbg_rt_task_fits_capacity(struct task_struct *tsk, int cpu);

extern int info_show(struct seq_file *m, void *v);
#endif /* _FRAME_GROUP_H */
