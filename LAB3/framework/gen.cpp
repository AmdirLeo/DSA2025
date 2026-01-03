#include<bits/stdc++.h>
using namespace std;
const int N=500000;

struct data{
    int rank;
    string name;
}dataset[N+5];
int main(int argc, char const* argv[]){
    srand(time(0));
    int insertcount=atoi(argv[1]);
    int querycount=atoi(argv[2]);
    string filename="test.in";//生成名为test.in的文件
    freopen("poj.txt","r",stdin);
    freopen("test.in","w",stdout);

    int x;
    for(int i=0;i<N;i++){
        cin>>x>>dataset[i].name>>dataset[i].rank>>x;
    }
    shuffle(dataset,dataset+N,default_random_engine(rand()));
    for(int i=0;i<insertcount;i++){
        cout<<0<<" "<<dataset[i].name<<" "<<dataset[i].rank<<endl;
    }
    shuffle(dataset,dataset+N,default_random_engine(rand()));
    for(int i=0;i<querycount;i++){
        cout<<1<<" "<<dataset[i].name<<endl;
    }
    cout<<2<<endl;
    return 0;
}