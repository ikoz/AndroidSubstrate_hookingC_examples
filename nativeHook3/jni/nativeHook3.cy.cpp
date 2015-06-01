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


void * get_base_of_lib_from_maps(char *soname)
{
  void *imagehandle = dlopen(soname, RTLD_LOCAL | RTLD_LAZY);
  if (soname == NULL)
    return NULL;
  if (imagehandle == NULL){
	  return NULL;
  }
  uintptr_t * irc = NULL;
  FILE *f = NULL;
  char line[200] = {0};
  char *state = NULL;
  char *tok = NULL;
  char * baseAddr = NULL;
  if ((f = fopen("/proc/self/maps", "r")) == NULL)
    return NULL;
  while (fgets(line, 199, f) != NULL)
  {
    tok = strtok_r(line, "-", &state);
    baseAddr = tok;
    tok = strtok_r(NULL, "\t ", &state);
    tok = strtok_r(NULL, "\t ", &state); // "r-xp" field
    tok = strtok_r(NULL, "\t ", &state); // "0000000" field
    tok = strtok_r(NULL, "\t ", &state); // "01:02" field
    tok = strtok_r(NULL, "\t ", &state); // "133224" field
    tok = strtok_r(NULL, "\t ", &state); // path field

    if (tok != NULL) {
      int i;
      for (i = (int)strlen(tok)-1; i >= 0; --i) {
        if (!(tok[i] == ' ' || tok[i] == '\r' || tok[i] == '\n' || tok[i] == '\t'))
          break;
        tok[i] = 0;
      }
      {
        size_t toklen = strlen(tok);
		size_t solen = strlen(soname);
		if (toklen > 0) {
		  if (toklen >= solen && strcmp(tok + (toklen - solen), soname) == 0) {
			fclose(f);
            return (uintptr_t*)strtoll(baseAddr,NULL,16);
		  }
		}
      }
    }
  }
  fclose(f);
  return NULL;
}

void * get_base_of_lib_from_soinfo(char *soname)
{
  if (soname == NULL)
    return NULL;
  void *imagehandle = dlopen(soname, RTLD_LOCAL | RTLD_LAZY);
  if (imagehandle == NULL){
          return NULL;
  }
        char *basename;
        char *searchname;
        int i;
        void * libdl_ptr=dlopen("libdl.so",3);
        basename = strrchr(soname,'/');
        searchname = basename ? basename +1 : soname;
        for(i =(int) libdl_ptr; i!=NULL; i=*(int*)(i+164)){
                if(!strcmp(searchname,(char*)i)){
                        unsigned int *lbase= (unsigned int*)i+140;
                        void * baseaddr = (void*)*lbase;
                        return baseaddr;
                }
        }
        return NULL;
}

MSInitialize {
    cigi_hook((void *)arc4random,(void*)&replaced_arc4random,(void**)&original_arc4random);
	void* lib_base = get_base_of_lib_from_maps("/data/app-lib/io.koz.targetApp2-1/libtargetLib.so");
//OR
//    void* lib_base = get_base_of_lib_from_soinfo("/data/app-lib/io.koz.targetApp2-1/libtargetLib.so");
    
	LOGI("lib base is %p",lib_base);
	if (lib_base!=NULL){
		void * getAgeSym = lib_base + 0xd2d;
		LOGI("getAge() should be at %p. Let's hook it",getAgeSym);
		cigi_hook(getAgeSym,(void*)&replaced_getAge,(void**)&original_getAge);
}
