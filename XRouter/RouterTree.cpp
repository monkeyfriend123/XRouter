//
//  RouterTree.cpp
//  XRouter
//
//  Created by js on 2017/4/19.
//  Copyright © 2017年 js. All rights reserved.
//

#include "RouterTree.hpp"

RouterTree::RouterTree(){
    childs = new vector<RouterTree *>();
}


RouterTree* RouterTree::insert(string pathComponent){
    RouterTree *router = new RouterTree();
    router->pathComponent = pathComponent;
    this->childs->push_back(router);
    return router;
}

RouterTree* RouterTree::findRouter(string pathComponent){
    auto size = this->childs->size();
    auto index = this->indexOf(pathComponent);
    
    if (index != RouterTree::npos() && index < size){
        return this->childs->at(index);
    }
    return NULL;
}

int RouterTree::indexOf(string pathComponent){
    auto size = this->childs->size();
    for (int i = 0; i < size; i++){
        RouterTree *one = this->childs->at(i);
        if (one->pathComponent.compare(pathComponent) == 0){
            return i;
        }
    }
    return RouterTree::npos();
}

int RouterTree::npos(){
    return -1;
}

vector<string>* RouterTree::allChildKeys(){
    vector<string> *keys = new vector<string>();
    auto size = this->childs->size();
    for (int i = 0; i < size; i++){
        RouterTree *one = this->childs->at(i);
        keys->push_back(one->pathComponent);
    }
    return keys;
}
