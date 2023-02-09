/**
 * Output:
 *
 * schedule task=1000
 * We have task ID=1001 task ID=1002 task ID=1003 task ID=1004 to schedule.
 */
#include <stdio.h>

#include "rbtree.h"

struct task {
	unsigned id;
	struct rb_node rb_node;
};

#define TASK_CNT	5
#define START_PID	1000

static struct task tasks[TASK_CNT];

static int add_task(struct rb_root *root, struct task *new_task)
{
	struct rb_node **p = &root->rb_node, *parent = NULL;
	struct task *task;

	while (*p) {
		parent = *p;
		task = rb_entry(parent, struct task, rb_node);
		if (new_task->id < task->id) {
			p = &parent->rb_left;
		} else if (new_task->id > task->id) {
			p = &parent->rb_right;
		} else {
			printf("id conflict: id=%d\n", new_task->id);
			return -1;
		}
	}
	/* Insert new_task to rbtree */
	return 0;
}

static struct task *search_task(struct rb_root *root, unsigned id)
{
	struct rb_node *node = root->rb_node;
	struct task *task;

	while (node) {
		task = rb_entry(node, struct task, rb_node);
		/* Where to go? */
		if (id < task->id) {
		} else if (id > task->id) {
		} else {
		}
	}
	return NULL;
}

static void schedule(struct task *task)
{
	if (task)
		printf("schedule task=%d\n", task->id);
}

int main(void)
{
	int i;
	struct task *task;
	struct rb_node *node;
	struct rb_root scheduler = RB_ROOT;

	/* Add every task to scheduler */
	for (i = 0; i < TASK_CNT; i++) {
		tasks[i].id = START_PID + i;
		add_task(&scheduler, tasks+i);
	}

	/* Schedule one task */
	task = search_task(&scheduler, START_PID);

	schedule(task);

	/* Remove task */

	/* List remaining tasks*/
	printf("We have ");
	for () {
		task= rb_entry(node, struct task, rb_node);
		printf("ID=%d ", task->id);
	}
	printf("tasks to schedule.\n");
	return 0;
}
