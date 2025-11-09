#include<cstdio>
#include<string.h>
#include<cmath>
#include<iostream>
using namespace std;

const long long M=1000000007;
struct Poly{
    int deg;
    long long coef[65];
};
Poly add(Poly &A, Poly &B){
    Poly C;
    memset(C.coef,0,sizeof(C.coef));
    C.deg = max(A.deg, B.deg);
    for(int i=0;i<=C.deg;i++){
        C.coef[i]=(A.coef[i]+B.coef[i])%M;
    }
    return C;
}
Poly sub(Poly &A, Poly &B){
    Poly C;
    memset(C.coef,0,sizeof(C.coef));
    C.deg = max(A.deg, B.deg);
    for(int i=0;i<=C.deg;i++){
        C.coef[i]=(A.coef[i]-B.coef[i]+M)%M;
    }
    return C;
}
Poly mul(Poly &A, Poly &B){
    Poly C;
    memset(C.coef,0,sizeof(C.coef));
    C.deg = min(64, A.deg + B.deg);
    for(int i=0;i<=A.deg;i++){
        for(int j=0;j<=B.deg && i+j<=64;j++){
            C.coef[i+j]=(C.coef[i+j]+A.coef[i]*B.coef[j])%M;
        }
    }
    return C;
}
Poly pow(Poly A, int b){
    Poly C;
    memset(C.coef,0,sizeof(C.coef));
    C.deg=0;
    C.coef[0]=1;
    while(b){
        if(b&1) C=mul(C,A);
        A=mul(A,A);
        b>>=1;
    }
    return C;
} 


char s[1000005];
int p;

int parse_pow(int n){
    int b = 0;
    while(p<n && '0'<=s[p] && s[p]<='9'){
        b = b*10 + (s[p]-'0');
        p++;
    }
    if(p<n && s[p]=='^'){
        p++;
        int right = parse_pow(n);
        b = pow(b, right);
    }
    return b;
}

bool ifMul(char prev, char curr){//判断是否需要隐式乘法
    if(prev=='\0') return false;
    if(curr=='(') return true;
    if(curr=='x'&&prev!='x') return true;
    return false;
}   
 

Poly eparse();

Poly fparse(int n){
    Poly res;
    memset(res.coef, 0, sizeof(res.coef));
    if(s[p]=='x'){
        res.deg=1;
        res.coef[1]=1;
        p++;
    }
    else if('0'<=s[p]&&s[p]<='9'){
        long long val=0;
        while(p<n&&'0'<=s[p] && s[p]<='9'){
            val=val*10+s[p]-'0';
            val%=M;
            p++;
        }
        res.deg=0;
        res.coef[0]=val;
    }
    else if(s[p]=='('){
        p++;
        res=eparse();
        if(p<n&&s[p]==')')p++;
    }
    return res;
}

Poly tparse(int n){
    Poly res=fparse(n);
    while(p<n){
        
        if(s[p]=='*'){
            p++;
            Poly t=fparse(n);
            res=mul(res,t);
        }
        else if(s[p]=='^'){
            p++;
            int exp = parse_pow(n);
            res = pow(res, exp);
        }
        else if(s[p]=='x'){
            Poly t=fparse(n);
            if(s[p-1]==')'||isdigit(s[p-1])) res=mul(res,t);
            else res=t;
        }
        else if(s[p]=='('){
            Poly t=fparse(n);
            if(s[p-1]==')'||isdigit(s[p-1])||s[p-1]=='x') res=mul(res,t);
            else res=t;
        }
        else break;
    }
    return res;
}

Poly eparse(){
    int n=strlen(s);
    Poly res=tparse(n);
    while(p<n){
        if(s[p]=='+'){
            p++;
            Poly t=tparse(n);
            res=add(res,t);
        }
        else if(s[p]=='-'){
            p++;
            Poly t=tparse(n);
            res=sub(res,t);
        }
        else break;
    }
    return res;
}

int main(){
    scanf("%s",s);
    p=0;
    Poly res=eparse();
    while (res.deg > 0 && res.coef[res.deg] == 0) res.deg--;

    for (int i = res.deg; i >= 0; i--) {
        cout << res.coef[i];
        if (i > 0) cout << " ";
    }
    cout << "\n";
    return 0;
}
