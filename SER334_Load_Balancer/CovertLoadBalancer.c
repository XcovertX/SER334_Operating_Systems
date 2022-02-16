/**
 * Functions to simulate a load balancer.
 *
 * @author James Covert
 * @version 1.0
 */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "LoadBalancer.h"

struct balancer{
    struct host *host;
    struct job_node *head;
    int size;
    int requests;
    pthread_mutex_t *lock;
};

/**
 * Initializes the load balancer. Takes batch size as parameter.
 */
balancer* balancer_create(int batch_size){
    balancer *lb = (balancer*)malloc(sizeof(struct balancer));
    lb->head = (struct job_node*)malloc(sizeof(struct job_node) * batch_size);
    lb->head = NULL;
    pthread_mutex_init(&lb->lock, NULL);
    lb->host = host_create();
    lb->size = batch_size;
    lb->requests = 0;
    return lb;
}

/**
 * Shuts down the load balancer. Ensures any outstanding batches have
 * completed.
 */
void balancer_destroy(balancer** lb){
    struct balancer* l = *lb;
    pthread_mutex_lock(&l->lock);
    if(l->head != NULL){
        host_request_instance(l->host, l->head);
        l->requests = 0;
        l->head = NULL;
    }
    pthread_mutex_unlock(&l->lock);
    host_destroy(&l->host);
    free(l->head);
    l->head = NULL;
    free(*lb);
    *lb = NULL;
}

/**
 * Adds a job to the load balancer. If enough jobs have been added to fill a
 * batch, will request a new instance from InstanceHost. When job is complete,
 * *data_return will be updated with the result.
 *
 * @param user_id the id of the user making the request.
 * @param data the data the user wants to process.
 * @param data_return a pointer to a location to store the result of processing.
 */
void balancer_add_job(balancer* lb, int user_id, int data, int* data_return){
    printf("LoadBalancer: Received new job from user# %d to process data = %d and store it at %p.\n" , user_id, data, data_return);

    pthread_mutex_lock(&lb->lock);

    struct job_node *node = (struct job_node*)malloc(sizeof(struct job_node));
    node->user_id = user_id;
    node->data = data;
    node->data_result = data_return;
    node->next = lb->head;
    lb->head = node;
    lb->requests++;

    if(lb->requests == lb->size){
        host_request_instance(lb->host, lb->head);
        lb->requests = 0;
        lb->head = NULL;
    }

    pthread_mutex_unlock(&lb->lock);
}
