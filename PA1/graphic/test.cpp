#include<iostream>
using namespace std;    

int main(){
    int a,b,c,d;
    cin>>a>>b>>c>>d;    
    if(a==b||b==c||c==d||d==a||a==c||b==d){
        cout<<"Yes"<<endl;
    }
}