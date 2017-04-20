# XRouter
C++ 版本路由管理



XRouter::registerURLPattern("hello/cpp/",[](void * params)->void{
        ParamsMap *paramsMap = (ParamsMap *)params;
        auto iterator = paramsMap->begin();
        while(iterator != paramsMap->end()){
            printf("%s , %s \n",iterator->first.c_str(),iterator->second->simpleValue.c_str());
            iterator++;
        }
        printf("hello cpp\n");
        
    });
    
    XRouter::registerURLPattern("hello/",[](void * params)->void{
        ParamsMap *paramsMap = (ParamsMap *)params;
        auto iterator = paramsMap->begin();
        while(iterator != paramsMap->end()){
            printf("%s , %s \n",iterator->first.c_str(),iterator->second->simpleValue.c_str());
            iterator++;
        }
        printf("hello world\n");
       
    });
    
    XRouter::openURL("hello/world?name=zhangsan");
    XRouter::openURL("hello/123?name=zhangsan");
    XRouter::openURL("hello/cpp?name=zhangsan");
