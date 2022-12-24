#ifndef HASHMAP_H_INCLUDED
#define HASHMAP_H_INCLUDED

typedef struct HashMap HashMap;
typedef struct node node;

struct node {
    char* key;
    void* data;
    node* next;
};

struct HashMap
{
    size_t listsize;
    node** list;
    unsigned int (*hash_f)(char* key);
};

typedef void* (*ResolveCollissionCallback)(void* old_data, void* new_data);
typedef void (*DestroyDataCallback)(void* data);
typedef void (*CallBack)(char* key, void* data);

HashMap* create_hashmap(size_t key_space);
unsigned int hash_func(char* key);

void insert_data(HashMap* hm, char* key, void* data, ResolveCollissionCallback resolve_collision);
void* get_data(HashMap* hm, char* key);

void remove_data(HashMap* hm, char* key, DestroyDataCallback destroy_data);
void iterate(HashMap* hm, CallBack callback);
void delete_hashmap(HashMap* hm, DestroyDataCallback destroy_data);


#endif // HASHMAP_H_INCLUDED
