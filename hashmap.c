#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

// Create hashmap
HashMap* create_hashmap(size_t key_space)
{
    HashMap* hm = (HashMap*) malloc(sizeof(HashMap));
    hm->listsize = key_space;
    hm->list = (node**) malloc(sizeof(node*)*key_space);

    hm->hash_f = hash_func;

    unsigned int i = 0;
    for (i = 0; i < key_space; i++)
    {
        hm->list[i] = NULL;
    }
    return hm;
}

// Function that hashes the input into a certain value
unsigned int hash_func(char* key)
{
    int sum = 0;
    int i = 0;
    for (i = 0; key[i] != '\0'; i++)
    {
        sum += key[i];
    }
    return sum;
}

// Insert data into the HashMap
void insert_data(HashMap* hm, char* key, void* data, ResolveCollissionCallback resolve_collision)
{
    int pos = hm->hash_f(key) % hm->listsize;
    node* firstnode = hm->list[pos];
    node* newnode = (node*) malloc(sizeof(node));
    node* temp = firstnode;

    while (temp)
    {
        if (strcmp(temp->key, key) == 0)
        {
            void* gooddata = resolve_collision(temp->data, data);
            temp->data = gooddata;
            free(newnode);
            newnode = NULL;
            return;
        }
        temp = temp->next;
    }

    newnode->key = (char*) malloc(strlen(key) + 1);
    memcpy(newnode->key, key, strlen(key)+1);

    newnode->data = data;
    newnode->next = firstnode;
    hm->list[pos] = newnode;
    return;
}

// Get data from the HashMap
void* get_data(HashMap* hm, char* key)
{
    int pos = hm->hash_f(key) % hm->listsize;
    node* buckethead = hm->list[pos];
    node* temp = buckethead;

    while(temp)
    {
        if (strcmp(temp->key, key) == 0)
        {
            return temp->data;
        }
        temp = temp->next;
    }
    return NULL;
}

// Remove a node
void remove_data(HashMap* hm, char* key, DestroyDataCallback destroy_data)
{
    int pos = hm->hash_f(key);
    node* firstnode = hm->list[pos];

    if (strcmp(firstnode->key, key) == 0)
    {
        hm->list[pos] = firstnode->next;
        if (destroy_data != NULL)
        {
            firstnode->data = NULL;

            destroy_data(firstnode->key);
            firstnode->key = NULL;

            destroy_data(firstnode);
            firstnode = NULL;
            return;
        } else {
            firstnode->data = NULL;

            free(firstnode->key);
            firstnode->key = NULL;

            free(firstnode);
            firstnode = NULL;
        }
        return;
    }

    node* previous = firstnode;
    node* temp = firstnode->next;

    while (temp)
    {
        if (strcmp(temp->key, key) == 0)
        {
            previous->next = temp->next;
            if (destroy_data != NULL)
            {
                temp->data = NULL;

                destroy_data(temp->key);
                temp->key = NULL;

                destroy_data(temp);
                temp = NULL;
                return;
            } else {
                temp->data = NULL;

                free(temp->key);
                temp->key = NULL;

                free(temp);
                temp = NULL;
            }

            return;
        }
        previous = temp;
        temp = temp->next;
    }
    return;
}

// Iterate over the hashmap and perform a callback function
void iterate(HashMap* hm, CallBack callback)
{
    unsigned int i = 0;
    for (i = 0; i < hm->listsize; i++)
    {
        node* nd = hm->list[i];
        node* curr;
        while ((curr = nd) != NULL)
        {
            nd = nd->next;
            callback(curr->key, curr->data);
        }
    }
    return;
}

// Delete entire hashmap
void delete_hashmap(HashMap* hm, DestroyDataCallback destroy_data)
{
    unsigned int i = 0;
    for (i = 0; i < hm->listsize; i++)
    {
        node* nd = hm->list[i];

        node* curr;
        while ((curr = nd) != NULL)
        {
            nd = nd->next;
            if (destroy_data != NULL)
            {
                curr->data = NULL;

                if (curr->key != NULL)
                {
                    destroy_data(curr->key);
                    curr->key = NULL;
                }
                destroy_data(curr);
                curr = NULL;
            } else {
                curr->data = NULL;

                if (curr->key != NULL)
                {
                    *curr->key = NULL;
                    curr->key = NULL;
                }
                free(curr);
                curr = NULL;
            }
        }
    }
    free(hm->list);
    hm->list = NULL;
    free(hm);
    hm = NULL;
    return;
}
