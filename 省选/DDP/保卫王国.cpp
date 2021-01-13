#include<iostream>
#include<algorithm>
#include<set>
#include<vector>
#define Iter set<node>::iterator
using namespace std;

void prepare() {
    ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
}

const int maxn = 1e7+5;
int n, m;
int a[maxn];

struct node {
    int l,r;
    mutable int v;
    node(int L,int R,int V):l(L),r(R),v(V){}
    node(int L):l(L){}
    bool operator<(const node &o)const{
        return l<o.l;
    }
};
set<node> s;

Iter split (int pos) {
    Iter it = s.lower_bound(node(pos));
    if (it!=s.end() && it->l==pos) return it;
    it--;
    int l = it->l;
    int r = it->r;
    int v = it->v;
    s.erase(it);
    s.insert(node(l,pos-1,v));
    return s.insert(node(pos,r,v)).first;
}

void assign (int l, int r, int v) {
    Iter right = split(r+1);
    Iter left = split(l);
    s.erase(left,right);
    s.insert(node(l,r,v));
}

int ask (int l, int r, int v) {
    Iter right = split(r+1);
    Iter left = split(l);
    int ans = 0;
    for (Iter it=left;it!=right;it++) {
        if (it->v==v) ans += it->r-it->l+1;
    }
    return ans;
}

int main () {
    prepare();
    cin>>n;
    for (int i=1;i<=n;i++) {
        int t;
        cin>>t;
        s.insert(node(i,i,t));
    }
    for (int i=1;i<=n;i++) {
        int opt,l,r,v;
        cin>>l>>r>>v;
        cout<<ask(l,r,v)<<endl;
        assign(l,r,v);
	}
    return 0;
}