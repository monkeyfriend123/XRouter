//
//  XRouter.cpp
//  XRouter
//
//  Created by GSDN on 2017/4/19.
//  Copyright © 2017年 js. All rights reserved.
//

#include "XRouter.hpp"
#include "Util.h"
#include <algorithm>

static string const X_ROUTER_WILDCARD_CHARACTER = "~";
static string specialCharacters = "/?&.";

string const XRouterParameterURL = "MGJRouterParameterURL";
string const XRouterParameterCompletion = "MGJRouterParameterCompletion";
string const XRouterParameterUserInfo = "MGJRouterParameterUserInfo";

static XRouter *_instance = NULL;
XRouter* XRouter::getInstance(){
    if (_instance == NULL){
        _instance = new XRouter();
    }
    return _instance;
}


RouterTree *XRouter::getHeaderRouter(){
    if (headRouter == NULL){
        headRouter = new RouterTree();
    }
    return headRouter;
}
/**
 *  注册 URLPattern 对应的 Handler，在 handler 中可以初始化 VC，然后对 VC 做各种操作
 *
 *  @param urlPattern 带上 scheme，如 mgj://beauty/:id
 *  @param handler    该 block 会传一个字典，包含了注册的 URL 中对应的变量。
 *                    假如注册的 URL 为 mgj://beauty/:id 那么，就会传一个 @{@"id": 4} 这样的字典过来
 */


void XRouter::registerURLPattern(string urlPattern,XRouterHandler handler){
   XRouter::getInstance()->addURLPattern(urlPattern, handler);
}


/**
 *  注册 URLPattern 对应的 ObjectHandler，需要返回一个 object 给调用方
 *
 *  @param urlPattern 带上 scheme，如 mgj://beauty/:id
 *  @param handler    该 block 会传一个字典，包含了注册的 URL 中对应的变量。
 *                    假如注册的 URL 为 mgj://beauty/:id 那么，就会传一个 @{@"id": 4} 这样的字典过来
 *                    自带的 key 为 @"url" 和 @"completion" (如果有的话)
 */
 void XRouter::registerObjectURLPattern(string urlPattern,XRouterObjectHandler handler){
     XRouter::getInstance()->addObjectURLPattern(urlPattern, handler);
}


/**
 *  取消注册某个 URL Pattern
 *
 *  @param urlPattern
 */
void XRouter::deregisterURLPattern(string urlPattern){
    
}


/**
 *  打开此 URL
 *  会在已注册的 URL -> Handler 中寻找，如果找到，则执行 Handler
 *
 *  @param URL 带 Scheme，如 mgj://beauty/3
 */
void XRouter::openURL(string URL){
    XRouter::openURL(URL, NULL);
}
/**
 *  打开此 URL，同时当操作完成时，执行额外的代码
 *
 *  @param URL        带 Scheme 的 URL，如 mgj://beauty/4
 *  @param completion URL 处理完成后的 callback，完成的判定跟具体的业务相关
 */
void XRouter::openURL(string URL, XResultHandler completion){
    XRouter::openURL(URL, NULL, NULL);
}


/**
 *  打开此 URL，带上附加信息，同时当操作完成时，执行额外的代码
 *
 *  @param URL        带 Scheme 的 URL，如 mgj://beauty/4
 *  @param parameters 附加参数
 *  @param completion URL 处理完成后的 callback，完成的判定跟具体的业务相关
 */
void XRouter::openURL(string URL, ParamsMap* userInfo,XResultHandler completion){
    auto parameters = XRouter::getInstance()->extractParametersFromURL(URL);
    if (parameters) {
        ParamBox *handler =NULL;
        if (parameters->find("block")!= parameters->end()){
            handler = (ParamBox *)parameters->at("block");
        }
        
//        if (completion) {
//            parameters->insert(make_pair(XRouterParameterCompletion, &completion));
//        }
//        if (userInfo) {
//            parameters->insert(make_pair(XRouterParameterUserInfo, userInfo));
//        }
        if (handler->type == 1) {
//            parameters->erase("block");
//            (*handler)(*parameters);
            
            handler->handler(parameters);
        }
    }
}

bool Comp(const string &a,const string &b)
{
    return a>b;
}

ParamsMap * XRouter::extractParametersFromURL(string url)
{
    ParamsMap* parameters = new ParamsMap();
    
    ParamBox *box = new ParamBox();
    box->simpleValue = url;
    box->type = 0;
    parameters->insert(make_pair(XRouterParameterURL, box));
    
    RouterTree* subRoutes = this->getHeaderRouter();
    
    auto pathComponents = this->pathComponentsFromURL(url);
    
    bool found = false;
    
    XRouterHandler recentestCompareBlock = NULL;//最近的匹配值
    // borrowed from HHRouter(https://github.com/Huohua/HHRouter)
    for (int i = 0 ;i < pathComponents.size(); i++){
        string pathComponent = pathComponents[i];
        
        // 对 key 进行排序，这样可以把 ~ 放到最后
        auto subRoutesKeys = subRoutes->allChildKeys();
        sort(subRoutesKeys->begin(), subRoutesKeys->end(), Comp);
        
 
        for (int i = 0 ; i < subRoutesKeys->size(); i++){
            string key = subRoutesKeys->at(i);
            if (key.compare(pathComponent) == 0 || key.compare(X_ROUTER_WILDCARD_CHARACTER)){
                found = true;
                subRoutes = subRoutes->findRouter(key);
            } else if (hasPrefix(key,":")) {
                found = true;
                subRoutes = subRoutes->findRouter(key);
                string newKey =  key.substr(1);
                string newPathComponent = pathComponent;
                // 再做一下特殊处理，比如 :id.html -> :id
                if (XRouter::checkIfContainsSpecialCharacter(key)) {
//                    NSCharacterSet *specialCharacterSet = [NSCharacterSet characterSetWithCharactersInString:specialCharacters];
//                    NSRange range = [key rangeOfCharacterFromSet:specialCharacterSet];
//                    if (range.location != NSNotFound) {
//                        // 把 pathComponent 后面的部分也去掉
//                        newKey = [newKey substringToIndex:range.location - 1];
//                        NSString *suffixToStrip = [key substringFromIndex:range.location];
//                        newPathComponent = [newPathComponent stringByReplacingOccurrencesOfString:suffixToStrip withString:@""];
//                    }
                }
                
                ParamBox *box = new ParamBox();
                box->simpleValue = newPathComponent;
                box->type = 0;
                parameters->insert(make_pair(XRouterParameterURL, box));

                parameters->insert(make_pair(newKey, box));
                break;
            }
        }
     
        
        if (found && subRoutes->indexOf("_")!= RouterTree::npos()){
            recentestCompareBlock = subRoutes->findRouter("_")->routerHandler;
        }
        
        // 如果没有找到该 pathComponent 对应的 handler，则以上一层的 handler 作为 fallback
        if (!found && subRoutes->indexOf("_") == RouterTree::npos()) {
            return NULL;
        }
    }
    
    // Extract Params From Query.
    auto pathInfo = split(url, "?");
    if (pathInfo.size() > 1) {
        string parametersString = pathInfo.at(1);
        auto paramStringArr = split(parametersString, "&");
        for (int i = 0 ; i < paramStringArr.size(); i++){
            auto paramString = paramStringArr[i];
            auto paramArr = split(paramString, "=");
            if (paramArr.size() > 1) {
                string key = paramArr[0];
                string value = paramArr[1];
                
                ParamBox *box = new ParamBox();
                box->simpleValue = value;
                box->type = 0;
                parameters->insert(make_pair(key,box));
            }
        }
    }
    
    if (recentestCompareBlock) {
        ParamBox *box = new ParamBox();
        box->handler = recentestCompareBlock;
        box->type = 1;
        parameters->insert(make_pair("block", box));
        
    }
    
    return parameters;
}

bool XRouter::checkIfContainsSpecialCharacter(string key){
    return false;
}
/**
 * 查找谁对某个 URL 感兴趣，如果有的话，返回一个 object
 *
 *  @param URL
 */

void* XRouter::objectForURL(string URL){
    return NULL;
}
/**
 * 查找谁对某个 URL 感兴趣，如果有的话，返回一个 object
 *
 *  @param URL
 *  @param userInfo
 */
void* XRouter::objectForURL(string URL,ParamsMap userInfo){
    return NULL;
}

/**
 *  是否可以打开URL
 *
 *  @param URL
 *
 *  @return
 */

bool XRouter::canOpenURL(string URL){
    return false;
}

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

string XRouter::generateURLWithPattern(string pattern,vector<void *> parameters){
    return NULL;
}

void XRouter::addObjectURLPattern(string URLPattern, XRouterObjectHandler handler){
//    RouterMap *subRoutes = this->addURLPattern(URLPattern);
//    if (handler && subRoutes){
//        subRoutes->insert(make_pair("_", &handler));
//    }
}

void XRouter::addURLPattern(string URLPattern, XRouterHandler handler){
    RouterTree *subRoutes = this->addURLPattern(URLPattern);
    if (handler && subRoutes){
       RouterTree *newRouter = subRoutes->insert("_");
       newRouter->routerHandler = handler;
    }
}

RouterTree* XRouter::addURLPattern(string URLPattern){
    
    auto pathComponents = this->pathComponentsFromURL(URLPattern);
    auto index = 0;
    RouterTree* subRoutes = this->getHeaderRouter();
    
    while (index < pathComponents.size()) {
        string pathComponent = pathComponents[index];
        
        bool hasKey = subRoutes->indexOf(pathComponent) != RouterTree::npos();
        if (!hasKey) {
            subRoutes->insert(pathComponent);
        }
        subRoutes = subRoutes->findRouter(pathComponent);
        index++;
    }
    return subRoutes;
}

vector<string> XRouter::pathComponentsFromURL(string URL){
    vector<string> pathComponents;
    if (URL.find("://") != string::npos) {
        auto pathSegments = split(URL,"://");//strtok(URL, "://");
       
        // 如果 URL 包含协议，那么把协议作为第一个元素放进去
        pathComponents.push_back(pathComponents[0]);
        // 如果只有协议，那么放一个占位符
        if ((pathSegments.size() == 2 && (pathSegments[1]).length() > 0) || pathSegments.size() < 2) {

            pathComponents.push_back(X_ROUTER_WILDCARD_CHARACTER);
        }
        
        URL = URL.substr(URL.find("://")+3);
    }
    
    //去掉url ？ 后面的值
    auto pos = URL.find("?");
    if (pos != string::npos){
        URL = URL.substr(0,pos);
    }
    
    auto urlPathComponents = split(URL, "/");
    
    for (int i = 0 ; i < urlPathComponents.size(); i++){
        auto pathComponent = urlPathComponents[i];
        pathComponents.push_back(pathComponent);
    }
    return pathComponents;
}
