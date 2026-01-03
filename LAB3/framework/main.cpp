#include <cstdio>
#include<iostream>
#include<bits/stdc++.h>
#include "hashtable.h"
using namespace std;

int main(int argc, char const* argv[]){
    int type;
    char buffer[1000];int data;
    hashing_strategy* hashing;
    collision_strategy* collision;
    int hashing_type=atoi(argv[1]),collision_strategy_type=atoi(argv[2]);
    if(hashing_type==1) hashing=new bad_hashing;
    if(hashing_type==2) hashing=new good_hashing;
    if(collision_strategy_type==1) collision=new linear_probe;
    if(collision_strategy_type==2) collision=new quad_probe;
    if(collision_strategy_type==3) collision=new overflow;

    hashtable table(TABLE_SIZE,hashing, collision);
    double t1=clock();

    while(true){
        scanf("%d", &type);
        if(type == 0){
            scanf("%s", buffer);scanf("%d",&data);
            table.insert(hash_entry(buffer, data));
        }else if(type == 1){
            scanf("%s",buffer);
            printf("%d\n", table.query(buffer));
        }else break;
    }
    double t2=clock();
    double res=(t2-t1)/CLOCKS_PER_SEC;
    ofstream f("result.txt",ios::app);

    f<<"hashing type: "<<argv[1]<<" collision strategy: "<<argv[2]<<endl;
    f<<"time: "<<res<<"s"<<endl;
    f.close();

    return 0;
}