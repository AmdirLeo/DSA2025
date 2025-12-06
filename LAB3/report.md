## 0.准备工作 
1. hashing_strategy 和 collision_strategy作为抽象基类，其指针可以支持动态绑定，根据派生类实际类型决定调用函数。  
同时，如果不使用指针，由于基类不能存储派生类的特性，在运行时（例如`hashtable(int size, hashing_strategy* hashing, collision_strategy* collision)`无法将派生类对象赋给基类成员，即无法实现基于多态的多种策略。  
2. 插入：找到新位置  
查询：查询链  
3. 初始化步长之类的  
4. 覆盖  
5. 无限循环  
6. 无限循环  
7. 减少冲突，增加空间利用率  
## 2.实现哈希策略  
1. 坏哈希  
    str为字符串，l为字符串长度，N为哈希表长度
    $$ \text{hash}(str) = \left(\sum\limits_{i = 0}^{l - 1}(str[i] \times \frac{l!}{(i+1)!})\right) \bmod{N} $$
    如此可以使得字符串下标较小的字符权重极大，导致不均匀的哈希值分布。  
2. 好哈希  
    $$ \text{hash}(str) = \left(\sum\limits_{i = 0}^{l - 1}(str[i] \times b^{l - i})\right) \bmod{N} $$
    使用多项式系数，使得分布较均匀同时对于字符变化敏感度较高  
3. 双向平方试探  
```cpp  
struct quad_probe: public collision_strategy{
    int dir;//0left 1right
    int r;//radius
    void init();
    int operator()(hash_entry* Table, int table_size, int last_choice) override;
};  
```  
dir代表当前是向左还是向右，r代表试探半径  
```cpp
int quad_probe::operator()(hash_entry* Table, int table_size, int last_choice){
    if(!dir){//向左
        dir^1;
        return (last_choice-((2*r*r)%table_size)+table_size)%table_size;
    }
    else{//向右
        r++;
        dir^1;
        return (((last_choice+((r*r-2*r+1)%table_size))%table_size)+((r*r)%table_size))%table_size;
    }
    
}
void quad_probe::init(){
    dir=1;r=0;//初始向右
}  
```  
如果dir为0，向左，则半径不变，位置减去2r^2，如果dir为1，向右，则先对当前位置加上r^2,再加上(r+1)^2.  

4. 公共溢出区  
```cpp  
int overflow::operator()(hash_entry* Table, int table_size, int last_choice){
    return p++;  
}

void overflow::init(){
    p=sz;
}
```
在overflow结构体中我设置了新的哈希表大小400031，是一个除四余三的质数并且留了100000的公共溢出区。  
初始化时直接将公共溢出区的首个位置指向哈希表的末尾。冲突时线性增加公共溢出区末尾的下标。  

## 改动：
1. 对于`hashtable.h`中的`hashtable::insert(hash_entry entry)`函数,加入dynamic_cast判断是否为公共溢出区策略，如果是则在插入时将哈希表大小改为静态变量sz。  
```cpp  
if(dynamic_cast<overflow*>(my_collision)){
          table_size=overflow::sz;
}  
```
2. 对于main函数，使用命令行参数指定策略。  
```cpp  
    hashing_strategy* hashing;
    collision_strategy* collision;
    int hashing_type=atoi(argv[1]),collision_strategy_type=atoi(argv[2]);
    if(hashing_type==1) hashing=new bad_hashing;
    if(hashing_type==2) hashing=new good_hashing;
    if(collision_strategy_type==1) collision=new linear_probe;
    if(collision_strategy_type==2) collision=new quad_probe;
    if(collision_strategy_type==1) collision=new overflow;

    hashtable table(TABLE_SIZE,hashing, collision);  
```  
argv[1]是哈希函数选择，argv[2]是冲突策略选择。