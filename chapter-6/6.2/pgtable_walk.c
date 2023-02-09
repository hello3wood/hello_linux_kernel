#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mm_types.h>
#include <asm/memory.h>

#include "pgtable_walk.h"

static inline void
pgtable_show_pte(int idx, unsigned long entry, unsigned long vaddr)
{
	/**
	 * ARMv8 D5.4.5 Data access preimssion controls
	 *
	 * Data access permissions for stage 1 translations
	 *
	 * AP[2:1]      Access from higher level     Access from EL0
	 * 00           Read/Write                   None
	 * 01           Read/Write                   Read/Write
	 * 10           Read-only                    None
	 * 11           Read-only                    Read-only
	 */
	int ap = entry & AP;

	printk("PG-LVL3-PTE[%d]: %#16lx - %#16lx %s",
	       idx, vaddr, vaddr+SZ_4K-1, ap <= 1? "RW":"R");
}

static inline void
pgtable_show_block(enum pgtable_level level, int idx,
		   unsigned long entry, unsigned long vaddr)
{
	int ap = entry & AP;
	unsigned long size = 0;

	switch (level) {
	case PG_LVL0:
		size = SZ_512G;
		break;
	case PG_LVL1:
		size = SZ_1G;
		break;
	case PG_LVL2:
		size = SZ_2M;
		break;
	default:
		printk("[%s] bad: block level=%d", __func__, level);
		return;
	}
	printk("PG-LVL%d-BLOCK[%d]: %#16lx, %#16lx - %#16lx %s",
	       level, idx, entry, vaddr, vaddr+size-1, ap <= 1? "RW":"R");
}

/**
 * Recursively walk the page table.
 *
 * table: The page table to walk.
 * level: The level of table.
 * vaddr: Where the table maps to.
 */
static void do_pgtable_walk(unsigned long *table, enum pgtable_level level,
			    unsigned long vaddr)
{
	int i;
	unsigned long table_phys;
	unsigned long virt_addr;

	for (i = 0; i < PTRS_PER_PGD; i++) {
		unsigned long entry = table[i];

		/* 1. Is the entry valid? */
		if (!entry_is_valid(entry, level))
			continue;

		/* 2. Calculate the virtual address */
		virt_addr = vaddr | ((unsigned long)i) << level_shift[level];

		/* 3. Is PTE? */
		if (level == PG_LVL3) {
			pgtable_show_pte(i, entry, virt_addr);
			/* Only show the first valid entry in PTE */
			break;
		}

		/* 4. Is block entry? */
		if (block_entry(entry)) {
			pgtable_show_block(level, i, entry, virt_addr);
			continue;
		}

		/* 5. It's table entry */
		printk("PG-LVL%d-TABLE[%d]: %#16lx", level, i, entry);
		table_phys = next_table(entry);
		do_pgtable_walk(phys_to_virt(table_phys), level+1, virt_addr);
		/* Only show the first valid entry in level 2 table */
		if (level == PG_LVL2)
			break;
	}
}

static int __init pgtable_walk_init(void)
{
	unsigned long ttbr1;
	unsigned long *swapper_pg_dir;

	/**
	 * The TTBR1_EL1 saves the physical address of swapper_pg_dir.
	 */
	ttbr1 = read_sysreg(ttbr1_el1);
	swapper_pg_dir = phys_to_virt(ttbr1);

#if CONFIG_ARM64_VA_BITS == 48
	printk("--------------------------------");
	do_pgtable_walk(swapper_pg_dir, PG_LVL0, PAGE_OFFSET);
	printk("--------------------------------");
#elif
	printk("[%s]: Only support VA_BITS_48.", __func__);
#endif

	printk("[%s]: Hello.", __func__);
	return 0;
}

static void __exit pgtable_walk_exit(void)
{
	printk("[%s]: Bye.", __func__);
}

module_init(pgtable_walk_init)
module_exit(pgtable_walk_exit)
MODULE_LICENSE("GPL");
