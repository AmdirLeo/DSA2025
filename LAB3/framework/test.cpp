#include<iostream>
#include<cstring>
#include<bits/stdc++.h>

using namespace std;

int badhash(char* str, int N){
    int hash = 0;
    int length = strlen(str);
    for (int i = 0; i < length; i++) {
        hash = (hash * (i+1) + str[i]) % N;
    }
    return hash;
}

int isprime(int x){
    if(x<2) return 0;
    if(x%2==0) return 0;
    for(int i=3;i<sqrt(x);i+=2){
        if(x%i==0) return 0;
    }
    return 1;
}
int main(){
    ofstream f("p.txt");

    for (int num = 350772; num <= 500000; ++num) {
        if (isprime(num) && num % 4 == 3) {
            f << num << endl;
        }
    }
    f.close();
    return 0; 
}