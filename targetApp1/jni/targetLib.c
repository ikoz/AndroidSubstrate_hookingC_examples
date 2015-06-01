#include <string.h>
#include <jni.h>
#include <android/log.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, "targetApp1-native", __VA_ARGS__)
int getAge(void)
{
    LOGI("[i] Verbose - getAge located at %p\n", &getAge);
    return 21;
}
jstring
Java_io_koz_targetApp1_targetApp1_doThings( JNIEnv* env,
                                                  jobject thiz )
{
    int r = arc4random() % 10000;
    LOGI("[+] John Smith is %d years old.\n", getAge());
    LOGI("[+] The totally reliable random seed is: %d\n\n", r);
    return 0;
}
