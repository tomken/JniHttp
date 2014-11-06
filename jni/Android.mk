# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS := -Wall -nostartfiles -Wno-unused-parameter -fvisibility=hidden

LOCAL_LDLIBS := -llog

LOCAL_MODULE    := upload
LOCAL_SRC_FILES := JniMain.cpp HttpClient.cpp HttpPostInputChunk.cpp HttpPostFileChunk.cpp

# include $(BUILD_EXECUTABLE)
include $(BUILD_SHARED_LIBRARY)
