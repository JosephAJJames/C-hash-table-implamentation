#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA_SIZE 150


typedef struct {
	int key;
	char* data;
}linked_list_entry;

typedef struct linked_list_node linked_list_node;

struct linked_list_node {
	linked_list_entry* data;
	linked_list_node* next;
};

typedef struct
{
	int key;
	char* data;
	linked_list_node* next;
}hash_table_entry;

typedef struct
{
	int size;
	int capacity;
	hash_table_entry** arr_pntr;
}hash_table;

int ask_user_for_size() {
	int size;

	do {
		printf("Size(<= %d): \n", MAX_DATA_SIZE);
		scanf("%d", &size);
	} while (size > MAX_DATA_SIZE);

	return size;
}

hash_table_entry* entry_init() {
	hash_table_entry* entry = (hash_table_entry*)malloc(sizeof(hash_table_entry));
	entry->key = -1;
	entry->data = (char*)malloc(sizeof(char) * 20);
	entry->next = NULL;
	return entry;
}

hash_table* table_init(int size) {
	hash_table* my_table = (hash_table*)malloc(sizeof(hash_table));
	my_table->size = size;
	my_table->capacity = 0;
	hash_table_entry** arr = (hash_table_entry**)malloc(size * sizeof(hash_table_entry));
	for (int x = 0; x < size; x++) {
		arr[x] = entry_init();
	}
	my_table->arr_pntr = arr;
	return my_table;
}


void print_entry(hash_table_entry* entry, int flag) {
	if (flag == 0) {
		printf("key: %d\n", entry->key);
	}
	if (entry->data != NULL) {
		printf("data: %s\n", entry->data);
	}
	//printf("next: %p\n", entry->next);
	printf("----------------------\n");
}

void print_linked_list_node(linked_list_node* node) {
	linked_list_entry* data = node->data;
	printf("key: %d\n", data->key);
	printf("data: %s\n", data->data);
	printf("----------------------\n");
}

void loop_through_table(hash_table* table) {
	for (int x = 0; x < table->size; x++) {
		print_entry(table->arr_pntr[x], 0);
	}
}

int print_options() {
	int option = 0;
	printf("1. Make new entry\n2. View table\n3. Get entry from key\n4. Exit\n");
	scanf("%d", &option);
	return option;
}

hash_table_entry* set_entry() {
	int key;
	char* data = (char*)malloc(sizeof(char) * 20);
	data[20] = '\0';

	printf("Key: \n");
	scanf("%d", &key);

	printf("String: (< 20 Characters)\n");
	scanf("%s", data);

	hash_table_entry* entry = entry_init();
	entry->key = key;
	strcpy(entry->data, data);
	return entry;
}

int hash_function(int num, int range) {
    int hash = 5381; 

    hash = ((hash << 5) + hash) + num; 

    return hash % range;
}

linked_list_entry* linked_list_entry_init(hash_table_entry* entry) {
	linked_list_entry* new_lle = (linked_list_entry*)malloc(sizeof(linked_list_entry));
	new_lle->data = (char*)malloc(sizeof(char) * 20);
	new_lle->data[20] = '\0';
	new_lle->key = entry->key;
	strcpy(new_lle->data, entry->data);
	return new_lle; 
}

linked_list_node* linked_list_node_init(linked_list_entry* linked_list_entry) {
	linked_list_node* new_node = (linked_list_node*)malloc(sizeof(linked_list_node));
	new_node->data = linked_list_entry;
	return new_node;
}

void traverse_and_insert(hash_table_entry* new_entry, hash_table* table, int pntr) {
	linked_list_node* next_node = table->arr_pntr[pntr]->next;
	while (next_node->next != NULL) { //traverse the linked list untill you find the end of the chain
		next_node = next_node->next;
	}
	//we are now at the end of the linked list chain
	next_node->next = linked_list_node_init(linked_list_entry_init(new_entry));
}


void resolve_collision(hash_table_entry* new_entry, hash_table* table, int pntr) {
	
	if (table->arr_pntr[pntr]->next != NULL) { //linked list had already started, this causes seg faults
		traverse_and_insert(new_entry, table, pntr);
	} else { //linked list hasnt been started
		

		table->arr_pntr[pntr]->next = linked_list_node_init(linked_list_entry_init(new_entry));
	}

}

void add_entry(hash_table_entry* entry, hash_table* table, int(*hash_func)(int, int)) {
	int pntr = hash_func(entry->key, table->size);

	if (table->arr_pntr[pntr]->key == -1) { //is the space filled
		table->arr_pntr[pntr] = entry;
		table->capacity++;

	} else { //space is filled, resolve collision
		
		resolve_collision(entry, table, pntr);
	}

}

int get_key(int range) {
	int key;
	printf("key to search by(<= %d): ", range);
	scanf("%d", &key);

	return key;
} 

void traverse_and_print(hash_table_entry* entry) {
	linked_list_node* nd = entry->next;
	while (nd != NULL) {
		print_linked_list_node(nd);
		nd = nd->next;
	}
}

void get_entrys_from_key(hash_table* my_table, int key, int(*hash_func)(int, int)) {
	int pntr = hash_func(key, my_table->size);

	printf("----------------------\n");
	print_entry(my_table->arr_pntr[pntr], 0);
	if (my_table->arr_pntr[pntr]->next != NULL) {
		traverse_and_print(my_table->arr_pntr[pntr]);
	}
}

int main() {
	int size = ask_user_for_size();

	hash_table* my_table = table_init(size);


	while (1 == 1) {
		int opt = print_options();

		if (opt == 1) {
			add_entry(set_entry(), my_table, hash_function);

		} else if (opt == 2) {
			loop_through_table(my_table);

		} else if (opt == 3) {
			get_entrys_from_key(my_table, get_key(my_table->size), hash_function);

		} else if (opt == 4) {
			return 0;
		}
	}
}