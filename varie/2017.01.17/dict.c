#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dict.h"

static int dict_hash(const char *key)
{
	int i = 0;
	int hash = 0;

	for (i = 0; i < strlen(key); ++i)
		hash += key[i] * 31;

	return hash;
}

struct dict *dict_new(int max_len, size_t size)
{
	int i = 0;
	struct dict *dict = malloc(sizeof(struct dict));
	struct dict_item item = {0};
	dict->max_len = max_len;
	dict->size = size;
	dict->buckets = malloc(max_len * sizeof(struct dict_item));

	for (i = 0; i < max_len; ++i)
		dict->buckets[i] = item; //init

	return dict;
}

void dict_set(struct dict *dict, const char *key, void *value)
{
	int hash = dict_hash(key); //key gets converted into an int
	struct dict_item *bucket = dict->buckets+(hash % dict->max_len); //address position of the bucket
	struct dict_item *new_item = NULL;

	/* empty bucket */
	if (NULL == bucket->value) {
		char *k = calloc(strlen(key) + 1, sizeof(char)); //NULL terminated
		void *v = calloc(1, dict->size); //e.g. una struct, o un intero

		strcpy(k, key);
		memcpy(v, value, dict->size);

		bucket->key = k;
		bucket->value = v;
		return;
	}
	//controllo se volevamo aggiornare un valore fino a quando non raggiungo la fine della lista verticale di questo bucket
	for (;;) {
		if (strcmp(bucket->key, key) == 0) {
			void *v = calloc(1, dict->size);

			memcpy(v, value, dict->size);
			free(bucket->value);
			bucket->value = v;
			return;
		}
		if (NULL == bucket->next)
			break;

		bucket = bucket->next;
	}
	//se c'è collisione => si crea una lista verticale


	new_item = calloc(1, sizeof(struct dict_item));
	new_item->key = calloc(strlen(key) + 1, sizeof(char));
	new_item->value = calloc(1, dict->size);
	strcpy(new_item->key, key);
	memcpy(new_item->value, value, dict->size);
	bucket->next = new_item;
}

void *dict_get(struct dict *dict, const char *key)
{
	int hash = dict_hash(key);
	struct dict_item *item = dict->buckets+(hash % dict->max_len);

	while (NULL != item && NULL != item->key) {
		if (strcmp(key, item->key) == 0)
			return item->value;
		item = item->next;
	}

	return NULL;
}

void dict_rm(struct dict *dict, const char *key)
{
	int hash = dict_hash(key);
	struct dict_item *bucket = dict->buckets+(hash % dict->max_len);
	struct dict_item *curr = bucket; //ogni bucket di fatto è una lista verticale
	struct dict_item *prev = NULL;
	struct dict_item *next = curr->next;

	while (NULL != curr) {
		next = curr->next;
		if (strcmp(curr->key, key) == 0)
			break;
		prev = curr;
		curr = next;
	}

	free(curr->key);
	free(curr->value);

	if (NULL == next) { //se è l'ultimo
		curr->key = NULL;
		curr->value = NULL;
		if (NULL == prev) //se era l'unico
			return;

		if (NULL != prev->next) {
			free(prev->next);
			prev->next = NULL;
		}
	} else { //il successivo elemento nella lista verticale prende il posto dell'elemento eliminato
		curr->key = next->key;
		curr->value = next->value;
		curr->next = next->next;
		free(next);
	}

}

void dict_free(struct dict *dict)
{
	int i = 0;

	for (i = 0; i < dict->max_len; ++i) {
		struct dict_item *bucket = dict->buckets+i;
		struct dict_item *next = (dict->buckets+i)->next;
		struct dict_item *curr = NULL;

		while (NULL != next) {
			curr = next;
			next = curr->next;
			if (NULL != curr->key)
				free(curr->key);
			if (NULL != curr->value)
				free(curr->value);
			free(curr);
		}

		if (NULL != bucket->key)
			free(bucket->key);
		if (NULL != bucket->value)
			free(bucket->value);
	}

	free(dict->buckets);
	free(dict);
}

void print_them_all(struct dict *dict){

	for (int i = 0; i < dict->max_len; i++) {
		struct dict_item *bucket = dict->buckets+i;
		struct dict_item *next = (dict->buckets+i)->next;
		struct dict_item *curr = NULL;

		while (NULL != next) {
			curr = next; //shift
			next = curr->next;
			if (NULL != curr->key)
				free(curr->key);
			if (NULL != curr->value)
				free(curr->value);
			free(curr);
		}

		if (NULL != bucket->key)
			free(bucket->key);
		if (NULL != bucket->value)
			free(bucket->value);
	}
}

