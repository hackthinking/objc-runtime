//
//  main.m
//  debug-objc
//
//  Created by closure on 2/24/16.
//
//

#import <Foundation/Foundation.h>

@interface Subject : NSObject
+(instancetype)getInstance;

@end

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        Subject *ia = [Subject getInstance];
        
        // insert code here...
        NSLog(@"Hello, World! %@", ia);
    }
    
    return 0;
}


@implementation Subject

static Subject *sharedInstance = nil;

+(instancetype)getInstance
{
    @synchronized(self)
    {
        // 实例对象只分配一次
        if(sharedInstance == nil)
        {
            //            sharedInstance = [[super allocWithZone:NULL] init];
            sharedInstance = [[Subject alloc] init];
            
            // 初始化
        }
    }
    return sharedInstance;
}

//+ (id)allocWithZone:(NSZone *)zone
//{
//    return [self getInstance];
//}
//
//- (id)copyWithZone:(NSZone *)zone
//{
//    return self;
//}

@end
