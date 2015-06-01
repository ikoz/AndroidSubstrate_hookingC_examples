#include <android/log.h>
#include <substrate.h>

#define LOG_TAG "SUBhook"

#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

void cigi_hook(void *orig_fcn, void* new_fcn, void **orig_fcn_ptr)
{
	MSHookFunction(orig_fcn, new_fcn, orig_fcn_ptr);
}
MSConfig(MSFilterExecutable, "/system/bin/app_process")

int (*original_arc4random)(void);
int replaced_arc4random(void)
{
    return 1234;
}
MSInitialize {
    cigi_hook((void *)arc4random,(void*)&replaced_arc4random,(void**)&original_arc4random);
}
