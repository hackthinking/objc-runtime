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

#ifndef _OBJC_RUNTIME_H
#define _OBJC_RUNTIME_H

#include <objc/objc.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <Availability.h>
#include <TargetConditionals.h>

#if TARGET_OS_MAC
#include <sys/types.h>
#endif


#pragma mark - 类型
/* Types */

#if !OBJC_TYPES_DEFINED

/// An opaque type that represents a method in a class definition.
/// 不透明类型，表示类定义中的一个方法
typedef struct objc_method *Method;

/// An opaque type that represents an instance variable.
/// 不透明类型，表示一个实例变量
typedef struct objc_ivar *Ivar;

/// An opaque type that represents a category.
/// 不透明类型，表示一个分类
typedef struct objc_category *Category;

/// An opaque type that represents an Objective-C declared property.
/// 不透明类型，表示一个 OC 声明的属性
typedef struct objc_property *objc_property_t;

// class 结构定义
struct objc_class {
    Class isa  OBJC_ISA_AVAILABILITY;

    //非OC2.0下
    #if !__OBJC2__
        Class super_class                                        OBJC2_UNAVAILABLE; // 父类
        const char *name                                         OBJC2_UNAVAILABLE; // 名称
        long version                                             OBJC2_UNAVAILABLE; // 版本
        long info                                                OBJC2_UNAVAILABLE; // 信息
        long instance_size                                       OBJC2_UNAVAILABLE; // 实例大小
        struct objc_ivar_list *ivars                             OBJC2_UNAVAILABLE; // 对象变量列表
        struct objc_method_list **methodLists                    OBJC2_UNAVAILABLE; // 对象方法列表
        struct objc_cache *cache                                 OBJC2_UNAVAILABLE; // 对象缓存
        struct objc_protocol_list *protocols                     OBJC2_UNAVAILABLE; // 对象协议列表
    #endif

} OBJC2_UNAVAILABLE;
/* Use `Class` instead of `struct objc_class *` */
/* 使用 `Class` 替代 `struct objc_class *` */

#endif

//定义protocol
#ifdef __OBJC__
    @class Protocol;
#else
    typedef struct objc_object Protocol;
#endif

/// Defines a method
/// 定义一个方法
struct objc_method_description {
    SEL name;               /**< The name of the method             方法名 */
    char *types;            /**< The types of the method arguments  方法参数类型 */
};

/// Defines a property attribute
/// 定义一个 property 的属性
typedef struct {
    const char *name;           /**< The name of the attribute                  属性名称 */
    const char *value;          /**< The value of the attribute (usually empty) 属性值(通常为空) */
} objc_property_attribute_t;


#pragma mark - 函数
/* Functions */

#pragma mark - 操作实例的函数
/* Working with Instances */

/** 
 * Returns a copy of a given object.
 * 返回给定对象的副本
 *
 * @param obj An Objective-C object.        一个 OC 对象
 * @param size The size of the object \e obj. 对象的大小
 *
 * @return A copy of \e obj.    对象的副本
 */
OBJC_EXPORT id object_copy(id obj, size_t size)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0)
    OBJC_ARC_UNAVAILABLE;

/** 
 * Frees the memory occupied by a given object.
 * 释放给定对象的内存
 * @note 本函数只会释放对象的内存空间，释放后对象的指针地址不会变化
 *
 * @param obj An Objective-C object.    一个 OC 对象
 *
 * @return nil
 */
OBJC_EXPORT id object_dispose(id obj)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0)
    OBJC_ARC_UNAVAILABLE;

/** 
 * Returns the class of an object.
 * 返回对象的类
 *
 * @param obj The object you want to inspect. 要检测的对象
 *
 * @return The class object of which \e object is an instance,
 *  or \c Nil if \e object is \c nil. 
 * object 实例的 class 对象，如果 \e object 是 \c nil 则返回 \c Nil
 */
OBJC_EXPORT Class object_getClass(id obj) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/**
 * Sets the class of an object.
 * 设置对象的类
 *
 * @param obj The object to modify.     要修改的类
 * @param cls A class object.           一个 class 对象
 *
 * @return The previous value of \e object's class, or \c Nil if \e object is \c nil.
 * object 修改前的 class，如果 \e object 是 \c nil 则返回 \c Nil
 */
OBJC_EXPORT Class object_setClass(id obj, Class cls) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);


/**
 * Returns whether an object is a class object.
 * 判断对象是否是一个 class 对象
 *
 * @param obj An Objective-C object.
 *
 * @return true if the object is a class or metaclass, false otherwise.
 * 如果对象是一个 class 或者 mataclass 返回 true，否则返回 false
 */
OBJC_EXPORT BOOL object_isClass(id obj)
    __OSX_AVAILABLE_STARTING(__MAC_10_10, __IPHONE_8_0);


/** 
 * Returns the class name of a given object.
 * 返回给定对象的类名
 *
 * @param obj An Objective-C object.
 *
 * @return The name of the class of which \e obj is an instance.
 * obj 实例对象的类名
 */
OBJC_EXPORT const char *object_getClassName(id obj)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Returns a pointer to any extra bytes allocated with an instance given object.
 * 返回指向给定对象的extra bytes的指针
 * 
 * @param obj An Objective-C object.
 * 
 * @return A pointer to any extra bytes allocated with \e obj. If \e obj was
 *   not allocated with any extra bytes, then dereferencing the returned pointer is undefined.
 * 返回指向给定对象的extra bytes的指针, 如果没有使用初始化extra bytes，则返回指针为undefined
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
 *
 */
OBJC_EXPORT void *object_getIndexedIvars(id obj)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0)
    OBJC_ARC_UNAVAILABLE;

/** 
 * Reads the value of an instance variable in an object.
 * 读取实例变量值
 *
 * @param obj The object containing the instance variable whose value you want to read.     实例
 * @param ivar The Ivar describing the instance variable whose value you want to read.      变量
 * 
 * @return The value of the instance variable specified by \e ivar, or \c nil if \e object is \c nil.   变量值，空返回nil
 * 
 * @note \c object_getIvar is faster than \c object_getInstanceVariable if the Ivar
 *  for the instance variable is already known.
 * @note 如果实例变量已确定存在，\c object_getIvar 比 \c object_getInstanceVariable 快。（后者比前者多了个查验变量的步骤）
 *
 */
OBJC_EXPORT id object_getIvar(id obj, Ivar ivar) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Sets the value of an instance variable in an object.
 * 设置实例变量值
 * 
 * @param obj The object containing the instance variable whose value you want to set.  实例
 * @param ivar The Ivar describing the instance variable whose value you want to set.   变量
 * @param value The new value for the instance variable.                                变量值
 * 
 * @note \c object_setIvar is faster than \c object_setInstanceVariable if the Ivar
 *  for the instance variable is already known.
 * @note 如果实例变量已确定存在，\c object_setIvar 比 \c object_setInstanceVariable 快。（后者比前者多了个查验变量的步骤）
 */
OBJC_EXPORT void object_setIvar(id obj, Ivar ivar, id value) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Changes the value of an instance variable of a class instance.
 * 设置实例变量值
 * 
 * @param obj A pointer to an instance of a class. Pass the object containing
 *  the instance variable whose value you wish to modify.                                           实例指针
 * @param name A C string. Pass the name of the instance variable whose value you wish to modify.   C String 变量名
 * @param value The new value for the instance variable.                                            变量值
 * 
 * @return A pointer to the \c Ivar data structure that defines the type and 
 *  name of the instance variable specified by \e name.
 * 返回 \c Ivar 数据结构的变量指针。
 */
OBJC_EXPORT Ivar object_setInstanceVariable(id obj, const char *name, void *value)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0)
    OBJC_ARC_UNAVAILABLE;

/** 
 * Obtains the value of an instance variable of a class instance.
 * 获取实例变量值
 *
 * @param obj A pointer to an instance of a class. Pass the object containing
 *  the instance variable whose value you wish to obtain.                                           实例指针
 * @param name A C string. Pass the name of the instance variable whose value you wish to obtain.   C String 变量名
 * @param outValue On return, contains a pointer to the value of the instance variable.             返回值指针
 * 
 * @return A pointer to the \c Ivar data structure that defines the type and name of
 *  the instance variable specified by \e name.
 * 返回 \c Ivar 数据结构的变量指针。
 */
OBJC_EXPORT Ivar object_getInstanceVariable(id obj, const char *name, void **outValue)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0)
    OBJC_ARC_UNAVAILABLE;


#pragma mark - 获取类定义函数
/* Obtaining Class Definitions */

/**
 * Returns the class definition of a specified class.
 * 返回指定类的类定义
 *
 * @param name The name of the class to look up.    要查询的类名
 *
 * @return The Class object for the named class, or \c nil
 *  if the class is not registered with the Objective-C runtime.
 * 指定名称的 Class 对象，如果该类没有被 OC 运行时注册，返回 \c nil
 *
 * @note \c objc_getClass is different from \c objc_lookUpClass in that if the class
 *  is not registered, \c objc_getClass calls the class handler callback and then checks
 *  a second time to see whether the class is registered. \c objc_lookUpClass does
 *  not call the class handler callback.
 *
 * @warning Earlier implementations of this function (prior to OS X v10.0)
 *  terminate the program if the class does not exist.
 *
 * @note  \c objc_getClass 与 \c objc_lookUpClass 函数的区别在于，类如果没有注册。
 *  \c objc_getClass 函数会调用 class 的句柄回调方法，然后再次检查该类是否已经注册
 *  \c objc_lookUpClass 函数不调用 class 的句柄回调方法
 *
 * @note 注：代码实现里，objc_getClass，objc_lookUpClass 最后的调用look_up_class参数根本就没用到。没区别！
 *
 * @warning 在 OS X 10.0以前版本执行此函数，如果类不存在会终止程序
 */
OBJC_EXPORT Class objc_getClass(const char *name)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Returns the metaclass definition of a specified class.
 * 获取类定义中的metaclass
 * 
 * @param name The name of the class to look up.    要查询的类名
 * 
 * @return The \c Class object for the metaclass of the named class, or \c nil if the class
 *  is not registered with the Objective-C runtime.
 *  返回对应 \c Class 对象的 metaclass. 如果类没在runtime中注册，返回nil
 *
 * @note If the definition for the named class is not registered, this function calls the class handler
 *  callback and then checks a second time to see if the class is registered. However, every class
 *  definition must have a valid metaclass definition, and so the metaclass definition is always returned,
 *  whether it’s valid or not.
 * @note 如果命名的类没有注册，会先回调类的句柄，然后进行二次注册检查。
 *  定义每个类，都必须有一个合法的metaclass定义，所以无论metaclass是否合法，定义总会有值返回。
 * （原文这个地方有问题，类中的metaclass必须合法，metaclass定义不一定合法 ？？？）
 *
 */
OBJC_EXPORT Class objc_getMetaClass(const char *name)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Returns the class definition of a specified class.
 * 返回指定类的类定义
 *
 * @param name The name of the class to look up.    要查询的类名
 * 
 * @return The Class object for the named class, or \c nil if the class
 *  is not registered with the Objective-C runtime.
 * 指定名称的 Class 对象，如果该类没有被 OC 运行时注册，返回 \c nil
 *
 * @note \c objc_getClass is different from this function in that if the class is not
 *  registered, \c objc_getClass calls the class handler callback and then checks a second
 *  time to see whether the class is registered. This function does not call the class handler callback.
 *
 *  \c objc_getClass 与 \c objc_lookUpClass 函数的区别在于，类如果没有注册。
 *  \c objc_getClass 函数会调用 class 的句柄回调方法，然后再次检查该类是否已经注册
 *  \c objc_lookUpClass 函数不调用 class 的句柄回调方法
 *
 */
OBJC_EXPORT Class objc_lookUpClass(const char *name)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Returns the class definition of a specified class.
 * 返回指定类的类定义
 *
 * @param name The name of the class to look up.    要查询的类名
 * 
 * @return The Class object for the named class.    类对象
 * 
 * @note This function is the same as \c objc_getClass, but kills the process if the class is not found.
 * @note This function is used by ZeroLink, where failing to find a class would be a compile-time link error without ZeroLink.
 *
 * @note 此方法与 \c objc_getClass 一样，但是如果类不存在，会杀死进程
 * @note ZeroLink使用此方法，不使ZeroLink时，查找类失败，会引起编译时链接错误(compile-time link error)
 */
OBJC_EXPORT Class objc_getRequiredClass(const char *name)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Obtains the list of registered class definitions.
 * 获取已注册的类定义列表
 * 
 * @param buffer An array of \c Class values. On output, each \c Class value points to
 *  one class definition, up to either \e bufferCount or the total number of registered classes,
 *  whichever is less. You can pass \c NULL to obtain the total number of registered class
 *  definitions without actually retrieving any class definitions.
 *  值为 \c Class 的数组。结果中，每个 \c Class 指针都指向类定义，数量取决于 \e bufferCount 或 注册的类总数 两者中少的一个。 
 *  参数可以传 \c NULL 来获取总数和所有已注册的类定义列表
 *
 * @param bufferCount An integer value. Pass the number of pointers for which you have allocated space
 *  in \e buffer. On return, this function fills in only this number of elements. If this number is less
 *  than the number of registered classes, this function returns an arbitrary subset of the registered classes.
 *  整数， 设置初始化的 \e buffer 指针数量。 方法返回指定数量的元素，如果 \c bufferCount 小于总注册数，方法讲返回任意分组的注册类
 *
 * @return An integer value indicating the total number of registered classes.  已注册的类总数
 * 
 * @note The Objective-C runtime library automatically registers all the classes defined in your source code.
 *  You can create class definitions at runtime and register them with the \c objc_addClass function.
 * @note Objective-C runtime library 自动注册所有代码中定义的类。你可以在运行时使用 \c objc_addClass 进行添加创建。
 *
 * @warning You cannot assume that class objects you get from this function are classes that inherit from \c NSObject,
 *  so you cannot safely call any methods on such classes without detecting that the method is implemented first.
 * @warning 所有从这个方法中获得的类对象，你不能假定是继承自 \c NSObject, 在没有确定前，不要直接调用。
 *
 */
OBJC_EXPORT int objc_getClassList(Class *buffer, int bufferCount)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Creates and returns a list of pointers to all registered class definitions.
 * 获取已注册的类定义指针列表
 *
 * @param outCount An integer pointer used to store the number of classes returned by
 *  this function in the list. It can be \c nil.    返回列表中指针数量，不能为nil
 * 
 * @return A nil terminated array of classes. It must be freed with \c free()   . nil元素结束的数组（最后一个元素是nil），必须用 \c free() 释放
 * 
 * @see objc_getClassList
 */
OBJC_EXPORT Class *objc_copyClassList(unsigned int *outCount)
     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_3_1);


#pragma mark - 操作类的函数
/* Working with Classes */

/** 
 * Returns the name of a class. 返回类名
 * 
 * @param cls A class object.   类对象
 * 
 * @return The name of the class, or the empty string if \e cls is \c Nil.  类名称，参数为Nil时，返回空字符串
 */
OBJC_EXPORT const char *class_getName(Class cls) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns a Boolean value that indicates whether a class object is a metaclass.    判断类是否是metaclass
 * 
 * @param cls A class object.   类对象
 * 
 * @return \c YES if \e cls is a metaclass, \c NO if \e cls is a non-meta class,    是，返回YES，反之NO，空对象返回NO
 *  \c NO if \e cls is \c Nil.
 */
OBJC_EXPORT BOOL class_isMetaClass(Class cls) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns the superclass of a class.   返回父类
 *
 * @param cls A class object.           类对象
 * 
 * @return The superclass of the class, or \c Nil if        返回父类，如果参数为根类（root class）或 Nil，返回 Nil
 *  \e cls is a root class, or \c Nil if \e cls is \c Nil.
 *
 * @note You should usually use \c NSObject's \c superclass method instead of this function.
 * @note 尽量使用 \c NSObject's \c superclass 方法
 */
OBJC_EXPORT Class class_getSuperclass(Class cls) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Sets the superclass of a given class.    设置新的父类
 * 
 * @param cls The class whose superclass you want to set.   类对象
 * @param newSuper The new superclass for cls.              新的父类
 * 
 * @return The old superclass for cls.      返回旧的父类
 * 
 * @warning You should not use this function.
 * @warning 你不应该使用这个方法 (请看使用版本的宏定义)
 */
OBJC_EXPORT Class class_setSuperclass(Class cls, Class newSuper) 
     __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_5,__MAC_10_5, __IPHONE_2_0,__IPHONE_2_0);

/** 
 * Returns the version number of a class definition.    返回类定义的版本号
 * 
 * @param cls A pointer to a \c Class data structure. Pass                      类对象指针
 *  the class definition for which you wish to obtain the version.
 * 
 * @return An integer indicating the version number of the class definition.    版本号，整数
 *
 * @see class_setVersion
 */
OBJC_EXPORT int class_getVersion(Class cls)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Sets the version number of a class definition.       设置类定义的的版本号
 * 
 * @param cls A pointer to an Class data structure.                                 类对象指针
 *  Pass the class definition for which you wish to set the version.
 * @param version An integer. Pass the new version number of the class definition.  版本号，整数
 *
 * @note You can use the version number of the class definition to provide versioning of the
 *  interface that your class represents to other classes. This is especially useful for object
 *  serialization (that is, archiving of the object in a flattened form), where it is important to
 *  recognize changes to the layout of the instance variables in different class-definition versions.
 * @note Classes derived from the Foundation framework \c NSObject class can set the class-definition
 *  version number using the \c setVersion: class method, which is implemented using the \c class_setVersion function.
 *
 * @note 设置类的版本号，在序列化对象的时候有用，可以用来识别。
 * @note Classes 源于 Foundation framework \c ，NSObject 可以用 \c setVersion 设置类定义的版本号，底层调用 \c class_setVersion 实现
 */
OBJC_EXPORT void class_setVersion(Class cls, int version)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Returns the size of instances of a class.    返回类实例大小
 *
 * @param cls A class object.                   类对象
 * 
 * @return The size in bytes of instances of the class \e cls, or \c 0 if \e cls is \c Nil. 返回类实例的bytes大小，对象为空返回0
 */
OBJC_EXPORT size_t class_getInstanceSize(Class cls) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns the \c Ivar for a specified instance variable of a given class.      返回类中指定名称的变量
 * 
 * @param cls The class whose instance variable you wish to obtain.
 * @param name The name of the instance variable definition to obtain.
 * 
 * @return A pointer to an \c Ivar data structure containing information about 
 *  the instance variable specified by \e name.
 */
OBJC_EXPORT Ivar class_getInstanceVariable(Class cls, const char *name)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Returns the Ivar for a specified class variable of a given class.            返回类中指定名称的变量
 * 
 * @param cls The class definition whose class variable you wish to obtain.
 * @param name The name of the class variable definition to obtain.
 * 
 * @return A pointer to an \c Ivar data structure containing information about 
 * the class variable specified by \e name.
 */
OBJC_EXPORT Ivar class_getClassVariable(Class cls, const char *name) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Describes the instance variables declared by a class.                        类中声明的实例变量
 * 
 * @param cls The class to inspect.
 * @param outCount On return, contains the length of the returned array. 
 *  If outCount is NULL, the length is not returned.
 * 
 * @return An array of pointers of type Ivar describing the instance variables declared by the class. 
 *  Any instance variables declared by superclasses are not included. The array contains *outCount 
 *  pointers followed by a NULL terminator. You must free the array with free().
 * 
 *  If the class declares no instance variables, or cls is Nil, NULL is returned and *outCount is 0.
 */
OBJC_EXPORT Ivar *class_copyIvarList(Class cls, unsigned int *outCount) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns a specified instance method for a given class.                       返回类中的方法
 * 
 * @param cls The class you want to inspect.
 * @param name The selector of the method you want to retrieve.
 * 
 * @return The method that corresponds to the implementation of the selector specified by 
 *  \e name for the class specified by \e cls, or \c NULL if the specified class or its 
 *  superclasses do not contain an instance method with the specified selector.
 *
 * @note This function searches superclasses for implementations, whereas \c class_copyMethodList does not.
 */
OBJC_EXPORT Method class_getInstanceMethod(Class cls, SEL name)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Returns a pointer to the data structure describing a given class method for a given class.   返回类中的方法
 * 
 * @param cls A pointer to a class definition. Pass the class that contains the method you want to retrieve.
 * @param name A pointer of type \c SEL. Pass the selector of the method you want to retrieve.
 * 
 * @return A pointer to the \c Method data structure that corresponds to the implementation of the 
 *  selector specified by aSelector for the class specified by aClass, or NULL if the specified 
 *  class or its superclasses do not contain an instance method with the specified selector.
 *
 * @note Note that this function searches superclasses for implementations, 
 *  whereas \c class_copyMethodList does not.
 */
OBJC_EXPORT Method class_getClassMethod(Class cls, SEL name)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Returns the function pointer that would be called if a               返回类中，方法选择器的IMP指针
 * particular message were sent to an instance of a class.
 * 
 * @param cls The class you want to inspect.
 * @param name A selector.
 * 
 * @return The function pointer that would be called if \c [object name] were called
 *  with an instance of the class, or \c NULL if \e cls is \c Nil.
 *
 * @note \c class_getMethodImplementation may be faster than \c method_getImplementation(class_getInstanceMethod(cls, name)).
 * @note The function pointer returned may be a function internal to the runtime instead of
 *  an actual method implementation. For example, if instances of the class do not respond to
 *  the selector, the function pointer returned will be part of the runtime's message forwarding machinery.
 */
OBJC_EXPORT IMP class_getMethodImplementation(Class cls, SEL name) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns the function pointer that would be called if a particular    返回类中，方法选择器的IMP指针
 * message were sent to an instance of a class.
 * 
 * @param cls The class you want to inspect.
 * @param name A selector.
 * 
 * @return The function pointer that would be called if \c [object name] were called
 *  with an instance of the class, or \c NULL if \e cls is \c Nil.
 */
OBJC_EXPORT IMP class_getMethodImplementation_stret(Class cls, SEL name) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0)
     OBJC_ARM64_UNAVAILABLE;

/** 
 * Returns a Boolean value that indicates whether instances of a class respond to a particular selector.    判断类是否响应指定的方法选择器。
 * 
 * @param cls The class you want to inspect.
 * @param sel A selector.
 * 
 * @return \c YES if instances of the class respond to the selector, otherwise \c NO.
 * 
 * @note You should usually use \c NSObject's \c respondsToSelector: or \c instancesRespondToSelector: 
 *  methods instead of this function.
 */
OBJC_EXPORT BOOL class_respondsToSelector(Class cls, SEL sel) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Describes the instance methods implemented by a class.   描述类方法
 * 
 * @param cls The class you want to inspect.
 * @param outCount On return, contains the length of the returned array. 
 *  If outCount is NULL, the length is not returned.
 * 
 * @return An array of pointers of type Method describing the instance methods 
 *  implemented by the class—any instance methods implemented by superclasses are not included. 
 *  The array contains *outCount pointers followed by a NULL terminator. You must free the array with free().
 * 
 *  If cls implements no instance methods, or cls is Nil, returns NULL and *outCount is 0.
 * 
 * @note To get the class methods of a class, use \c class_copyMethodList(object_getClass(cls), &count).
 * @note To get the implementations of methods that may be implemented by superclasses, 
 *  use \c class_getInstanceMethod or \c class_getClassMethod.
 */
OBJC_EXPORT Method *class_copyMethodList(Class cls, unsigned int *outCount) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns a Boolean value that indicates whether a class conforms to a given protocol.     判断一个类是否遵循某个协议
 * 
 * @param cls The class you want to inspect.
 * @param protocol A protocol.
 *
 * @return YES if cls conforms to protocol, otherwise NO.
 *
 * @note You should usually use NSObject's conformsToProtocol: method instead of this function.
 */
OBJC_EXPORT BOOL class_conformsToProtocol(Class cls, Protocol *protocol) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Describes the protocols adopted by a class.  描述类遵循的协议
 * 
 * @param cls The class you want to inspect.
 * @param outCount On return, contains the length of the returned array. 
 *  If outCount is NULL, the length is not returned.
 * 
 * @return An array of pointers of type Protocol* describing the protocols adopted 
 *  by the class. Any protocols adopted by superclasses or other protocols are not included. 
 *  The array contains *outCount pointers followed by a NULL terminator. You must free the array with free().
 * 
 *  If cls adopts no protocols, or cls is Nil, returns NULL and *outCount is 0.
 */
OBJC_EXPORT Protocol * __unsafe_unretained *class_copyProtocolList(Class cls, unsigned int *outCount)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns a property with a given name of a given class.   返回类中指定名称的属性
 * 
 * @param cls The class you want to inspect.
 * @param name The name of the property you want to inspect.
 * 
 * @return A pointer of type \c objc_property_t describing the property, or
 *  \c NULL if the class does not declare a property with that name, 
 *  or \c NULL if \e cls is \c Nil.
 */
OBJC_EXPORT objc_property_t class_getProperty(Class cls, const char *name)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Describes the properties declared by a class.    返回类中的多个属性
 * 
 * @param cls The class you want to inspect.
 * @param outCount On return, contains the length of the returned array. 
 *  If \e outCount is \c NULL, the length is not returned.        
 * 
 * @return An array of pointers of type \c objc_property_t describing the properties 
 *  declared by the class. Any properties declared by superclasses are not included. 
 *  The array contains \c *outCount pointers followed by a \c NULL terminator. You must free the array with \c free().
 * 
 *  If \e cls declares no properties, or \e cls is \c Nil, returns \c NULL and \c *outCount is \c 0.
 */
OBJC_EXPORT objc_property_t *class_copyPropertyList(Class cls, unsigned int *outCount)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns a description of the \c Ivar layout for a given class.   返回类中的ivarLayout（强引用），相关阅读：http://blog.sunnyxx.com/2015/09/13/class-ivar-layout/
 * 
 * @param cls The class to inspect.
 * 
 * @return A description of the \c Ivar layout for \e cls.
 */
OBJC_EXPORT const uint8_t *class_getIvarLayout(Class cls)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns a description of the layout of weak Ivars for a given class. 返回类中的weakIvarLayout，相关阅读：http://blog.sunnyxx.com/2015/09/13/class-ivar-layout/
 * 
 * @param cls The class to inspect.
 * 
 * @return A description of the layout of the weak \c Ivars for \e cls.
 */
OBJC_EXPORT const uint8_t *class_getWeakIvarLayout(Class cls)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Adds a new method to a class with a given name and implementation.   添加方法选择器到类，绑定名称和IMP
 * 
 * @param cls The class to which to add a method.
 * @param name A selector that specifies the name of the method being added.
 * @param imp A function which is the implementation of the new method. The function must take at least two arguments—self and _cmd.
 * @param types An array of characters that describe the types of the arguments to the method. 
 * 
 * @return YES if the method was added successfully, otherwise NO 
 *  (for example, the class already contains a method implementation with that name).
 *
 * @note class_addMethod will add an override of a superclass's implementation, 
 *  but will not replace an existing implementation in this class. 
 *  To change an existing implementation, use method_setImplementation.
 */
OBJC_EXPORT BOOL class_addMethod(Class cls, SEL name, IMP imp, 
                                 const char *types) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Replaces the implementation of a method for a given class.           替换类中方法选择器IMP
 * 
 * @param cls The class you want to modify.
 * @param name A selector that identifies the method whose implementation you want to replace.
 * @param imp The new implementation for the method identified by name for the class identified by cls.
 * @param types An array of characters that describe the types of the arguments to the method. 
 *  Since the function must take at least two arguments—self and _cmd, the second and third characters
 *  must be “@:” (the first character is the return type).
 * 
 * @return The previous implementation of the method identified by \e name for the class identified by \e cls.
 * 
 * @note This function behaves in two different ways:
 *  - If the method identified by \e name does not yet exist, it is added as if \c class_addMethod were called. 
 *    The type encoding specified by \e types is used as given.
 *  - If the method identified by \e name does exist, its \c IMP is replaced as if \c method_setImplementation were called.
 *    The type encoding specified by \e types is ignored.
 */
OBJC_EXPORT IMP class_replaceMethod(Class cls, SEL name, IMP imp, 
                                    const char *types) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Adds a new instance variable to a class. 添加一个新的实例变量到类
 * 
 * @return YES if the instance variable was added successfully, otherwise NO 
 *         (for example, the class already contains an instance variable with that name).
 *
 * @note This function may only be called after objc_allocateClassPair and before objc_registerClassPair. 
 *       Adding an instance variable to an existing class is not supported.
 * @note The class must not be a metaclass. Adding an instance variable to a metaclass is not supported.
 * @note The instance variable's minimum alignment in bytes is 1<<align. The minimum alignment of an instance 
 *       variable depends on the ivar's type and the machine architecture. 
 *       For variables of any pointer type, pass log2(sizeof(pointer_type)).
 */
OBJC_EXPORT BOOL class_addIvar(Class cls, const char *name, size_t size, 
                               uint8_t alignment, const char *types) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Adds a protocol to a class.  添加一个协议到类
 * 
 * @param cls The class to modify.
 * @param protocol The protocol to add to \e cls.
 * 
 * @return \c YES if the method was added successfully, otherwise \c NO 
 *  (for example, the class already conforms to that protocol).
 */
OBJC_EXPORT BOOL class_addProtocol(Class cls, Protocol *protocol) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Adds a property to a class.  添加一个属性到类
 * 
 * @param cls The class to modify.
 * @param name The name of the property.
 * @param attributes An array of property attributes.
 * @param attributeCount The number of attributes in \e attributes.
 * 
 * @return \c YES if the property was added successfully, otherwise \c NO
 *  (for example, the class already has that property).
 */
OBJC_EXPORT BOOL class_addProperty(Class cls, const char *name, const objc_property_attribute_t *attributes, unsigned int attributeCount)
     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

/** 
 * Replace a property of a class.   替换类中的属性
 * 
 * @param cls The class to modify.
 * @param name The name of the property.
 * @param attributes An array of property attributes.
 * @param attributeCount The number of attributes in \e attributes. 
 */
OBJC_EXPORT void class_replaceProperty(Class cls, const char *name, const objc_property_attribute_t *attributes, unsigned int attributeCount)
     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

/** 
 * Sets the Ivar layout for a given class.  设置类中的Ivar layout（变量强引用），相关阅读：http://blog.sunnyxx.com/2015/09/13/class-ivar-layout/
 * 
 * @param cls The class to modify.
 * @param layout The layout of the \c Ivars for \e cls.
 */
OBJC_EXPORT void class_setIvarLayout(Class cls, const uint8_t *layout)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Sets the layout for weak Ivars for a given class.    设置类中的 weak Ivar layout（变量弱引用），相关阅读：http://blog.sunnyxx.com/2015/09/13/class-ivar-layout/
 * 
 * @param cls The class to modify.
 * @param layout The layout of the weak Ivars for \e cls.
 */
OBJC_EXPORT void class_setWeakIvarLayout(Class cls, const uint8_t *layout)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Used by CoreFoundation's toll-free bridging.  CoreFoundation's 桥接使用，根据类名返回类实例id
 * Return the id of the named class.
 * 
 * @return The id of the named class, or an uninitialized class
 *  structure that will be used for the class when and if it does 
 *  get loaded.
 * 
 * @warning Do not call this function yourself.
 */
OBJC_EXPORT Class objc_getFutureClass(const char *name) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0)
     OBJC_ARC_UNAVAILABLE;


#pragma mark - 类的实例化
/* Instantiating Classes */

/** 
 * Creates an instance of a class, allocating memory for the class in the 
 * default malloc memory zone.
 * 创建一个类实例，在默认的内存区域（memory zone）中初始化
 *
 * @param cls The class that you wish to allocate an instance of.                   要初始化的类
 * @param extraBytes An integer indicating the number of extra bytes to allocate.   额外的bytes空间。额外的空间用来存储额外的类定义变量。
 *  The additional bytes can be used to store additional instance variables beyond 
 *  those defined in the class definition.
 * 
 * @return An instance of the class \e cls.
 */
OBJC_EXPORT id class_createInstance(Class cls, size_t extraBytes)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0)
    OBJC_ARC_UNAVAILABLE;

/** 
 * Creates an instance of a class at the specific location provided.
 * 在提供的特定位置创建一个类实例 （内存全部填充0， well-aligned zero-filled memory）
 *
 * @param cls The class that you wish to allocate an instance of.                   要初始化的类
 * @param bytes The location at which to allocate an instance of \e cls.            实例大小，至少 \c class_getInstanceSize(cls) bytes of well-aligned, zero-filled memory.
 *  Must point to at least \c class_getInstanceSize(cls) bytes of well-aligned,
 *  zero-filled memory.
 *
 * @return \e bytes on success, \c nil otherwise. (For example, \e cls or \e bytes  返回初始化的内存对象，或空
 *  might be \c nil)
 *
 * @see class_createInstance
 */
OBJC_EXPORT id objc_constructInstance(Class cls, void *bytes) 
    __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_3_0)
    OBJC_ARC_UNAVAILABLE;

/** 
 * Destroys an instance of a class without freeing memory and removes any           销毁一个实例，不会释放内存和移除实例引用
 * associated references this instance might have had.
 * 
 * @param obj The class instance to destroy.        要销毁的实例
 * 
 * @return \e obj. Does nothing if \e obj is nil.
 * 
 * @warning GC does not call this. If you edit this, also edit finalize.
 * @warning 垃圾回收不回调用，如果修改，同样需要修改finalize
 *
 * @note CF and other clients do call this under GC.
 */
OBJC_EXPORT void *objc_destructInstance(id obj) 
    __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_3_0)
    OBJC_ARC_UNAVAILABLE;


#pragma mark - 添加类
/* Adding Classes */

/** 
 * Creates a new class and metaclass.   添加一个新的类和metaclass
 *
 * @param superclass The class to use as the new class's superclass, or \c Nil to create a new root class.
 * @param name The string to use as the new class's name. The string will be copied.
 * @param extraBytes The number of bytes to allocate for indexed ivars at the end of 
 *  the class and metaclass objects. This should usually be \c 0.
 * 
 * @return The new class, or Nil if the class could not be created (for example, the desired name is already in use).
 * 
 * @note You can get a pointer to the new metaclass by calling \c object_getClass(newClass).
 * @note To create a new class, start by calling \c objc_allocateClassPair. 
 *  Then set the class's attributes with functions like \c class_addMethod and \c class_addIvar.
 *  When you are done building the class, call \c objc_registerClassPair. The new class is now ready for use.
 * @note Instance methods and instance variables should be added to the class itself. 
 *  Class methods should be added to the metaclass.
 */
OBJC_EXPORT Class objc_allocateClassPair(Class superclass, const char *name, 
                                         size_t extraBytes) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Registers a class that was allocated using \c objc_allocateClassPair.    注册一个用\c objc_allocateClassPair 初始化的类
 * 
 * @param cls The class you want to register.
 */
OBJC_EXPORT void objc_registerClassPair(Class cls) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Used by Foundation's Key-Value Observing.        使用于Foundation 的key-value监听
 * 
 * @warning Do not call this function yourself.
 */
OBJC_EXPORT Class objc_duplicateClass(Class original, const char *name, size_t extraBytes)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Destroy a class and its associated metaclass.    销毁一个类对象和相关metaclass
 * 
 * @param cls The class to be destroyed. It must have been allocated with 
 *  \c objc_allocateClassPair
 * 
 * @warning Do not call if instances of this class or a subclass exist.
 */
OBJC_EXPORT void objc_disposeClassPair(Class cls) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);


#pragma mark - 操作方法的函数
/* Working with Methods */

/** 
 * Returns the name of a method.                        返回方法名称
 * 
 * @param m The method to inspect.
 * 
 * @return A pointer of type SEL.
 * 
 * @note To get the method name as a C string, call \c sel_getName(method_getName(method)).
 */
OBJC_EXPORT SEL method_getName(Method m) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns the implementation of a method.              返回方法的IMP指针
 * 
 * @param m The method to inspect.
 * 
 * @return A function pointer of type IMP.
 */
OBJC_EXPORT IMP method_getImplementation(Method m) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns a string describing a method's parameter and return types.   返回描述方法的参数和返回类型，字符串
 * 
 * @param m The method to inspect.
 * 
 * @return A C string. The string may be \c NULL.
 */
OBJC_EXPORT const char *method_getTypeEncoding(Method m) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns the number of arguments accepted by a method.                返回方法的参数数量
 * 
 * @param m A pointer to a \c Method data structure. Pass the method in question.
 * 
 * @return An integer containing the number of arguments accepted by the given method.
 */
OBJC_EXPORT unsigned int method_getNumberOfArguments(Method m)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Returns a string describing a method's return type.                  返回描述方法返回类型，字符串
 * 
 * @param m The method to inspect.
 * 
 * @return A C string describing the return type. You must free the string with \c free().
 */
OBJC_EXPORT char *method_copyReturnType(Method m) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns a string describing a single parameter type of a method.     返回某个指定描述方法参数描述，字符串
 * 
 * @param m The method to inspect.
 * @param index The index of the parameter to inspect.
 * 
 * @return A C string describing the type of the parameter at index \e index, or \c NULL
 *  if method has no parameter index \e index. You must free the string with \c free().
 */
OBJC_EXPORT char *method_copyArgumentType(Method m, unsigned int index) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns by reference a string describing a method's return type.                 获取函数的返回类型，结果赋值到引用字符串dst
 * 
 * @param m The method you want to inquire about. 
 * @param dst The reference string to store the description.
 * @param dst_len The maximum number of characters that can be stored in \e dst.
 *
 * @note The method's return type string is copied to \e dst.
 *  \e dst is filled as if \c strncpy(dst, parameter_type, dst_len) were called.
 */
OBJC_EXPORT void method_getReturnType(Method m, char *dst, size_t dst_len) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns by reference a string describing a single parameter type of a method.    获取某个指定描述方法参数描述，结果赋值到引用字符串dst
 * 
 * @param m The method you want to inquire about. 
 * @param index The index of the parameter you want to inquire about.
 * @param dst The reference string to store the description.
 * @param dst_len The maximum number of characters that can be stored in \e dst.
 * 
 * @note The parameter type string is copied to \e dst. \e dst is filled as if \c strncpy(dst, parameter_type, dst_len) 
 *  were called. If the method contains no parameter with that index, \e dst is filled as
 *  if \c strncpy(dst, "", dst_len) were called.
 */
OBJC_EXPORT void method_getArgumentType(Method m, unsigned int index, 
                                        char *dst, size_t dst_len) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);
OBJC_EXPORT struct objc_method_description *method_getDescription(Method m) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Sets the implementation of a method.                 设置方法的IMP指针
 * 
 * @param m The method for which to set an implementation.
 * @param imp The implemention to set to this method.
 * 
 * @return The previous implementation of the method.
 */
OBJC_EXPORT IMP method_setImplementation(Method m, IMP imp) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Exchanges the implementations of two methods.        交换两个方法的IMP指针。（swizz）
 * 
 * @param m1 Method to exchange with second method.
 * @param m2 Method to exchange with first method.
 * 
 * @note This is an atomic version of the following:
 *  \code 
 *  IMP imp1 = method_getImplementation(m1);
 *  IMP imp2 = method_getImplementation(m2);
 *  method_setImplementation(m1, imp2);
 *  method_setImplementation(m2, imp1);
 *  \endcode
 */
OBJC_EXPORT void method_exchangeImplementations(Method m1, Method m2) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

#pragma mark - 操作实例变量的函数
/* Working with Instance Variables */

/** 
 * Returns the name of an instance variable.            返回变量的名称
 * 
 * @param v The instance variable you want to enquire about.
 * 
 * @return A C string containing the instance variable's name.
 */
OBJC_EXPORT const char *ivar_getName(Ivar v) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns the type string of an instance variable.     返回变量的类型，字符串（类型详见，Type Encodings）
 * 
 * @param v The instance variable you want to enquire about.
 * 
 * @return A C string containing the instance variable's type encoding.
 *
 * @note For possible values, see Objective-C Runtime Programming Guide > Type Encodings.
 */
OBJC_EXPORT const char *ivar_getTypeEncoding(Ivar v) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns the offset of an instance variable.          获取变量的基地址偏移字节，关于变量的阅读：http://www.jianshu.com/p/1e06bfee99d0/comments/2475893
 * 
 * @param v The instance variable you want to enquire about.
 * 
 * @return The offset of \e v.
 * 
 * @note For instance variables of type \c id or other object types, call \c object_getIvar
 *  and \c object_setIvar instead of using this offset to access the instance variable data directly.
 */
OBJC_EXPORT ptrdiff_t ivar_getOffset(Ivar v) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);


#pragma mark - 操作属性的函数
/* Working with Properties */

/** 
 * Returns the name of a property.              返回属性名称
 * 
 * @param property The property you want to inquire about.
 * 
 * @return A C string containing the property's name.
 */
OBJC_EXPORT const char *property_getName(objc_property_t property) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns the attribute string of a property.  返回属性(property)的属性(attribute)
 * 
 * @param property A property.
 *
 * @return A C string containing the property's attributes.
 * 
 * @note The format of the attribute string is described in Declared Properties in Objective-C Runtime Programming Guide.
 */
OBJC_EXPORT const char *property_getAttributes(objc_property_t property) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns an array of property attributes for a property.              返回多个属性的属性
 * 
 * @param property The property whose attributes you want copied.
 * @param outCount The number of attributes returned in the array.
 * 
 * @return An array of property attributes; must be free'd() by the caller. 
 */
OBJC_EXPORT objc_property_attribute_t *property_copyAttributeList(objc_property_t property, unsigned int *outCount)
     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

/** 
 * Returns the value of a property attribute given the attribute name.  根据属性和attributeName，返回attribute
 * 
 * @param property The property whose attribute value you are interested in.
 * @param attributeName C string representing the attribute name.
 *
 * @return The value string of the attribute \e attributeName if it exists in
 *  \e property, \c nil otherwise. 
 */
OBJC_EXPORT char *property_copyAttributeValue(objc_property_t property, const char *attributeName)
     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);


#pragma mark - 操作协议的函数
/* Working with Protocols */

/** 
 * Returns a specified protocol.        返回指定名称的协议
 * 
 * @param name The name of a protocol.
 * 
 * @return The protocol named \e name, or \c NULL if no protocol named \e name could be found.
 * 
 * @note This function acquires the runtime lock.
 */
OBJC_EXPORT Protocol *objc_getProtocol(const char *name)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns an array of all the protocols known to the runtime.  返回runtime中所有已知协议
 * 
 * @param outCount Upon return, contains the number of protocols in the returned array.
 * 
 * @return A C array of all the protocols known to the runtime. The array contains \c *outCount
 *  pointers followed by a \c NULL terminator. You must free the list with \c free().
 * 
 * @note This function acquires the runtime lock.
 */
OBJC_EXPORT Protocol * __unsafe_unretained *objc_copyProtocolList(unsigned int *outCount)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns a Boolean value that indicates whether one protocol conforms to another protocol. 判断一个协议是否遵从于另一个协议，Bool
 * 
 * @param proto A protocol.
 * @param other A protocol.
 * 
 * @return \c YES if \e proto conforms to \e other, otherwise \c NO.
 * 
 * @note One protocol can incorporate other protocols using the same syntax 
 *  that classes use to adopt a protocol:
 *  \code
 *  @protocol ProtocolName < protocol list >
 *  \endcode
 *  All the protocols listed between angle brackets are considered part of the ProtocolName protocol.
 */
OBJC_EXPORT BOOL protocol_conformsToProtocol(Protocol *proto, Protocol *other)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns a Boolean value that indicates whether two protocols are equal.      判断两个协议是否一致，Bool
 * 
 * @param proto A protocol.
 * @param other A protocol.
 * 
 * @return \c YES if \e proto is the same as \e other, otherwise \c NO.
 */
OBJC_EXPORT BOOL protocol_isEqual(Protocol *proto, Protocol *other)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns the name of a protocol.  返回协议名称
 * 
 * @param p A protocol.
 * 
 * @return The name of the protocol \e p as a C string.
 */
OBJC_EXPORT const char *protocol_getName(Protocol *p)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns a method description structure for a specified method of a given protocol.   返回一个 objc_method_description 描述协议中的指定方法
 * 
 * @param p A protocol.
 * @param aSel A selector.
 * @param isRequiredMethod A Boolean value that indicates whether aSel is a required method.
 * @param isInstanceMethod A Boolean value that indicates whether aSel is an instance method.
 * 
 * @return An \c objc_method_description structure that describes the method specified by \e aSel,
 *  \e isRequiredMethod, and \e isInstanceMethod for the protocol \e p.
 *  If the protocol does not contain the specified method, returns an \c objc_method_description structure
 *  with the value \c {NULL, \c NULL}.
 * 
 * @note This function recursively searches any protocols that this protocol conforms to.
 */
OBJC_EXPORT struct objc_method_description protocol_getMethodDescription(Protocol *p, SEL aSel, BOOL isRequiredMethod, BOOL isInstanceMethod)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns an array of method descriptions of methods meeting a given specification for a given protocol.   返回一个 objc_method_description数组， 描述协议中的多个指定方法
 * 
 * @param p A protocol.
 * @param isRequiredMethod A Boolean value that indicates whether returned methods should
 *  be required methods (pass YES to specify required methods).
 * @param isInstanceMethod A Boolean value that indicates whether returned methods should
 *  be instance methods (pass YES to specify instance methods).
 * @param outCount Upon return, contains the number of method description structures in the returned array.
 * 
 * @return A C array of \c objc_method_description structures containing the names and types of \e p's methods 
 *  specified by \e isRequiredMethod and \e isInstanceMethod. The array contains \c *outCount pointers followed
 *  by a \c NULL terminator. You must free the list with \c free().
 *  If the protocol declares no methods that meet the specification, \c NULL is returned and \c *outCount is 0.
 * 
 * @note Methods in other protocols adopted by this protocol are not included.
 */
OBJC_EXPORT struct objc_method_description *protocol_copyMethodDescriptionList(Protocol *p, BOOL isRequiredMethod, BOOL isInstanceMethod, unsigned int *outCount)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns the specified property of a given protocol.  获取协议中的属性
 * 
 * @param proto A protocol.
 * @param name The name of a property.
 * @param isRequiredProperty A Boolean value that indicates whether name is a required property.
 * @param isInstanceProperty A Boolean value that indicates whether name is a required property.
 * 
 * @return The property specified by \e name, \e isRequiredProperty, and \e isInstanceProperty for \e proto,
 *  or \c NULL if none of \e proto's properties meets the specification.
 */
OBJC_EXPORT objc_property_t protocol_getProperty(Protocol *proto, const char *name, BOOL isRequiredProperty, BOOL isInstanceProperty)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns an array of the properties declared by a protocol.   获取协议中指定个数的多个属性
 * 
 * @param proto A protocol.
 * @param outCount Upon return, contains the number of elements in the returned array.
 * 
 * @return A C array of pointers of type \c objc_property_t describing the properties declared by \e proto.
 *  Any properties declared by other protocols adopted by this protocol are not included. The array contains
 *  \c *outCount pointers followed by a \c NULL terminator. You must free the array with \c free().
 *  If the protocol declares no properties, \c NULL is returned and \c *outCount is \c 0.
 */
OBJC_EXPORT objc_property_t *protocol_copyPropertyList(Protocol *proto, unsigned int *outCount)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns an array of the protocols adopted by a protocol. 返回协议中采用的协议数组
 * 
 * @param proto A protocol.
 * @param outCount Upon return, contains the number of elements in the returned array.
 * 
 * @return A C array of protocols adopted by \e proto. The array contains \e *outCount pointers
 *  followed by a \c NULL terminator. You must free the array with \c free().
 *  If the protocol declares no properties, \c NULL is returned and \c *outCount is \c 0.
 */
OBJC_EXPORT Protocol * __unsafe_unretained *protocol_copyProtocolList(Protocol *proto, unsigned int *outCount)
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Creates a new protocol instance that cannot be used until registered with    创建一个新协议实例，在 objc_registerProtocol() 注册前不能使用
 * \c objc_registerProtocol()
 * 
 * @param name The name of the protocol to create.
 *
 * @return The Protocol instance on success, \c nil if a protocol
 *  with the same name already exists. 
 * @note There is no dispose method for this. 
 */
OBJC_EXPORT Protocol *objc_allocateProtocol(const char *name) 
     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

/** 
 * Registers a newly constructed protocol with the runtime. The protocol        runtime中注册一个新构建的协议，注册后可直接使用
 * will be ready for use and is immutable after this.
 * 
 * @param proto The protocol you want to register.
 */
OBJC_EXPORT void objc_registerProtocol(Protocol *proto) 
     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

/** 
 * Adds a method to a protocol. The protocol must be under construction.        给协议中添加方法，协议必须已经构建
 * 
 * @param proto The protocol to add a method to.
 * @param name The name of the method to add.
 * @param types A C string that represents the method signature.
 * @param isRequiredMethod YES if the method is not an optional method.
 * @param isInstanceMethod YES if the method is an instance method. 
 */
OBJC_EXPORT void protocol_addMethodDescription(Protocol *proto, SEL name, const char *types, BOOL isRequiredMethod, BOOL isInstanceMethod) 
     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

/** 
 * Adds an incorporated protocol to another protocol. The protocol being        合并两个协议，两个协议必须已构建
 * added to must still be under construction, while the additional protocol
 * must be already constructed.
 * 
 * @param proto The protocol you want to add to, it must be under construction.
 * @param addition The protocol you want to incorporate into \e proto, it must be registered.
 */
OBJC_EXPORT void protocol_addProtocol(Protocol *proto, Protocol *addition) 
     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

/** 
 * Adds a property to a protocol. The protocol must be under construction.      添加一个属性到协议中，协议必须已构建
 * 
 * @param proto The protocol to add a property to.
 * @param name The name of the property.
 * @param attributes An array of property attributes.
 * @param attributeCount The number of attributes in \e attributes.
 * @param isRequiredProperty YES if the property (accessor methods) is not optional. 
 * @param isInstanceProperty YES if the property (accessor methods) are instance methods. 
 *  This is the only case allowed fo a property, as a result, setting this to NO will 
 *  not add the property to the protocol at all. 
 */
OBJC_EXPORT void protocol_addProperty(Protocol *proto, const char *name, const objc_property_attribute_t *attributes, unsigned int attributeCount, BOOL isRequiredProperty, BOOL isInstanceProperty)
     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);


#pragma mark - 操作库的函数
/* Working with Libraries */

/** 
 * Returns the names of all the loaded Objective-C frameworks and dynamic   返回所有加载的oc框架和动态库名称
 * libraries.
 * 
 * @param outCount The number of names returned.
 * 
 * @return An array of C strings of names. Must be free()'d by caller.
 */
OBJC_EXPORT const char **objc_copyImageNames(unsigned int *outCount) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns the dynamic library name a class originated from.                返回指定类归属的动态库名称
 * 
 * @param cls The class you are inquiring about.
 * 
 * @return The name of the library containing this class.
 */
OBJC_EXPORT const char *class_getImageName(Class cls) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Returns the names of all the classes within a library.                   返回指定库中所有类的名字
 * 
 * @param image The library or framework you are inquiring about.
 * @param outCount The number of class names returned.
 * 
 * @return An array of C strings representing the class names.
 */
OBJC_EXPORT const char **objc_copyClassNamesForImage(const char *image, 
                                                     unsigned int *outCount) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);


#pragma mark - 操作 Selector 的函数
/* Working with Selectors */

/** 
 * Returns the name of the method specified by a given selector.            返回指定方法选择器的方法名
 * 
 * @param sel A pointer of type \c SEL. Pass the selector whose name you wish to determine.
 * 
 * @return A C string indicating the name of the selector.
 */
OBJC_EXPORT const char *sel_getName(SEL sel)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Registers a method name with the Objective-C runtime system.             在runtime system中注册一个方法
 * 
 * @param str A pointer to a C string. Pass the name of the method you wish to register.
 * 
 * @return A pointer of type SEL specifying the selector for the named method.
 * 
 * @note The implementation of this method is identical to the implementation of \c sel_registerName.
 * @note Prior to OS X version 10.0, this method tried to find the selector mapped to the given name
 *  and returned \c NULL if the selector was not found. This was changed for safety, because it was
 *  observed that many of the callers of this function did not check the return value for \c NULL.
 */
OBJC_EXPORT SEL sel_getUid(const char *str)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Registers a method with the Objective-C runtime system, maps the method  在runtime system中注册一个方法, 并且对应方法选择器，返回方法选择器
 * name to a selector, and returns the selector value.
 * 
 * @param str A pointer to a C string. Pass the name of the method you wish to register.
 * 
 * @return A pointer of type SEL specifying the selector for the named method.
 * 
 * @note You must register a method name with the Objective-C runtime system to obtain the
 *  method’s selector before you can add the method to a class definition. If the method name
 *  has already been registered, this function simply returns the selector.
 */
OBJC_EXPORT SEL sel_registerName(const char *str)
    __OSX_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/** 
 * Returns a Boolean value that indicates whether two selectors are equal.  判断方法是否一致，返回bool结果
 * 
 * @param lhs The selector to compare with rhs.
 * @param rhs The selector to compare with lhs.
 * 
 * @return \c YES if \e rhs and \e rhs are equal, otherwise \c NO.
 * 
 * @note sel_isEqual is equivalent to ==.
 */
OBJC_EXPORT BOOL sel_isEqual(SEL lhs, SEL rhs) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);


#pragma mark - Objective-C 语言特性的函数
/* Objective-C Language Features */

/** 
 * This function is inserted by the compiler when a mutation
 * is detected during a foreach iteration. It gets called 
 * when a mutation occurs, and the enumerationMutationHandler
 * is enacted if it is set up. A fatal error occurs if a handler is not set up.
 *
 * @param obj The object being mutated.
 * 
 */
OBJC_EXPORT void objc_enumerationMutation(id obj) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Sets the current mutation handler. 
 * 
 * @param handler Function pointer to the new mutation handler.
 */
OBJC_EXPORT void objc_setEnumerationMutationHandler(void (*handler)(id)) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Set the function to be called by objc_msgForward.
 * 
 * @param fwd Function to be jumped to by objc_msgForward.
 * @param fwd_stret Function to be jumped to by objc_msgForward_stret.
 * 
 * @see message.h::_objc_msgForward
 */
OBJC_EXPORT void objc_setForwardHandler(void *fwd, void *fwd_stret) 
     __OSX_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/** 
 * Creates a pointer to a function that will call the block
 * when the method is called.
 * 
 * @param block The block that implements this method. Its signature should
 *  be: method_return_type ^(id self, method_args...). 
 *  The selector is not available as a parameter to this block.
 *  The block is copied with \c Block_copy().
 * 
 * @return The IMP that calls this block. Must be disposed of with
 *  \c imp_removeBlock.
 */
OBJC_EXPORT IMP imp_implementationWithBlock(id block)
     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

/** 
 * Return the block associated with an IMP that was created using
 * \c imp_implementationWithBlock.
 * 
 * @param anImp The IMP that calls this block.
 * 
 * @return The block called by \e anImp.
 */
OBJC_EXPORT id imp_getBlock(IMP anImp)
     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

/** 
 * Disassociates a block from an IMP that was created using
 * \c imp_implementationWithBlock and releases the copy of the 
 * block that was created.
 * 
 * @param anImp An IMP that was created using \c imp_implementationWithBlock.
 * 
 * @return YES if the block was released successfully, NO otherwise. 
 *  (For example, the block might not have been used to create an IMP previously).
 */
OBJC_EXPORT BOOL imp_removeBlock(IMP anImp)
     __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_4_3);

/** 
 * This loads the object referenced by a weak pointer and returns it, after
 * retaining and autoreleasing the object to ensure that it stays alive
 * long enough for the caller to use it. This function would be used
 * anywhere a __weak variable is used in an expression.
 * 
 * @param location The weak pointer address
 * 
 * @return The object pointed to by \e location, or \c nil if \e location is \c nil.
 */
OBJC_EXPORT id objc_loadWeak(id *location)
    __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_5_0);

/** 
 * This function stores a new value into a __weak variable. It would
 * be used anywhere a __weak variable is the target of an assignment.
 * 
 * @param location The address of the weak pointer itself
 * @param obj The new object this weak ptr should now point to
 * 
 * @return The value stored into \e location, i.e. \e obj
 */
OBJC_EXPORT id objc_storeWeak(id *location, id obj) 
    __OSX_AVAILABLE_STARTING(__MAC_10_7, __IPHONE_5_0);


#pragma mark - 关联引用
/* Associative References */

// 允许开发者为任何对象附着键值数据，关联引用基于键的内存地址，而不是值的.
// 参考：http://blog.csdn.net/chaoyuan899/article/details/46672155
// 具体用法，见：https://github.com/forkingdog/FDFullscreenPopGesture/blob/master/FDFullscreenPopGesture/UINavigationController%2BFDFullscreenPopGesture.m

/** 
 * Policies related to associative references.      关联引用的策略
 *
 * These are options to objc_setAssociatedObject()  objc_setAssociatedObject() 函数选项
 *
 */
enum {
    OBJC_ASSOCIATION_ASSIGN = 0,           /**< Specifies a weak reference to the associated object.    指定一个弱引用到被关联的对象  */
    OBJC_ASSOCIATION_RETAIN_NONATOMIC = 1, /**< Specifies a strong reference to the associated object.
                                                The association is not made atomically.                 指定一个强引用到被关联的对象，该关联不是原子性的 */
    OBJC_ASSOCIATION_COPY_NONATOMIC = 3,   /**< Specifies that the associated object is copied.
                                                The association is not made atomically.                 指定被关联的对象是拷贝的，该关联不是原子性的 */
    OBJC_ASSOCIATION_RETAIN = 01401,       /**< Specifies a strong reference to the associated object.
                                                The association is made atomically.                     指定一个强引用到被关联的对象，该关联是原子性的 */
    OBJC_ASSOCIATION_COPY = 01403          /**< Specifies that the associated object is copied.
                                                The association is made atomically.                     指定被关联的对象是拷贝的，该关联是原子性的 */
};

/// Type to specify the behavior of an association.
/// 指定关联行为的类型
typedef uintptr_t objc_AssociationPolicy;

/** 
 * Sets an associated value for a given object using a given key and association policy.
 * 使用给定的键及关联策略，为对象设置值
 * 
 * @param object The source object for the association.     源对象
 * @param key The key for the association.                  关联的键
 * @param value The value to associate with the key key for object. Pass nil to clear an existing association.      要设置的值，传入 nil 将清除现有关联
 * @param policy The policy for the association. For possible values, see “Associative Object Behaviors.”           关联策略，请参阅"关联对象行为"
 * 
 * @see objc_setAssociatedObject
 * @see objc_removeAssociatedObjects
 */
OBJC_EXPORT void objc_setAssociatedObject(id object, const void *key, id value, objc_AssociationPolicy policy)
__OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_3_1);

/** 
 * Returns the value associated with a given object for a given key.
 * 返回对象给定键关联的值
 * 
 * @param object The source object for the association.     源对象
 * @param key The key for the association.                  关联的键
 * 
 * @return The value associated with the key \e key for \e object.
 *  与 \e object 的 \e key 键关联的值
 * 
 * @see objc_setAssociatedObject
 */
OBJC_EXPORT id objc_getAssociatedObject(id object, const void *key)
__OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_3_1);

/** 
 * Removes all associations for a given object.
 * 删除对象的所有关联
 * 
 * @param object An object that maintains associated objects.   维护关联对象的源对象
 * 
 * @note The main purpose of this function is to make it easy to return an object
 *  to a "pristine state”. You should not use this function for general removal of
 *  associations from objects, since it also removes associations that other clients
 *  may have added to the object. Typically you should use \c objc_setAssociatedObject
 *  with a nil value to clear an association.
 * @note 本函数的主要目的是直接返回一个对象的"原始版本"。通常，不应该使用此函数从对象中删除关联，
 *  因为会同时删除掉其他对象中已经引用的关联属性。通常情况下，应该使用 objc_setAssociatedObject
 *  函数，并传入一个 nil 值清除一个关联
 * 
 * @see objc_setAssociatedObject
 * @see objc_getAssociatedObject
 */
OBJC_EXPORT void objc_removeAssociatedObjects(id object)
    __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_3_1);


//OC中的类型定义，对应C的字符串。用法参见 scan_basic_ivar_type

#define _C_ID       '@'
#define _C_CLASS    '#'
#define _C_SEL      ':'
#define _C_CHR      'c'
#define _C_UCHR     'C'
#define _C_SHT      's'
#define _C_USHT     'S'
#define _C_INT      'i'
#define _C_UINT     'I'
#define _C_LNG      'l'
#define _C_ULNG     'L'
#define _C_LNG_LNG  'q'
#define _C_ULNG_LNG 'Q'
#define _C_FLT      'f'
#define _C_DBL      'd'
#define _C_BFLD     'b'
#define _C_BOOL     'B'
#define _C_VOID     'v'
#define _C_UNDEF    '?'
#define _C_PTR      '^'
#define _C_CHARPTR  '*'
#define _C_ATOM     '%'
#define _C_ARY_B    '['
#define _C_ARY_E    ']'
#define _C_UNION_B  '('
#define _C_UNION_E  ')'
#define _C_STRUCT_B '{'
#define _C_STRUCT_E '}'
#define _C_VECTOR   '!'
#define _C_CONST    'r'


#pragma mark - 废弃的类型
/* Obsolete types */

#if !__OBJC2__

#define CLS_GETINFO(cls,infomask)        ((cls)->info & (infomask))
#define CLS_SETINFO(cls,infomask)        ((cls)->info |= (infomask))

// class is not a metaclass
#define CLS_CLASS               0x1
// class is a metaclass
#define CLS_META                0x2
// class's +initialize method has completed
#define CLS_INITIALIZED         0x4
// class is posing
#define CLS_POSING              0x8
// unused
#define CLS_MAPPED              0x10
// class and subclasses need cache flush during image loading
#define CLS_FLUSH_CACHE         0x20
// method cache should grow when full
#define CLS_GROW_CACHE          0x40
// unused
#define CLS_NEED_BIND           0x80
// methodLists is array of method lists
#define CLS_METHOD_ARRAY        0x100
// the JavaBridge constructs classes with these markers
#define CLS_JAVA_HYBRID         0x200
#define CLS_JAVA_CLASS          0x400
// thread-safe +initialize
#define CLS_INITIALIZING        0x800
// bundle unloading
#define CLS_FROM_BUNDLE         0x1000
// C++ ivar support
#define CLS_HAS_CXX_STRUCTORS   0x2000
// Lazy method list arrays
#define CLS_NO_METHOD_ARRAY     0x4000
// +load implementation
#define CLS_HAS_LOAD_METHOD     0x8000
// objc_allocateClassPair API
#define CLS_CONSTRUCTING        0x10000
// class compiled with bigger class structure
#define CLS_EXT                 0x20000


struct objc_method_description_list {
        int count;
        struct objc_method_description list[1];
};


struct objc_protocol_list {
    struct objc_protocol_list *next;
    long count;
    Protocol *list[1];
};


struct objc_category {
    char *category_name                                      OBJC2_UNAVAILABLE;
    char *class_name                                         OBJC2_UNAVAILABLE;
    struct objc_method_list *instance_methods                OBJC2_UNAVAILABLE;
    struct objc_method_list *class_methods                   OBJC2_UNAVAILABLE;
    struct objc_protocol_list *protocols                     OBJC2_UNAVAILABLE;
}                                                            OBJC2_UNAVAILABLE;


struct objc_ivar {
    char *ivar_name                                          OBJC2_UNAVAILABLE;
    char *ivar_type                                          OBJC2_UNAVAILABLE;
    int ivar_offset                                          OBJC2_UNAVAILABLE;
#ifdef __LP64__
    int space                                                OBJC2_UNAVAILABLE;
#endif
}                                                            OBJC2_UNAVAILABLE;

struct objc_ivar_list {
    int ivar_count                                           OBJC2_UNAVAILABLE;
#ifdef __LP64__
    int space                                                OBJC2_UNAVAILABLE;
#endif
    /* variable length structure */
    struct objc_ivar ivar_list[1]                            OBJC2_UNAVAILABLE;
}                                                            OBJC2_UNAVAILABLE;


struct objc_method {
    SEL method_name                                          OBJC2_UNAVAILABLE;
    char *method_types                                       OBJC2_UNAVAILABLE;
    IMP method_imp                                           OBJC2_UNAVAILABLE;
}                                                            OBJC2_UNAVAILABLE;

struct objc_method_list {
    struct objc_method_list *obsolete                        OBJC2_UNAVAILABLE;

    int method_count                                         OBJC2_UNAVAILABLE;
#ifdef __LP64__
    int space                                                OBJC2_UNAVAILABLE;
#endif
    /* variable length structure */
    struct objc_method method_list[1]                        OBJC2_UNAVAILABLE;
}                                                            OBJC2_UNAVAILABLE;


typedef struct objc_symtab *Symtab                           OBJC2_UNAVAILABLE;

struct objc_symtab {
    unsigned long sel_ref_cnt                                OBJC2_UNAVAILABLE;
    SEL *refs                                                OBJC2_UNAVAILABLE;
    unsigned short cls_def_cnt                               OBJC2_UNAVAILABLE;
    unsigned short cat_def_cnt                               OBJC2_UNAVAILABLE;
    void *defs[1] /* variable size */                        OBJC2_UNAVAILABLE;
}                                                            OBJC2_UNAVAILABLE;


typedef struct objc_cache *Cache                             OBJC2_UNAVAILABLE;

#define CACHE_BUCKET_NAME(B)  ((B)->method_name)
#define CACHE_BUCKET_IMP(B)   ((B)->method_imp)
#define CACHE_BUCKET_VALID(B) (B)
#ifndef __LP64__
#define CACHE_HASH(sel, mask) (((uintptr_t)(sel)>>2) & (mask))
#else
#define CACHE_HASH(sel, mask) (((unsigned int)((uintptr_t)(sel)>>3)) & (mask))
#endif
struct objc_cache {
    unsigned int mask /* total = mask + 1 */                 OBJC2_UNAVAILABLE;
    unsigned int occupied                                    OBJC2_UNAVAILABLE;
    Method buckets[1]                                        OBJC2_UNAVAILABLE;
};


typedef struct objc_module *Module                           OBJC2_UNAVAILABLE;

struct objc_module {
    unsigned long version                                    OBJC2_UNAVAILABLE;
    unsigned long size                                       OBJC2_UNAVAILABLE;
    const char *name                                         OBJC2_UNAVAILABLE;
    Symtab symtab                                            OBJC2_UNAVAILABLE;
}                                                            OBJC2_UNAVAILABLE;

#else

struct objc_method_list;

#endif


#pragma mark - 废弃的函数
/* Obsolete functions */

OBJC_EXPORT IMP class_lookupMethod(Class cls, SEL sel) 
    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_5, __IPHONE_2_0,__IPHONE_2_0);
OBJC_EXPORT BOOL class_respondsToMethod(Class cls, SEL sel)
    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_5, __IPHONE_2_0,__IPHONE_2_0);
OBJC_EXPORT void _objc_flush_caches(Class cls) 
    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_5, __IPHONE_2_0,__IPHONE_2_0);

OBJC_EXPORT id object_copyFromZone(id anObject, size_t nBytes, void *z) 
    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_5, __IPHONE_NA,__IPHONE_NA)
    OBJC_ARC_UNAVAILABLE;
OBJC_EXPORT id object_realloc(id anObject, size_t nBytes)    OBJC2_UNAVAILABLE;
OBJC_EXPORT id object_reallocFromZone(id anObject, size_t nBytes, void *z) OBJC2_UNAVAILABLE;

#define OBSOLETE_OBJC_GETCLASSES 1
OBJC_EXPORT void *objc_getClasses(void)                      OBJC2_UNAVAILABLE;
OBJC_EXPORT void objc_addClass(Class myClass)                OBJC2_UNAVAILABLE;
OBJC_EXPORT void objc_setClassHandler(int (*)(const char *)) OBJC2_UNAVAILABLE;
OBJC_EXPORT void objc_setMultithreaded (BOOL flag)           OBJC2_UNAVAILABLE;

OBJC_EXPORT id class_createInstanceFromZone(Class, size_t idxIvars, void *z)  
    __OSX_AVAILABLE_BUT_DEPRECATED(__MAC_10_0,__MAC_10_5, __IPHONE_NA,__IPHONE_NA)
    OBJC_ARC_UNAVAILABLE;

OBJC_EXPORT void class_addMethods(Class, struct objc_method_list *) OBJC2_UNAVAILABLE;
OBJC_EXPORT void class_removeMethods(Class, struct objc_method_list *) OBJC2_UNAVAILABLE;
OBJC_EXPORT void _objc_resolve_categories_for_class(Class cls)  OBJC2_UNAVAILABLE;

OBJC_EXPORT Class class_poseAs(Class imposter, Class original) OBJC2_UNAVAILABLE;

OBJC_EXPORT unsigned int method_getSizeOfArguments(Method m) OBJC2_UNAVAILABLE;
OBJC_EXPORT unsigned method_getArgumentInfo(struct objc_method *m, int arg, const char **type, int *offset) OBJC2_UNAVAILABLE;

OBJC_EXPORT Class objc_getOrigClass(const char *name)        OBJC2_UNAVAILABLE;
#define OBJC_NEXT_METHOD_LIST 1
OBJC_EXPORT struct objc_method_list *class_nextMethodList(Class, void **) OBJC2_UNAVAILABLE;
// usage for nextMethodList
//
// void *iterator = 0;
// struct objc_method_list *mlist;
// while ( mlist = class_nextMethodList( cls, &iterator ) )
//    ;
 
OBJC_EXPORT id (*_alloc)(Class, size_t)                      OBJC2_UNAVAILABLE;
OBJC_EXPORT id (*_copy)(id, size_t)                          OBJC2_UNAVAILABLE;
OBJC_EXPORT id (*_realloc)(id, size_t)                       OBJC2_UNAVAILABLE;
OBJC_EXPORT id (*_dealloc)(id)                               OBJC2_UNAVAILABLE;
OBJC_EXPORT id (*_zoneAlloc)(Class, size_t, void *)          OBJC2_UNAVAILABLE;
OBJC_EXPORT id (*_zoneRealloc)(id, size_t, void *)           OBJC2_UNAVAILABLE;
OBJC_EXPORT id (*_zoneCopy)(id, size_t, void *)              OBJC2_UNAVAILABLE;
OBJC_EXPORT void (*_error)(id, const char *, va_list)        OBJC2_UNAVAILABLE;

#endif
