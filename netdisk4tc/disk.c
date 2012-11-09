#include <stdio.h>
#include "utility.h"
#include "disk.h"

static wchar_t *_config_file = L"netdisks.ini";
NDISK_TYPE available_disk_types[] = {
    {NDISK_SKYDRIVE, L"Microsoft SkyDrive", L"https://signup.live.com/signup.aspx"},
    {NDISK_KUAIPAN, L"金山快盘", L"http://www.kuaipan.cn/account_register.htm"},
    {NDISK_VDISK, L"新浪微盘", L"http://weibo.com/signup/signup.php"}
};

DICTIONARY *available_disks = NULL;

void disk_set() {

}

void ndisk_destroy(NDISK **disk) {
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
