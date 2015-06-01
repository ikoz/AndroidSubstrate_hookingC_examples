LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
SUBSTRATE_LIB_PATH_ARM := /home/ikoz/android/sdk/extras/saurikit/cydia_substrate/lib/armeabi
SUBSTRATE_LIB_PATH_x86 := /home/ikoz/android/sdk/extras/saurikit/cydia_substrate/lib/x86
LOCAL_MODULE    := nativeHook2.cy
LOCAL_SRC_FILES := nativeHook2.cy.cpp
LOCAL_LDLIBS := -L$(SUBSTRATE_LIB_PATH_ARM) -L$(SUBSTRATE_LIB_PATH_x86) -lsubstrate -lsubstrate-dvm -llog       
LOCAL_C_INCLUDES := /home/ikoz/android/sdk/extras/saurikit/cydia_substrate
include $(BUILD_SHARED_LIBRARY)
