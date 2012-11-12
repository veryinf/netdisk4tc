#include <stdio.h>
#include "utility.h"
#include "disk.h"

static wchar_t *_config_file = L"netdisks.ini";
NDisk_Entry *available_disk_entries = NULL;
size_t available_disk_entries_length = 0;
Dictionary *available_disks = NULL;

void disk_set() {

}

void ndisk_destroy(NDisk **disk) {
    if((*disk) == NULL) {
        return;
    }
    if((*disk)->nickname != NULL) {
        free((*disk)->nickname);
        (*disk)->nickname = NULL;
    }
    if((*disk)->password != NULL) {
        free((*disk)->password);
        (*disk)->password = NULL;
    }
    if((*disk)->secret != NULL) {
        free((*disk)->secret);
        (*disk)->secret = NULL;
    }
    if((*disk)->token != NULL) {
        free((*disk)->token);
        (*disk)->token = NULL;
    }
    if((*disk)->username != NULL) {
        free((*disk)->username);
        (*disk)->username = NULL;
    }
    (*disk) = NULL;
}
