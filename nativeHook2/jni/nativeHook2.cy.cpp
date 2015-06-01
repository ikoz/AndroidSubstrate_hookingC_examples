#include <android/log.h>
#include <substrate.h>
#include <stdio.h>

#define LOG_TAG "SUBhook"

#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

void cigi_hook(void *orig_fcn, void* new_fcn, void **orig_fcn_ptr)
{
	MSHookFunction(orig_fcn, new_fcn, orig_fcn_ptr);
}
MSConfig(MSFilterExecutable, "/system/bin/app_process")

int (*original_getAge)(void);
int replaced_getAge(void) {
    return 99;
}
int (*original_arc4random)(void);
int replaced_arc4random(void)
{
    return 1234;
}
void* lookup_symbol(char* libraryname,char* symbolname)
{
	void *imagehandle = dlopen(libraryname, RTLD_GLOBAL | RTLD_NOW);
	if (imagehandle != NULL){
		void * sym = dlsym(imagehandle, symbolname);
		if (sym != NULL){
			return sym;
			}
		else{
			LOGI("(lookup_symbol) dlsym didn't work");
			return NULL;
		}
	}
	else{
		LOGI("(lookup_symbol) dlerror: %s",dlerror());
		return NULL;
	}
}
MSInitialize {
  
    cigi_hook((void *)arc4random,(void*)&replaced_arc4random,(void**)&original_arc4random);
    void * getAgeSym = lookup_symbol("/data/data/io.koz.targetApp2/lib/libtargetLib.so","getAge");
    cigi_hook(getAgeSym,(void*)&replaced_getAge,(void**)&original_getAge);

}
