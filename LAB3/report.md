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
    $$ \text{hash}(str) = \left(\sum\limits_{i = 0}^{l - 1}str[i] \right) \bmod{N} $$
    参考了课件中给出的坏哈希函数例子，此函数完全消除了字符间顺序的信息，使得冲突可能性较大。 
2. 好哈希  
    $$ \text{hash}(str) = \left(\sum\limits_{i = 0}^{l - 1}(str[i] \times 37^{l - i})\right) \bmod{N} $$
    使用多项式系数，使得分布较均匀同时对于字符变化敏感度较高。  
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
        dir=dir^1;
        return (last_choice-((2*r*r)%table_size)+table_size)%table_size;
    }
    else{//向右
        r++;
        dir=dir^1;
        return (((last_choice+((r*r-2*r+1)%table_size))%table_size)+((r*r)%table_size))%table_size;
    }
    
}
void quad_probe::init(){
    dir=1;r=0;//初始向右
}  
```  
初始向右探测，半径归零。  
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
在overflow结构体中我设置了新的哈希表大小400031，是一个除四余三的质数并且预留了100000的公共溢出区。  
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

3. 对于main函数，添加t1,t2=clock(),用于记录时间，并在测试结束后将结果输入result.txt文件内。  
```cpp  
    double res=(t2-t1)/CLOCKS_PER_SEC;
    ofstream f("result.txt",ios::app);

    f<<"hashing type: "<<argv[1]<<" collision strategy: "<<argv[2]<<endl;
    f<<"time: "<<res<<"s"<<endl;
    f.close();
```

## 3 测试
我使用gen.cpp构造了三个测例。其实现方式是使用shuffle将poj中数据洗混之后再读取前n个作为插入，再一次洗混之后读取前m个作为查询。命令行参数有两个，第一个是插入次数n，第二个是查询次数m。  
生成命令为:  
```bash
./gen n m  
```   
此部分实现参考了https://github.com/tfia/THU-CST-DSA-24fall/blob/main/lab3/hashfun/gen.cpp。  
对于测例大小，我设想的是三个测例依次递增，最大测例大小不超过公共溢出区大小（约十万）。
对于测试结果以及测试环境在result.txt中有说明。  

## 4 结果分析  
1. 数据规模较小时无显著差异，规模较大时（test2、3）好哈希显著优于坏哈希，这是因为好哈希数据分布较均匀，冲突概率远低于坏哈希，因此效率较高。  
2. 使用好哈希时无明显差异，使用坏哈希时双向平方试探显著优于线性试探。这是因为平方试探间隔迅速增长，可以有效避免冲突数据聚集，而冲突较少时（好哈希）两者性能接近。  
3. 封闭散列更占优势。公共溢出区策略可能在冲突较多而规模不大的数据集上占优势，因为它不需要过多试探。
4. 可能导致冲突概率增加，哈希表性能下降。
5. 可以检测负载因子，当达到某一阈值（例如50%）时建立长度为原来两倍的一个空表，再将所有当前表中数据逐个插入。当达到另一阈值（例如10%）时建立长度为原来一半的一个空表，再将所有当前表中数据逐个插入。
