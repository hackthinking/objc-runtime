/*	NSObjCRuntime.h
	Copyright (c) 1994-2012, Apple Inc. All rights reserved.
*/

#ifndef _OBJC_NSOBJCRUNTIME_H_
#define _OBJC_NSOBJCRUNTIME_H_

#include <TargetConditionals.h>
#include <objc/objc.h>

//64位判断
#if __LP64__ || (TARGET_OS_EMBEDDED && !TARGET_OS_IPHONE) || TARGET_OS_WIN32 || NS_BUILD_32_LIKE_64
    //定义64位
    typedef long NSInteger;
    typedef unsigned long NSUInteger;
#else
    //定义32位
    typedef int NSInteger;
    typedef unsigned int NSUInteger;
#endif

//定义极限数值
#define NSIntegerMax    LONG_MAX
#define NSIntegerMin    LONG_MIN
#define NSUIntegerMax   ULONG_MAX

//标示 NSINTEGER 已定义
#define NSINTEGER_DEFINED 1

// 指定初始化构造器 (子类重写父类的指定构造方法, 则必须使用super调用父类的指定构造方法)
// 参考阅读：http://useyourloaf.com/blog/xcode-6-objective-c-modernization/
// 参考阅读：https://segmentfault.com/a/1190000004116196?spm=5176.100239.blogcont5847.7.KVKJxo
#ifndef NS_DESIGNATED_INITIALIZER

    //判断clang 中的 objc_designated_initializer 属性
    #if __has_attribute(objc_designated_initializer)

        // __attribute__ 是一个编译器指令，指定了声明的特性，它允许更丰富的错误检查和高级优化
        // 参考阅读：http://nshipster.com/__attribute__/
        // 参考阅读：http://honglu.me/2016/03/04/Cocoa中常用的宏定义/
        // 参考阅读：http://blog.sunnyxx.com/2014/09/15/objc-attribute-cleanup/
        #define NS_DESIGNATED_INITIALIZER __attribute__((objc_designated_initializer))
    #else
        #define NS_DESIGNATED_INITIALIZER
    #endif

#endif

#endif
