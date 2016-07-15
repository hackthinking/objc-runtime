//
//  main.m
//  debug-objc
//
//  Created by closure on 2/24/16.
//
//

#import <Foundation/Foundation.h>
#import "objc.h"
#import "objc-runtime.h"


@interface classA : NSObject

@property (nonatomic, strong) NSString *nameA;

@end

@implementation classA

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.nameA = @"AAAAAAA";
    }
    return self;
}

@end

@interface classB : NSObject

@property (nonatomic, strong) NSString *nameB;

@end

@implementation classB

- (instancetype)init
{
    self = [super init];
    if (self) {
        self.nameB = @"BBBBBBB";
    }
    return self;
}

@end

@interface classC : classA

@end

@implementation classC

- (instancetype)init
{
    self = [super init];
    if (self) {
//        NSLog(@"nameA %@", self.nameA);
        self.nameA = @"CCCCCCC";
//        NSLog(@"nameA %@", self.nameA);
    }
    return self;
}

@end

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        
        classA *a = [[classA alloc] init];
        classB *b = [[classB alloc] init];
        classC *c = [[classC alloc] init];
        
        NSLog(@"a %@", class_getSuperclass(object_getClass(a)) );
        NSLog(@"b %@", class_getSuperclass([b class]) );
        NSLog(@"c %@", class_getSuperclass([c class]) );
        NSLog(@"version %d", class_getVersion(object_getClass(b)));
    }
    
    return 0;
}
