#include <cstdio>
#include<iostream>
#include "hashtable.h"

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
    if(collision_strategy_type==1) collision=new overflow;

    hashtable table(TABLE_SIZE,hashing, collision);
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
    return 0;
}