/**
 * Loads data files
 *
 * completion time 1hr
 * @author James Covert
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include "DataLoader.h"

/**
 * Loads a test_scenario strut from a textfile.
 *
 * @param filename The name of the file to load.
 * @return A struct containing the loaded file.
 */
struct test_scenario* load_test_data(char* filename){
    struct test_scenario *ts = (struct test_scenario*)malloc(sizeof(struct test_scenario));
    FILE *file = fopen(filename, "r");
    fscanf(file, "%d", &ts->page_count);
    fscanf(file, "%d", &ts->frame_count);
    ts->refstr_len = 0;
    while(fscanf(file, "%d", &ts->refstr[ts->refstr_len]) != EOF) {
        printf("%dth page in reference string\n", ts->refstr[ts->refstr_len]);
        ts->refstr_len++;
    }
    return ts;
}
