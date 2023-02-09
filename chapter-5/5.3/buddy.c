#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define MEM_ORDER	1

static int __init buddy_test_init(void)
{
	char *mem;
	size_t mem_sz;
	struct page *p;

	p = alloc_pages(GFP_KERNEL | __GFP_ZERO, MEM_ORDER);
	if (p == NULL) {
		printk("[%s]: alloc pages failed", __func__);
		return -ENOMEM;
	}
	mem = page_to_virt(p);
	mem_sz = (1UL << MEM_ORDER) * PAGE_SIZE;
	snprintf(mem, mem_sz, "Hello buddy test!");
	printk("[%s]: %s", __func__, mem);
	printk("[%s]: OK!", __func__);
	__free_pages(p, MEM_ORDER);
	return 0;
}

static void __exit buddy_test_exit(void)
{
	printk("[%s]: OK!", __func__);
}

module_init(buddy_test_init)
module_exit(buddy_test_exit)
MODULE_LICENSE("GPL");
