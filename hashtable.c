/* This hash table has been adapted by Abhisha Nirmalathas.
	The original hash table it was adapted from was made
 by Matt Farrugia <matt.farrugia@unimelb.edu.au, for Design of Algorithms.
 This hash table uses separate chaining with an array.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hashtable.h"

typedef struct bucket Bucket;
struct bucket {
	char **keys; //array of words
	int *values; //array of weighted frequencies, lower value is higher frequency
	int capacity; //space available in bucket
	int n_elems; //number of elements in bucket
};

struct table {
	int size; //number of buckets
	Bucket *buckets; //list of buckets
    int remaining; // checks number of remaining elements
};

/************************************************************************/
// moves recently accessed key & value to the front of bucket
void move_to_front(Bucket *bucket, int hash_value);

 unsigned int seed = 73802;
 unsigned int xor_hash(const char *key, unsigned int size) {
 	unsigned int h = seed;

 	int i;
 	for (i = 0; key[i] != '\0'; i++) {
 		h = h ^ ((h << 5) + key[i] + (h >> 2));
 	}

 	return h % size;
 }
/************************************************************************/
Bucket *add_to_bucket(Bucket *bucket, char *key, int value) {
	/*Adds new key and value to existing bucket*/
	bucket->n_elems = bucket->n_elems + 1;
	// allocates more memory if not enough space
	if (bucket->capacity <= bucket->n_elems){
		bucket->capacity = 2*(bucket->capacity + 1);
		bucket->keys = realloc(bucket->keys,sizeof(char*) * (bucket->capacity));
		bucket->values = realloc(bucket->values,sizeof(int) * (bucket->capacity));
	}
	assert(bucket->keys);
	assert(bucket->values);
	//adds new key and avlue to end of bucket
	bucket->values[bucket->n_elems-1] = value;
	bucket->keys[bucket->n_elems-1] = key;
	return bucket;
}
/************************************************************************/
// Warning: does not free bucket->next
void free_bucket(Bucket bucket) {
	/*Deallocates memory in bucket*/
	free(bucket.keys);
	free(bucket.values);
}


/************************************************************************/
HashTable *new_hash_table(int size) {
	/*Builds a new hash table of input size*/
	HashTable *table = malloc(sizeof *table);
	assert(table);
	table->size = size;
	table->buckets = malloc(size * (sizeof *table->buckets));
    table->remaining = size;
	assert(table->buckets);
	int i;
	for (i = 0; i < size; i++) {
		table->buckets[i].keys = malloc(sizeof(char*));
		table->buckets[i].values = malloc(sizeof(int));
		table->buckets[i].capacity = 0;
		table->buckets[i].n_elems = 0;
	}
	return table;
}
/************************************************************************/
void free_hash_table(HashTable *table) {
	/*Deallocates memory from hash table*/
	assert(table != NULL);
	int i;
	for (i = 0; i < table->size; i++) {
		free_bucket(table->buckets[i]);
	}
	free(table->buckets);
	free(table);
}

/************************************************************************/
bool equal(char *a, char *b) {
	/*Returns if a string is equal to another string*/
	return strcmp(a, b) == 0;
}

/************************************************************************/
void hash_table_put(HashTable *table, char *key, int value) {
	/*Inserts new key into hash table with weighted value*/
	assert(table != NULL);
	assert(key != NULL);
	int hash_value = xor_hash(key, table->size);
	// look for existing key
	Bucket *bucket = &table->buckets[hash_value];
	for (int i=0; i < bucket->n_elems; i++){
		if (equal(bucket->keys[i], key)){
			bucket->values[i] = value;
			move_to_front(bucket, i);
			return;
		}
	}
	// adds key and value as not in hash table
	bucket = add_to_bucket(bucket, key, value);
	table->buckets[hash_value] = *bucket;
	move_to_front(bucket, bucket->n_elems - 1);
}
/************************************************************************/
void move_to_front(Bucket *bucket, int index){
	/*moves recently accessed key & value to the front of bucket*/
	char* temp_key;
	int temp_value;
	if (bucket->n_elems <= 1 && index >= bucket->n_elems){
		return;
	}
	// swaps position of first element with recently accessed
	temp_key = bucket->keys[index];
	temp_value = bucket->values[index];
	bucket->keys[index] = bucket->keys[0];
	bucket->values[index] = bucket->values[0];
	bucket->values[0] = temp_value;
	bucket->keys[0] = temp_key;
}
/************************************************************************/
int hash_table_get(HashTable *table, char *key) {
	/*Checks if key in hash table and returns value if present*/
	int freq = -1;
	assert(table != NULL);
	assert(key != NULL);

	int hash_value = xor_hash(key, table->size);

	// look for existing key
	Bucket *bucket = &table->buckets[hash_value];
	for (int i=0; i < bucket->n_elems; i++){
		if (equal(bucket->keys[i], key)){
			freq = bucket->values[i];
            bucket->keys[i] = "";
			move_to_front(bucket, i);
            table->remaining--;
			break;
		}
	}
	//returns value of key if found, else return -1
	return freq;
}
/************************************************************************/
bool hash_table_has(HashTable *table, char *key) {
	/*Checks if the key exists in hash table*/
	assert(table != NULL);
	assert(key != NULL);

	int hash_value = xor_hash(key, table->size);

	// look for existing key
	Bucket *bucket = &table->buckets[hash_value];
	for (int i=0; i < bucket->n_elems; i++){
		if (equal(bucket->keys[i], key)){
            // reset hash to avoid duplicate keys
            bucket->keys[i] = "";
            table->remaining--;
			return true;
		}
	}
	return false;
}
/************************************************************************/
int remaining_hashes(HashTable *table){
	/* Returns remaining elements in hash table*/
    return table->remaining;
}