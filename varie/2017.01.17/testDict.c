#include <stdio.h>

#include "dict.h"

struct drink {
	char name[32];
	int price;
	int mls;
};

void rm(struct dict *dd, const char *key)
{
	void *val = NULL;
	dict_rm(dd, key);
	val = dict_get(dd, key);

	val = NULL;
	val = dict_get(dd, key);

	if (NULL == val)
		printf("%s is not found\n", key);
	else
		printf("%s is still there\n", key);
}

int main(void)
{
	struct dict *dd = dict_new(3, sizeof(int));

	int apple = 23;
	int orange = 42;
	int grape = 70;
	int grape_2 = 54;
	int cherry = 31;
	int banana = 80;

	struct drink whiskey = { "Whiskey", 16, 40 };
	struct drink coffee = { "Coffee", 5, 350 };
	struct drink beer = { "Beer", 9, 425 };

	struct drink *drink = NULL;

	dict_set(dd, "Apple", &apple);
	dict_set(dd, "Orange", &orange);
	dict_set(dd, "Grape", &grape);
	dict_set(dd, "Cherry", &cherry);
	dict_set(dd, "Banana", &banana);

	printf("%s %d\n", "Apple", *(int*)dict_get(dd, "Apple"));
	printf("%s %d\n", "Orange", *(int*)dict_get(dd, "Orange"));
	printf("%s %d\n", "Grape", *(int*)dict_get(dd, "Grape"));
	printf("%s %d\n", "Cherry", *(int*)dict_get(dd, "Cherry"));
	printf("%s %d\n", "Banana", *(int*)dict_get(dd, "Banana"));

	dict_set(dd, "Grape", &grape_2);

	printf("%s %d\n", "Grape", *(int*)dict_get(dd, "Grape"));

	/* rm from end */
	rm(dd, "Banana");

	/* rm from beginning */
	rm(dd, "Orange");

	/* rm from middle */
	rm(dd, "Grape");

	dict_free(dd);

	dd = dict_new(3, sizeof(struct drink));

	dict_set(dd, whiskey.name, &whiskey);
	dict_set(dd, "Coffee", &coffee);
	dict_set(dd, beer.name, &beer);

	drink = (struct drink*)dict_get(dd, whiskey.name);
	printf("%s %d %d\n", "Whiskey", drink->price, drink->mls);

	drink = (struct drink*)dict_get(dd, coffee.name);
	printf("%s %d %d\n", coffee.name, drink->price, drink->mls);

	drink = (struct drink*)dict_get(dd, beer.name);
	printf("%s %d %d\n", "B E E R", drink->price, drink->mls);

	dict_free(dd);

	return 0;
}
