//
// Created by Shiki on 2021/4/5.
//

#ifndef NATIVEOPENGLLEARN_DEMO_INCLUDE_LOG_H_
#define NATIVEOPENGLLEARN_DEMO_INCLUDE_LOG_H_

#include <plog/Init.h>
#include <plog/Appenders/AndroidAppender.h>
#include <plog/Formatters/FuncMessageFormatter.h>
#include <plog/Log.h>
#ifdef DEMO_ANDROID

static void initLog(const char *tag = "Demo") {
  static plog::AndroidAppender<plog::FuncMessageFormatter> appender(tag);
  static plog::Logger<0> &logger = plog::init(plog::debug, &appender);
}

#endif

#endif //NATIVEOPENGLLEARN_DEMO_INCLUDE_LOG_H_
