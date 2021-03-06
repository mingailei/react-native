/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <Foundation/Foundation.h>

@class RCTBridge;

/**
 * The type of a block that is capable of sending a response to a bridged
 * operation. Use this for returning callback methods to JS.
 */
typedef void (^RCTResponseSenderBlock)(NSArray *response);

/**
 * Provides the interface needed to register a bridge module.
 */
@protocol RCTBridgeModule <NSObject>
@optional

/**
 * A reference to the RCTBridge. Useful for modules that require access
 * to bridge features, such as sending events or making JS calls. This
 * will be set automatically by the bridge when it initializes the module.
* To implement this in your module, just add @synthesize bridge = _bridge;
 */
@property (nonatomic, strong) RCTBridge *bridge;

/**
 * Place this macro in your class implementation, to automatically register
 * your module with the bridge when it loads. The optional js_name argument
 * will be used as the JS module name. If omitted, the JS module name will
 * match the Objective-C class name.
 */
#define RCT_EXPORT_MODULE(js_name) \
+ (NSString *)moduleName { __attribute__((used, section("__DATA,RCTExportModule" \
))) static const char *__rct_export_entry__ = { __func__ }; return @#js_name; } \

/**
 * Place this macro inside the method body of any method you want to expose
 * to JS. The optional js_name argument will be used as the JS method name
 * (the method will be namespaced to the module name, as specified above).
 * If omitted, the JS method name will match the first part of the Objective-C
 * method selector name (up to the first colon).
 */
#define RCT_EXPORT(js_name) \
  _Pragma("message(\"RCT_EXPORT is deprecated. Use RCT_EXPORT_METHOD instead.\")") \
  __attribute__((used, section("__DATA,RCTExport"))) \
  static const char *__rct_export_entry__[] = { __func__, #js_name }

/**
 * Wrap the parameter line of your method implementation with this macro to
 * expose it to JS. Unlike the deprecated RCT_EXPORT, this macro does not take
 * a js_name argument and the exposed method will match the first part of the
 * Objective-C method selector name (up to the first colon).
 *
 * For example, in MyClass.m:
 *
 * - (void)doSomething:(NSString *)aString withA:(NSInteger)a andB:(NSInteger)b
 * {}
 *
 * becomes
 *
 * RCT_EXPORT_METHOD(doSomething:(NSString *)aString
 *                   withA:(NSInteger)a
 *                   andB:(NSInteger)b)
 * {}
 *
 * and is exposed to JavaScript as `NativeModules.ModuleName.doSomething`.
 */
#define RCT_EXPORT_METHOD(method) \
  - (void)__rct_export__##method { \
    __attribute__((used, section("__DATA,RCTExport"))) \
    static const char *__rct_export_entry__[] = { __func__, #method }; \
  } \
  - (void)method

/**
 * Injects constants into JS. These constants are made accessible via
 * NativeModules.ModuleName.X. This method is called when the module is
 * registered by the bridge. It is only called once for the lifetime of the
 * bridge, so it is not suitable for returning dynamic values, but may be
 * used for long-lived values such as session keys, that are regenerated only
 * as part of a reload of the entire React application.
 */
- (NSDictionary *)constantsToExport;

/**
 * Notifies the module that a batch of JS method invocations has just completed.
 */
- (void)batchDidComplete;

@end

#ifdef __cplusplus
extern "C" {
#endif
void RCTBridgeModuleRegisterClass(Class cls, NSString *moduleName);
#ifdef __cplusplus
}
#endif
