//
//  Util.h
//  XRouter
//
//  Created by GSDN on 2017/4/19.
//  Copyright © 2017年 js. All rights reserved.
//

#ifndef Util_h
#define Util_h
#include <string>
#include <vector>
#include <map>
using namespace std;

vector<string> split(string str,string pattern)
{
    string::size_type pos;
    vector<string> result;
    str+=pattern;//扩展字符串以方便操作
    string::size_type size=str.size();
    
    for(string::size_type i=0; i<size; i++)
    {
        pos=str.find(pattern,i);
        if(pos<size)
        {
            string s=str.substr(i,pos-i);
            if (s.length() > 0){
                result.push_back(s);
            }
           
            i=pos+pattern.size()-1;
        }
    }
    return result;
}

vector<string> getMapAllkeys(map<string,void*> map){
    vector<string> vector;
    auto iterator = map.begin();
    while(iterator != map.end())
    {
        vector.push_back(iterator->first);
        iterator++;
    }
    return vector;
}


bool hasPrefix(string str, string prefix){
    auto pos = str.find(prefix);
    if (pos != string::npos && pos == 0){
        return true;
    }
    return false;
}

#endif /* Util_h */
