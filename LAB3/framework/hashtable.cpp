#include "hashtable.h"
int naive_hashing::operator()(char* str, int N){
    if(str == NULL) return 0;
    else return (str[0]+N)%N;
}

int bad_hashing::operator()(char *str, int N)
{
    if (str == NULL)
        return 0;
    int hash = 0;
    int length = strlen(str);
    for (int i = 0; i < length; i++) {
        hash = (hash * (i+1) + str[i]) % N;
    }
}

int good_hashing::operator()(char* str, int N){
    if(str==NULL) return 0;
    int hash = 0;
    int length = strlen(str);
    for(int i = 0; i < length; i++){
        hash = (hash * 37 + str[i]) % N;
    }
}

int linear_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    return (last_choice + 1) % table_size;
}
void linear_probe::init(){
    return;// do nothing
}

int quad_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    if(!dir){//向左
        dir^1;
        return (last_choice-((2*r*r)%table_size)+table_size)%table_size;
    }
    else{//向右
        dir^1;
        return (((last_choice+((r*r-2*r+1)%table_size))%table_size)+((r*r)%table_size))%table_size;
    }
    
}

void quad_probe::init(){
    dir=1;r=1;//初始向右
}

int overflow::operator()(hash_entry* Table, int table_size, int last_choice){
    return p++;  
}

void overflow::init(){
    p=sz;
}