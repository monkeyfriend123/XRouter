//
//  XRouter.hpp
//  XRouter
//
//  Created by GSDN on 2017/4/19.
//  Copyright © 2017年 js. All rights reserved.
//

#ifndef XRouter_hpp
#define XRouter_hpp

#include <stdio.h>
#include <string>
#include <functional>
#include <map>
#include <vector>

using namespace std;

extern string const XRouterParameterURL;
extern string const XRouterParameterCompletion;
extern string const XRouterParameterUserInfo;

/**
 *  routerParameters 里内置的几个参数会用到上面定义的 string
 */
//typedef void (^MGJRouterHandler)(NSDictionary *routerParameters);

typedef function<void(map<string,void *>)> XRouterHandler;

/**
 *  需要返回一个 object，配合 objectForURL: 使用
 */
typedef function<void(map<string,void *>)> XRouterObjectHandler;

typedef function<void(void *)> XResultHandler;


typedef map<string,void *> RouterMap;

class XRouter{
private:
    RouterMap *routes;
/**
 *  注册 URLPattern 对应的 Handler，在 handler 中可以初始化 VC，然后对 VC 做各种操作
 *
 *  @param URLPattern 带上 scheme，如 mgj://beauty/:id
 *  @param handler    该 block 会传一个字典，包含了注册的 URL 中对应的变量。
 *                    假如注册的 URL 为 mgj://beauty/:id 那么，就会传一个 @{@"id": 4} 这样的字典过来
 */
public:    
    RouterMap* getRoutes();
    void setRoutes(RouterMap *routes);
    //单例
    static XRouter* getInstance();
    
    static void registerURLPattern(string urlPattern,XRouterHandler handler);


/**
 *  注册 URLPattern 对应的 ObjectHandler，需要返回一个 object 给调用方
 *
 *  @param urlPattern 带上 scheme，如 mgj://beauty/:id
 *  @param handler    该 block 会传一个字典，包含了注册的 URL 中对应的变量。
 *                    假如注册的 URL 为 mgj://beauty/:id 那么，就会传一个 @{@"id": 4} 这样的字典过来
 *                    自带的 key 为 @"url" 和 @"completion" (如果有的话)
 */
    static void registerObjectURLPattern(string urlPattern, XRouterObjectHandler handler);


/**
 *  取消注册某个 URL Pattern
 *
 *  @param urlPattern
 */
static void deregisterURLPattern(string urlPattern);


/**
 *  打开此 URL
 *  会在已注册的 URL -> Handler 中寻找，如果找到，则执行 Handler
 *
 *  @param URL 带 Scheme，如 mgj://beauty/3
 */
static void openURL(string URL);
/**
 *  打开此 URL，同时当操作完成时，执行额外的代码
 *
 *  @param URL        带 Scheme 的 URL，如 mgj://beauty/4
 *  @param completion URL 处理完成后的 callback，完成的判定跟具体的业务相关
 */
static void openURL(string URL, XResultHandler completion);


/**
 *  打开此 URL，带上附加信息，同时当操作完成时，执行额外的代码
 *
 *  @param URL        带 Scheme 的 URL，如 mgj://beauty/4
 *  @param parameters 附加参数
 *  @param completion URL 处理完成后的 callback，完成的判定跟具体的业务相关
 */
static void openURL(string URL, map<string,void *>* parameters,XResultHandler completion);

RouterMap * extractParametersFromURL(string url);
/**
 * 查找谁对某个 URL 感兴趣，如果有的话，返回一个 object
 *
 *  @param URL
 */

static void* objectForURL(string URL);
/**
 * 查找谁对某个 URL 感兴趣，如果有的话，返回一个 object
 *
 *  @param URL
 *  @param userInfo
 */
static void* objectForURL(string URL,RouterMap userInfo);

/**
 *  是否可以打开URL
 *
 *  @param URL
 *
 *  @return
 */

static bool canOpenURL(string URL);

/**
 *  调用此方法来拼接 urlpattern 和 parameters
 *
 *  #define MGJ_ROUTE_BEAUTY @"beauty/:id"
 *  [MGJRouter generateURLWithPattern:MGJ_ROUTE_BEAUTY, @[@13]];
 *
 *
 *  @param pattern    url pattern 比如 @"beauty/:id"
 *  @param parameters 一个数组，数量要跟 pattern 里的变量一致
 *
 *  @return
 */
    
static string generateURLWithPattern(string pattern,vector<void *> parameters);


void addURLPattern(string URLPattern, XRouterHandler handler);
void addObjectURLPattern(string URLPattern, XRouterObjectHandler handler);
    
RouterMap* addURLPattern(string URLPattern);

    
vector<string> pathComponentsFromURL(string URL);
    
static bool checkIfContainsSpecialCharacter(string key);
    
};


#endif /* XRouter_hpp */
