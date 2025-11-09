
#include<bits/stdc++.h>

using namespace std;

int main(){
    ofstream outfile("07.in");
    for(int i = 0; i < 157; ++i){
        outfile << "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    /*outfile<<"JJIIHHGGFFEEDDCCBBAA";
    string s="AABCCBAA";
    for(int i=0;i<255;++i){
        s="CCBB"+s+"BBCC";
    }
    outfile<<s;
    for(int i = 0; i < 79; ++i){
        outfile << "AABBCCDDEEFFGGHHIIJJKKLLMMNNOOPPQQRRSSTTUUVVWWXXYYZZ";
    }
    outfile<<"\n";
    outfile<<"2\n";
    outfile<<"3072 C\n";
    outfile<<"3071 B\n";*/

    for(int i = 0; i < 500; ++i){
        outfile << "\n0 A";
        outfile << "\n0 B";
    }       


    outfile.close();
    return 0;
}