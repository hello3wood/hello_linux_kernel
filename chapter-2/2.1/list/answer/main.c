/**
 * This program:
 *  (1) Adds "tiger", "lion", "hippo" and "giraffe" to the zoo.
 *  (2) Tours the zoo and prints all animals.
 *  (3) Remove all animals.
 */
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

struct animal {
	char comm[16]; // name
	struct list_head entry;
};

static int add_animal(struct list_head *zoo, char *name)
{
	int name_len;
	struct animal *ai;

	if (!zoo || !name)
	      return -1;

	ai = malloc(sizeof(*ai));
	if (ai == NULL) {
		printf("[-] allocate animal failed: %s\n", strerror(errno));
		return -1;
	}

	name_len = strlen(name);
	if (name_len > 16)
	      name_len = 16;
	strncpy(ai->comm, name, name_len);
	ai->comm[15] = '\0';
	list_add(&ai->entry, zoo);
	return 0;
}

static void tour_animals(struct list_head *zoo)
{
	struct list_head *iter;
	struct animal *ai;

	list_for_each(iter, zoo) {
		ai = list_entry(iter, struct animal, entry);
		printf("[+] animal: %s\n", ai->comm);
	}
}

static void remove_animals(struct list_head *zoo)
{
	struct list_head *iter, *tmp;
	struct animal *ai;

	list_for_each_safe(iter, tmp, zoo) {
		ai = list_entry(iter, struct animal, entry);
		list_del(iter);
		printf("[+] remove animal: %s\n", ai->comm);
		free(ai);
	}
}

int main(void)
{
	LIST_HEAD(zoo);

	add_animal(&zoo, "tiger");
	add_animal(&zoo, "lion");
	add_animal(&zoo, "hippo");
	add_animal(&zoo, "giraffe");

	tour_animals(&zoo);
	remove_animals(&zoo);
	return 0;
}
