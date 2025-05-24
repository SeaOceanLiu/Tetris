LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

# Add your application source files here...
LOCAL_SRC_FILES := \
    Actor.cpp \
    AnchorPoint.cpp \
    Animation.cpp \
    Arena.cpp \
    AudioSuite.cpp \
    BlockData.cpp \
    BlockMoveTracker.cpp \
    Brick.cpp \
    ConstDef.cpp \
    DebugTrace.cpp \
    EventQueue.cpp \
    FadeTracker.cpp \
    FontSuite.cpp \
    InputControl.cpp \
    Interface.cpp \
    main.cpp \
    Material.cpp \
    PhotoCarousel.cpp \
    Timer.cpp \
    tinyxml2.cpp

SDL_PATH := ../SDL  # SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include  # SDL

LOCAL_SHARED_LIBRARIES := SDL3

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid  # SDL

include $(BUILD_SHARED_LIBRARY)
