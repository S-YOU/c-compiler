struct Map2;

struct Map {
	struct Map2 *internal;
};

void insert(struct Map *map_ptr, const char *key, void *value);
void *lookup(const struct Map map, const char *key);
struct Map init_map(void);
int isElem(const struct Map map, const char *key);
struct Map clone_map(const struct Map map);
