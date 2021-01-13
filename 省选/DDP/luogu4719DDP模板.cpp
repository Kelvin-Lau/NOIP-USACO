#include <cstdio>
#include <algorithm>
using std::max;
const int N = 1e5 + 7;
int n, m, v[N<<1], nxt[N<<1], cnt, hd[N], sz[N], son[N], wt[N];
inline void add(int u,int V) {
	v[++cnt] = V; nxt[cnt] = hd[u]; hd[u] = cnt;
}
int dfs1(int u) {//这里的树剖只需要一趟dfs求重儿子 
    sz[u] = 1; int mx = 0;
    for (int i = hd[u]; i; i = nxt[i])
        if(sz[v[i]] == 0) {
			sz[u] += dfs1(v[i]);
			if(mx < sz[v[i]]) mx = sz[v[i]], son[u] = v[i];
		}
    return sz[u];
}
struct Mat {//矩阵类 
    int mp[2][2];
    Mat() {
		mp[0][0] = mp[0][1] = mp[1][0] = mp[1][1] = -0x3f3f3f3f;
	}
    Mat(int x) {
		mp[0][0] = mp[1][1] = 0; mp[1][0] = mp[0][1] = -0x3f3f3f3f;
	}
    Mat operator *(const Mat &b) const {
        Mat c;
        //Floyd
		for (int k = 0; k < 2; ++k)
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
					c.mp[i][j] = max(c.mp[i][j], mp[i][k]+b.mp[k][j]);
        return c;
    }
    int gmx() {
		return max(max(mp[0][0],mp[0][1]), max(mp[1][0],mp[1][1]));
	}
    int* operator [](const int& x) { return mp[x]; }
};
struct Bst {
    int s[N][2], fa[N], st[N], tp, lsz[N], root;
	bool book[N];
    Mat mul[N], w[N];
	Bst() { w[0] = mul[0] = Mat(1); }
    void update(const int& x) {
		mul[x] = mul[s[x][0]] * w[x] * mul[s[x][1]];
	}
    void gtw(const int& x, const int& v) {
		w[x][1][0] += mul[v].gmx();
		w[x][0][0] = w[x][1][0];
		w[x][0][1] += max(mul[v][0][0], mul[v][1][0]);
		fa[v] = x;
	}
    void init() {
		for (int i = 1; i <= n; ++i)
			w[i][0][1] = wt[i], w[i][0][0] = w[i][1][0] = 0;
	}
    bool is_root(const int& p) {
		return (s[fa[p]][1]!=p) && (s[fa[p]][0]!=p);
	}
    int sbuild(const int& l, const int& r) {//对序列建Bst 
        if (l > r) return 0;
		int tot = 0;
		for (int i = l; i <= r; ++i) tot += lsz[st[i]];
        for (int i = l, ns = lsz[st[i]]; i <= r; ++i, ns+=lsz[st[i]])
            if(2*ns >= tot) {
                int rs = sbuild(l, i-1);
				int ls = sbuild(i+1, r);
				s[st[i]][0] = ls, s[st[i]][1] = rs;
                fa[ls] = st[i], fa[rs] = st[i], update(st[i]);
				return st[i];//找重心递归建树 
            }
    }
    int build(int p) {//链分治，每次处理一条链 
        for (int t = p; t; t = son[t]) book[t] = true;
        for (int t = p; t; t = son[t])
            for (int i = hd[t]; i; i = nxt[i])
				if (not book[v[i]]) gtw(t, build(v[i]));
        tp = 0;
		for (int t = p; t; t = son[t]) st[++tp] = t;
        for (int t = p; t; t = son[t]) lsz[t] = sz[t]-sz[son[t]];
		return sbuild(1, tp);
    }
    void modify(int p, int W) {//修改
        w[p][0][1] += W - wt[p], wt[p] = W;
        for (int t = p; t; t = fa[t])
            if(is_root(t) && fa[t]) {//如果是轻边 
                w[fa[t]][0][0] -= mul[t].gmx(), w[fa[t]][1][0] = w[fa[t]][0][0];
                w[fa[t]][0][1] -= max(mul[t][0][0], mul[t][1][0]); 
				update(t);
                w[fa[t]][0][0] += mul[t].gmx(), w[fa[t]][1][0] = w[fa[t]][0][0];
                w[fa[t]][0][1] += max(mul[t][0][0], mul[t][1][0]);
            }else update(t);
    }
}bst;
int main() {
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; ++i) scanf("%d", wt + i);
    for (int i = 1, u, v; i < n; ++i) {
		scanf("%d%d", &u, &v), add(u, v), add(v,u);
	}
    dfs1(1); bst.init(); bst.root = bst.build(1);
    for (int i = 1, p, w; i <= m; ++i) {
		scanf("%d%d", &p, &w);
		bst.modify(p, w);
		printf("%d\n", bst.mul[bst.root].gmx());
	}
    return 0;
}