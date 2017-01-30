# Copyright 2017 The Fuchsia Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

LOCAL_DIR := $(GET_LOCAL_DIR)

MODULE := $(LOCAL_DIR)

MODULE_TYPE := usertest

MODULE_SRCS += \
    $(LOCAL_DIR)/blobstore.cpp

MODULE_NAME := blobstore-test

MODULE_DEP := uapp/blobstore

MODULE_STATIC_LIBS := \
    ulib/merkle \
    ulib/cryptolib \

MODULE_LIBS := \
    ulib/c \
    ulib/fs-management \
    ulib/mxio \
    ulib/magenta \
    ulib/mxcpp \
    ulib/mxtl \
    ulib/unittest \

include make/module.mk
