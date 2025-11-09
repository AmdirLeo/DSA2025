#include<cstdio>
#include<string.h>
#include<cmath>
#include<iostream>
#include<cassert>
using namespace std;

const int M=1000000007;
struct Poly{
    int deg;
    int coef[65];

    void adjust_deg(){
        while(deg>0 && coef[deg]==0) deg--;
    }
};

Poly add(Poly &A, Poly &B){
    Poly C;
    memset(C.coef,0,sizeof(C.coef));
    C.deg = max(A.deg, B.deg);
    for(int i=0;i<=C.deg;i++){
        C.coef[i]=(A.coef[i]+B.coef[i])%M;
    }
    C.adjust_deg();
    return C;
}
Poly sub(Poly &A, Poly &B){
    Poly C;
    memset(C.coef,0,sizeof(C.coef));
    C.deg = max(A.deg, B.deg);
    for(int i=0;i<=C.deg;i++){
        C.coef[i]=(A.coef[i]-B.coef[i]+M)%M;
    }
    C.adjust_deg();
    return C;
}
Poly mul(Poly &A, Poly &B){
    Poly C;
    memset(C.coef,0,sizeof(C.coef));
    C.deg = min(64, A.deg + B.deg);
    for(int i=0;i<=A.deg;i++){
        for(int j=0;j<=B.deg && i+j<=64;j++){
            C.coef[i+j]=(C.coef[i+j]+1ll*A.coef[i]*B.coef[j])%M;
        }
    }
    C.adjust_deg();
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
Poly *numStack;
char opStack[1000005];
int numtop=-1;
int optop=-1;


int prec(char op){//比较优先级
    if(op=='+'||op=='-') return 1;
    if(op=='*') return 2;
    if(op=='^') return 3;
    return 0;
}
bool ifMul(char prev, char curr){//判断是否需要隐式乘法
    if(prev=='\0') return false;
    if(curr=='('){
        if(prev==')'||prev=='x'||isdigit(prev)){
            return true;
        }
    }
    if(curr=='x'&& (prev==')'||isdigit(prev))){
        return true;
    } 
    return false;
}   

void applyNextOp(){
    char op=opStack[optop--];
    Poly b=numStack[numtop--];
    Poly a=numStack[numtop--];
    Poly c;
    if(op=='+') c=add(a,b);
    else if(op=='-') c=sub(a,b);
    else if(op=='*') c=mul(a,b);
    else if(op=='^') c=pow(a,(int)b.coef[0]);
    numStack[++numtop]=c;
}

char s[1000005];

int main(){
    scanf("%s",s);
    int n=strlen(s);
    int cnt=0;
    for(int i=0;i<n;i++){
        if(s[i]=='('){
            cnt++;
        }
    }
    numStack=new Poly[cnt+5];
    for(int i=0;i<n;){
        char c=s[i];
        if('0'<=c && c<='9'){//数字
            long long val=0;
            while(i<n && '0'<=s[i] && s[i]<='9'){
                val=val*10+(s[i]-'0');
                val%=M;
                i++;
            }
            Poly num;
            memset(num.coef,0,sizeof(num.coef));
            num.deg=0;
            num.coef[0]=val%M;
            numStack[++numtop]=num;
            if(i<n && ifMul(s[i-1],s[i])){
                i--;
                s[i]='*';
            }
        }
        else if(c=='x'){//变量
            Poly num;
            memset(num.coef,0,sizeof(num.coef));
            num.deg=1;
            num.coef[1]=1;
            numStack[++numtop]=num;
            i++;
            if(i<n && ifMul('x',s[i])){
                i--;
                s[i]='*';
            }
        }
        else{//操作符或括号
            if(c=='('){
                opStack[++optop]=c; 
                i++;
            }
            else if(c==')'){
                while(optop>=0 && opStack[optop]!='('){
                    applyNextOp();
                }
                optop--;//弹出左括号
                i++;
                if(i<n && ifMul(')',s[i])){
                    i--;
                    s[i]='*';
                }
            }
            else{//运算符
                while(optop>=0 && opStack[optop] != '('&& prec(opStack[optop])>=prec(c)){
                    applyNextOp();
                }
                opStack[++optop]=c;
                i++;
            }
        }
    }
    while(optop>=0 && numtop >= 1){
        applyNextOp();
    }
    assert(optop == -1 && numtop == 0);
    Poly &result=numStack[numtop];
    result.adjust_deg();
    if(result.deg==0&&result.coef[0]==0){
        cout<<"0\n";
        return 0;
    }
    for(int i=result.deg;i>=0;i--){
        cout<<result.coef[i];
        if(i>0) cout<<" ";
    }
    cout<<"\n";
    delete[] numStack;
    return 0;
}