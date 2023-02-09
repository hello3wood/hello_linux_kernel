#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/module.h>

#define SLAB_SZ		256

static struct kmem_cache *mcache;

static int __init slub_test_init(void)
{
	char *mem;

	mcache = kmem_cache_create("mcache", SLAB_SZ, 0, 0, NULL);
	if (mcache == NULL) {
		printk("[%s]: Alloc mcache failed!", __func__);
		return -ENOMEM;
	}

	mem = kmem_cache_alloc(mcache, GFP_KERNEL);
	if (IS_ERR_OR_NULL(mem)) {
		kmem_cache_destroy(mcache);
		printk("[%s] Alloc mem failed!", __func__);
		return -ENOMEM;
	}

	memset(mem, 0, SLAB_SZ);
	snprintf(mem, SLAB_SZ, "Hello slub!");
	printk("[%s]: %s", __func__, mem);
	kmem_cache_free(mcache, mem);
	printk("[%s]: OK!", __func__);
	return 0;
}

static void __exit slub_test_exit(void)
{
	kmem_cache_destroy(mcache);
	printk("[%s]: OK!", __func__);
}

module_init(slub_test_init)
module_exit(slub_test_exit)
MODULE_LICENSE("GPL");
