#include "map.h"
#include "header.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _mapchip {
	const char *ptr;
	void *value;
};

#define GARBAGE_INT 0xCCCCCCCC

/* it overrides; it does not overwrite. */
void insert(struct map *ptr, const char *key, void *value)
{
	struct _mapchip a;
	a.ptr = key;
	a.value = value;

	if (ptr->_alloc < ptr->_length + 1) {

		ptr->_vec =
		    realloc(ptr->_vec, ptr->_alloc * 2 * sizeof(struct _mapchip));

		ptr->_alloc *= 2;
	}

	ptr->_vec[ptr->_length] = a;
	++(ptr->_length);
}

void *lookup(const struct map m, const char *key)
{
	for (int i = (m._length) - 1; i >= 0; --i) {
		if (strcmp(m._vec[i].ptr, key) == 0) {
			return m._vec[i].value;
		}
	}
	return 0;
}

int isElem(const struct map m, const char *key) { return lookup(m, key) != 0; }

void deletion(struct map *ptr, const char *key) { insert(ptr, key, 0); }

struct map init_int_map(void)
{
	struct map res;
	res._length = 0;
	res._alloc = 256;
	res._vec = calloc(res._alloc, sizeof(struct _mapchip));
	return res;
}
