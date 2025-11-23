#include <iostream>
#include <stack>

using namespace std;

const int plen_target = 1 << 11;
const int plen_bound = 1 << 12;

stack<char> st;

int main()
{
    freopen("07.in", "w", stdout);
    for(int i = 1; i <= (1 << 11) - 2; i++)
    {
        cout << char((i - 1) % 5 + 'A');
    }
    cout << "FA";
    // cout << endl;
    for(int i = 1; i <= 682; i++)
    {
        cout << char(i % 5 + 'A');
        st.push(char(i % 5 + 'A'));
        // BCDDCCBB
    }
    cout << "FF";
    while(!st.empty())
    {
        char c = st.top();
        cout << c << c;
        st.pop();
    }
    // cout << endl;
    for(int i = 1; i <= (1 << 11); i++)
    {
        cout << char((i - 1) % 5 + 'A');
    }
    cout << endl << "2" << endl;
    cout << (1 << 11) + 682 << " F" << endl;
    cout << (1 << 11) << " A" << endl;
}