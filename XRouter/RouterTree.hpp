//
//  RouterTree.hpp
//  XRouter
//
//  Created by js on 2017/4/19.
//  Copyright © 2017年 js. All rights reserved.
//

#ifndef RouterTree_hpp
#define RouterTree_hpp

#include <stdio.h>
#include <string>
#include <functional>
#include <map>
#include <vector>
using namespace std;

typedef function<void(void *)> RouterHandler;

typedef struct RouterTree{
    vector<RouterTree *>childs;
    
    string pathComponent;
    
    RouterHandler routerHandler;
    
    //按照路由路径 插入一个节点
    RouterTree* insert(string pathComponent);
    
    //只在childs 中查找
    RouterTree* findRouter(string pathComponent);
    
    int indexOf(string pathComponent);
    
    //no pos 没找到
    static int npos();
    
    vector<string>* allChildKeys();
    
}RouterTree;
#endif /* RouterTree_hpp */
