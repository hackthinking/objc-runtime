/*
 * Copyright (c) 1999-2007 Apple Inc.  All Rights Reserved.
 * 
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*
 *	objc.h
 *	Copyright 1988-1996, NeXT Software, Inc.
 */

#ifndef _OBJC_OBJC_H_
#define _OBJC_OBJC_H_

#include <sys/types.h>      // for __DARWIN_NULL
#include <Availability.h>
#include <objc/objc-api.h>
#include <stdbool.h>

///判断是否定义oc类型
#if !OBJC_TYPES_DEFINED

    /// An opaque type that represents an Objective-C class.
    /// 不透明类型，表示一个 OC 声明的类
    typedef struct objc_class *Class;

    /// Represents an instance of a class.
    /// 表示一个 OC 声明的实例类
    struct objc_object {
        Class isa  OBJC_ISA_AVAILABILITY;   // OC 2.0后这个属性被弃用了，详见宏定义
    };

    /// A pointer to an instance of a class.
    /// id定义，表示一个 OC 实例类的指针
    typedef struct objc_object *id;

#endif

/**
 * 关于SEL和IMP定义
 * SEL的理解: SEL又叫选择器，是表示一个方法的selector的指针,映射方法的名字。Objective-C在编译时，会依据每一个方法的名字、参数序列，生成一个唯一的整型标识(Int类型的地址)，这个标识就是SEL。 SEL的作用是作为IMP的KEY，存储在NSSet中，便于hash快速查询方法。SEL不能相同，对应方法可以不同。所以在Objective-C同一个类(及类的继承体系)中，不能存在2个同名的方法，就算参数类型不同。多个方法可以有同一个SEL。 不同的类可以有相同的方法名。不同类的实例对象执行相同的selector时，会在各自的方法列表中去根据selector去寻找自己对应的IMP。
 * IMP的理解: IMP是指向实现函数的指针，通过SEL取得IMP后，我们就获得了最终要找的实现函数的入口.
 * 参考阅读：http://heeween.github.io/blog/2015-03-03-runtimepou-xi/
 **/

/// An opaque type that represents a method selector.
/// SEL定义，不透明类型，标示一个方法的选择器
typedef struct objc_selector *SEL;

/// A pointer to the function of a method implementation.
/// IMP指针定义，指向实现函数的指针
/// 判断是否是旧版本dispatch的原型
#if !OBJC_OLD_DISPATCH_PROTOTYPES
    typedef void (*IMP)(void /* id, SEL, ... */ );
#else
    typedef id (*IMP)(id, SEL, ...);
#endif

// 定义bool类型
#define OBJC_BOOL_DEFINED

/// Type to represent a boolean value.
/// 64位判断
#if (TARGET_OS_IPHONE && __LP64__)  ||  TARGET_OS_WATCH
    #define OBJC_BOOL_IS_BOOL 1
    typedef bool BOOL;
#else
    #define OBJC_BOOL_IS_CHAR 1
    typedef signed char BOOL;
// BOOL is explicitly signed so @encode(BOOL) == "c" rather than "C" 
// even if -funsigned-char is used.
#endif

// OC中，YES和NO的定义
#if __has_feature(objc_bool)
    #define YES __objc_yes
    #define NO  __objc_no
#else
    #define YES ((BOOL)1)
    #define NO  ((BOOL)0)
#endif

// Nil和nil定义都是一样的。
#ifndef Nil
# if __has_feature(cxx_nullptr) //c++的空指针常量
#   define Nil nullptr
# else
#   define Nil __DARWIN_NULL
# endif
#endif

#ifndef nil
# if __has_feature(cxx_nullptr)
#   define nil nullptr
# else
#   define nil __DARWIN_NULL
# endif
#endif

// 如果没使用 OC的垃圾回收 和 ARC，就定义__strong为空
#if ! (defined(__OBJC_GC__)  ||  __has_feature(objc_arc))
    #define __strong /* empty */
#endif

// 如果没使用ARC，就定义 __unsafe_unretained 和 __autoreleasing 为空
#if !__has_feature(objc_arc)
    #define __unsafe_unretained /* empty */
    #define __autoreleasing /* empty */
#endif

// 关于上述修饰符定义和说明，详见clang手册
// TODO: clang ARC手册阅读
// 参考阅读：http://clang.llvm.org/docs/AutomaticReferenceCounting.html#ownership-qualification

/**
 * Returns the name of the method specified by a given selector.
 * 根据selector参数，返回方法名
 * 
 * @param sel A pointer of type \c SEL. Pass the selector whose name you wish to determine. c类型指针.
 * 
 * @return A C string indicating the name of the selector.  c类型字符串，表示selector的名称
 */
OBJC_EXPORT const char *sel_getName(SEL sel)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Registers a method with the Objective-C runtime system, maps the method 
 * name to a selector, and returns the selector value.
 *
 * 在 OC runtime system 中注册一个方法，设置名称到选择器，并且返回SEL值
 *（因为存储SEL的是一个NXMapTable结构，map可以理解为遍历表中元素，或者是添加新元素）
 *
 * @param str A pointer to a C string. Pass the name of the method you wish to register.
 * 
 * @return A pointer of type SEL specifying the selector for the named method.
 * 
 * @note You must register a method name with the Objective-C runtime system to obtain the
 *  method’s selector before you can add the method to a class definition. If the method name
 *  has already been registered, this function simply returns the selector.
 *
 * @note 方法添加到类定义之前，必须要在OC runtime system中注册，如果已经注册过了，方法会直接返回数据
 */
OBJC_EXPORT SEL sel_registerName(const char *str)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Returns the class name of a given object.
 * 根据obj参数，返回类名称
 * 
 * @param obj An Objective-C object.
 * 
 * @return The name of the class of which \e obj is an instance.    //返回类实例的名称
 */
OBJC_EXPORT const char *object_getClassName(id obj)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Returns a pointer to any extra bytes allocated with an instance given object.
 * 返回实力对象初始化时的额外字节 extraBytes 的指针。注：直接看class_createInstance函数，一看就明白extraBytes定义了。
 *
 * @param obj An Objective-C object.
 * 
 * @return A pointer to any extra bytes allocated with \e obj. If \e obj was
 *   not allocated with any extra bytes, then dereferencing the returned pointer is undefined.
 * 
 * @note This function returns a pointer to any extra bytes allocated with the instance
 *  (as specified by \c class_createInstance with extraBytes>0). This memory follows the
 *  object's ordinary ivars, but may not be adjacent to the last ivar.
 * @note The returned pointer is guaranteed to be pointer-size aligned, even if the area following
 *  the object's last ivar is less aligned than that. Alignment greater than pointer-size is never
 *  guaranteed, even if the area following the object's last ivar is more aligned than that.
 * @note In a garbage-collected environment, the memory is scanned conservatively.
 *
 * @note 这个方法返回实力对象分配的额外字节的指针（class_createInstance 参数中声明的 extraBytes>0)。
 *  此内存跟随对象的普通实例变量，但可能与最后的变量不相邻。
 * @note 返回指针被保证与指针大小（pointer-size）一致，即使最后变量小于指针大小（pointer-size）。
 *  如果最后的变量大小超过指针大小（pointer-size），不保证Alignment 比指针大小（pointer-size）大
 * @note 在垃圾回收的环境中，内存是保守扫描。（？？？）
 *
 * @note 注：这部分应该与初始化相关，OC2.0种allocWithZone的zone，应该就是extraBytes，系统默认的将这个参数置0. 
 *  可参见 _objc_rootAllocWithZone
 */
OBJC_EXPORT void *object_getIndexedIvars(id obj)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Identifies a selector as being valid or invalid.
 * 识别一个选择器为有效或无效
 * 
 * @param sel The selector you want to identify.
 * 
 * @return YES if selector is valid and has a function implementation, NO otherwise. 
 * 
 * @warning On some platforms, an invalid reference (to invalid memory addresses) can cause
 *  a crash.
 *
 * @note 在某些平台，一个无效的引用（无效的内存地址）会引起crash
 */
OBJC_EXPORT BOOL sel_isMapped(SEL sel)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Registers a method name with the Objective-C runtime system.
 * 在OC runtime system中注册一个方法名
 * 
 * @param str A pointer to a C string. Pass the name of the method you wish to register. 方法名
 * 
 * @return A pointer of type SEL specifying the selector for the named method.
 * 
 * @note The implementation of this method is identical to the implementation of \c sel_registerName.
 * @note Prior to OS X version 10.0, this method tried to find the selector mapped to the given name
 *  and returned \c NULL if the selector was not found. This was changed for safety, because it was
 *  observed that many of the callers of this function did not check the return value for \c NULL.
 *
 * @note 方法实现与sel_registerName相同
 * @note 为了安全起见，OSX 10.0之前，这个方法尝试寻找选择器，如果没有，返回NULL。因为很多对此方法的调用没有检查返回的NULL
 */
OBJC_EXPORT SEL sel_getUid(const char *str)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);


// Obsolete ARC conversions. Deprecation forthcoming.
// Use CFBridgingRetain, CFBridgingRelease, and __bridge casts instead.

// 过时ARC转换。即将弃用。
// 使用CFBridgingRetain, CFBridgingRelease, __bridge映射 替代
typedef const void* objc_objectptr_t;

// ARC定义
#if __has_feature(objc_arc)
#   define objc_retainedObject(o) ((__bridge_transfer id)(objc_objectptr_t)(o))
#   define objc_unretainedObject(o) ((__bridge id)(objc_objectptr_t)(o))
#   define objc_unretainedPointer(o) ((__bridge objc_objectptr_t)(id)(o))
#else
#   define objc_retainedObject(o) ((id)(objc_objectptr_t)(o))
#   define objc_unretainedObject(o) ((id)(objc_objectptr_t)(o))
#   define objc_unretainedPointer(o) ((objc_objectptr_t)(id)(o))
#endif


#if !__OBJC2__

// The following declarations are provided here for source compatibility.
// 兼容代码

//64位判断
#if defined(__LP64__)
    typedef long arith_t;
    typedef unsigned long uarith_t;
#   define ARITH_SHIFT 32
#else
    typedef int arith_t;
    typedef unsigned uarith_t;
#   define ARITH_SHIFT 16
#endif

typedef char *STR;

#define ISSELECTOR(sel) sel_isMapped(sel)
#define SELNAME(sel)	sel_getName(sel)
#define SELUID(str)	sel_getUid(str)
#define NAMEOF(obj)     object_getClassName(obj)
#define IV(obj)         object_getIndexedIvars(obj)

#endif

#endif  /* _OBJC_OBJC_H_ */
