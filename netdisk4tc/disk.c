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