/**
 * Functions to simulate a cloud-like server instance host.
 *
 * @author James Covert
 * @version 1.0
 */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "InstanceHost.h"

typedef struct host{
    int instance_count;
    pthread_t *t;
};

void process_data(struct job_node* batch);

/**
* Initializes the host environment.
*/
host* host_create(){
    host *nh = (struct host *)malloc(sizeof(host));
    nh->t = (pthread_t *)malloc(sizeof(pthread_t));
    nh->instance_count = 0;
    return nh;
}

/**
* Shuts down the host environment. Ensures any outstanding batches have
* completed.
*/
void host_destroy(host** host){
    struct host* h = *host;
    free(h->t);
    h->t = NULL;
    free(*host);
    *host = NULL;
}

/**
* Creates a new server instance (i.e., thread) to handle processing the items
* contained in a batch (i.e., a listed list of job_node). InstanceHost will
* maintain a list of active instances, and if the host is requested to
* shutdown, ensures that all jobs are completed.
*
* @param h
*/
void host_request_instance(host* h, struct job_node* batch){
    printf("LoadBalancer: Received batch and spinning up new instance.\n");
    h->instance_count++;
    pthread_t *temp = h->t;
    h->t = (pthread_t *)realloc((temp), sizeof(pthread_t)*(int)h->instance_count+1);
    free(temp);
    pthread_create(&h->t[h->instance_count], NULL, process_data, (void*)batch);
    pthread_join(h->t[h->instance_count], NULL);
    h->t = NULL;
    h->instance_count--;
}

/**
 * processes the data of each job_node in batch by squaring the data.
 *
 * @param batch
 */
void process_data(struct job_node* batch){

    while(batch != NULL) {
        *batch->data_result = (batch->data) * (batch->data);
        batch = batch->next;
    }
}