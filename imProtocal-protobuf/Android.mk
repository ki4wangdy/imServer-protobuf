
LOCAL_PATH := $(call my-dir)

LOCAL_MODULE := libimProtocal
LOCAL_MODULE_FILENAME := libimProtocal

LOCAL_SRC_FILES := \
	src/message.cpp	\
	src/message.pb.cpp	\
	src/header.pb.cpp

LOCAL_C_INCLUDES :=	$(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDES :=	$(LOCAL_PATH)/include

#LOCAL_CFLAGS := 
#LOCAL_EXPORT_CFLAGS := 

LOCAL_WHOLE_STATIC_LIBRARIES := libprotobuf 

LOCAL_LDLIBS := -lz

include $(BUILD_SHARED_LIBRARY)

# main execute 
# =======================================================
include $(CLEAR_VARS)

LOCAL_MODULE := android_im_protocal_buf_exe
LOCAL_MODULE_FILENAME := android_im_protocal_buf_exe

LOCAL_SRC_FILES := src/main.c

LOCAL_C_INCLUDES :=	$(LOCAL_PATH)/include
LOCAL_EXPORT_C_INCLUDES :=	$(LOCAL_PATH)/include

LOCAL_STATIC_LIBRARIES := libprotobuf libimProtocal

include $(BUILD_EXECUTABLE)

$(call import-module,protobuf-2.5.0)

