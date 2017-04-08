/*
 * Copyright (C) 2005 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//
// C/C++ logging functions.  See the logging documentation for API details.
//
// We'd like these to be available from C code (in case we import some from
// somewhere), so this has a C interface.
//
// The output will be correct when the log file is shared between multiple
// threads and/or multiple processes so long as the operating system
// supports O_APPEND.  These calls have mutex-protected data structures
// and so are NOT reentrant.  Do not use LOG in a signal handler.
//
#ifndef _LIBS_UTILS_LOG_H
#define _LIBS_UTILS_LOG_H

#include <QtGlobal>

#define LOCAL_LOG

#ifdef LOCAL_LOG
#define ALOGDTRACE()            qDebug("[D][%s] Function[%s]", LOG_TAG, __FUNCTION__)
#endif

#ifndef LOG_NDEBUG
#define LOG_NDEBUG 0
#endif

#if LOG_NDEBUG
#ifdef QT_NO_DEBUG
#define LOG_LEVEL 2
#else
#define LOG_LEVEL 3
#endif // QT_NO_DEBUG
#else
#define LOG_LEVEL 4
#endif // LOG_NDEBUG

#if LOG_LEVEL >= 4
#define ALOGV(x, ...)           qDebug("[V][%s] " x, LOG_TAG, ##__VA_ARGS__)
#else
#define ALOGV(x, ...)
#endif

#if LOG_LEVEL >= 3
#define ALOGD(x, ...)           qDebug("[D][%s] " x, LOG_TAG, ##__VA_ARGS__)
#else
#define ALOGD(x, ...)
#define ALOGDTRACE()
#endif

#if LOG_LEVEL >= 2
#define ALOGI(x, ...)           qDebug("[I][%s] " x, LOG_TAG, ##__VA_ARGS__)
#else
#define ALOGI(x, ...)
#endif

#if LOG_LEVEL >= 1
#define ALOGW(x, ...)           qWarning("[W][%s] " x, LOG_TAG, ##__VA_ARGS__)
#else
#define ALOGW(x, ...)
#endif

#if LOG_LEVEL >= 0
#define ALOGE(x, ...)           qCritical("[E][%s] " x, LOG_TAG, ##__VA_ARGS__)
#else
#define ALOGE(x, ...)
#endif

#define LOG_ALWAYS_FATAL    qFatal

#define LOG_FATAL_IF(a, fmt, args...)               \
    if ((a)) {                                      \
        qFatal((fmt), ##args);                      \
    }

#define LOG_ALWAYS_FATAL_IF(a, fmt, args...)        \
    if ((a)) {                                      \
        qFatal((fmt), ##args);                      \
    }

#ifndef QT_NO_DEBUG
#define ALOG_ASSERT(a, fmt, args...)                \
    if (!(a)) {                                     \
        qFatal((fmt), ##args);                      \
    }
#else
#define ALOG_ASSERT(a, fmt, args...)
#endif

#define ALOGW_IF(a, fmt, args...)                   \
    if ((a)) {                                      \
        qWarning((fmt), ##args);                    \
    }

#endif // _LIBS_UTILS_LOG_H
