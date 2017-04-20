//
//  ViewController.m
//  XRouter
//
//  Created by GSDN on 2017/4/19.
//  Copyright © 2017年 js. All rights reserved.
//

#import "ViewController.h"
#import "XRouter.hpp"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)btnClicked:(id)sender{
    
    auto userInfo = new ParamsMap();
    ParamBox *box = new ParamBox();
    box->simpleValue = "122";
    box->type = 0;
    userInfo->insert(make_pair("test", box));
    
    
    XRouter::openURL("hello/world?name=zhangsan",userInfo);
    XRouter::openURL("hello/123?name=zhangsan",userInfo);
    XRouter::openURL("hello/cpp?name=zhangsan",userInfo);

}


@end
