#ifndef __PGTABLE_WALK_H
#define __PGTABLE_WALK_H

#include <asm/pgtable-hwdef.h>

#define SZ_512G			0x8000000000UL

/**
 * ARMv8 D5.2.4 Memory translation granule size
 *
 * shift:
 *
 * |47    39|38   30|29   21|20   12|11         0
 * +--------+-------+-------+-------+------------+
 * |        |       |       |       |            |
 * +--------+-------+-------+-------+------------+
 *          ^       ^       ^       ^
 *          lvl0    lvl1    lvl2    lvl3
 */
#define PG_LVL0_SHIFT		39
#define PG_LVL1_SHIFT		30
#define PG_LVL2_SHIFT		21
#define PG_LVL3_SHIFT		12

/**
 * ARMv8 D5.3.3 Memory attribute fields in the VMSA8-64 translation
 * table format descriptors
 * -- Attributes fields in stage 1 VMSAv8-64 Block and Page descriptors
 *
 * Bits[7:6]:	AP:	Data Access Permission bits
 */
#define AP			(0x3 << 6)

/**
 * Read ARMv8-A system register
 */
#define read_sysreg(r) ({					\
	u64 __val;						\
	asm volatile("mrs %0, " __stringify(r) : "=r" (__val));	\
	__val;							\
})

enum pgtable_level {
	PG_LVL0 = 0,
	PG_LVL1,
	PG_LVL2,
	PG_LVL3,
};

int level_shift[] = {
	PG_LVL0_SHIFT,
	PG_LVL1_SHIFT,
	PG_LVL2_SHIFT,
	PG_LVL3_SHIFT,
};

static inline
int valid_entry(unsigned long entry)
{
	/**
	 * ARMv8 D5.3.1 VMSAv8-64 translation table level 0, level 1, and level
	 * 2 descriptor formats
	 *
	 * Invalid:
	 *
	 * |63                                        1|0|
	 * +-------------------------------------------+-+
	 * |                                           |0|
	 * +-------------------------------------------+-+
	 */
	if (entry & 1)
		return 1;
	return 0;
}

static inline
int block_entry(unsigned long entry)
{
	/**
	 * ARMv8 D5.3.1 VMSAv8-64 translation table level 0, level 1, and level
	 * 2 descriptor formats
	 *
	 * Block:
	 *
	 * |63                                       |1|0|
	 * +-----------------------------------------+-+-+
	 * |                                         |0|1|
	 * +-----------------------------------------+-+-+
	 */
	if ((entry & 0x3) == 1)
		return 1;
	return 0;
}

static inline
int valid_pte(unsigned long pte)
{
	/**
	 * ARMv8 D5.3.2 Armv8 translation table level 3 descriptor formats
	 *
	 * Page, 4KB granule:
	 *
	 * |63                                       |1|0|
	 * +-------------------------------------------+-+
	 * |                                         |1|1|
	 * +-------------------------------------------+-+
	 */
	if ((pte & 0x3) == 3)
		return 1;
	return 0;
}

static inline
int entry_is_valid(unsigned long entry, int level)
{
	if (level < PG_LVL3)
		return valid_entry(entry);
	return valid_pte(entry);
}

static inline
unsigned long next_table(unsigned long entry)
{
	return entry & PHYS_MASK & (int)PAGE_MASK;
}

#endif
