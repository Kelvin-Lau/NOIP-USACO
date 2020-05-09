#include <cstdio>
#include <cstring>
#include <algorithm>
const int N = 1e5 + 7;
typedef long long LL;
int prm[N>>2], p[N], ct, pct;
bool noP[N];
void eulerSieve(int n) {
    noP[0] = noP[1] = true;
    for (int i = 2; i <= n; ++i) {
        if (noP[i] == false) prm[++*prm] = i;
        for (int j = 1; j<=*prm && prm[j]*i<=n; ++j) {
            noP[i*prm[j]] = true;
            if (i%prm[j] == 0) break;
        }
    }
}
bool isPrime(int x) {
    if (x < 2) return false;
    if (x < 1e5) return not noP[x];
    for (int i = 1; i<=pct && 1LL*prm[i]*prm[i]<=x; ++i)
        if (x%prm[i] == 0) return false;
    return true;
}
void dfs(int mul, int pos, int r) {
    if (r == 1) { p[++ct] = mul; return; }
    if (r-1>prm[pos] && isPrime(r-1)) p[++ct] = mul * (r-1);
    for (int i = pos+1; i<=pct && 1LL*prm[i]*prm[i]<=r; ++i) {
        for (LL j=prm[i], t=j+1; t <= r; j*=prm[i], t+=j)
            if (r%t == 0) dfs(mul*j, i, r/t);
    }
}
int main() {
    int n;
    eulerSieve(N - 1);
    pct = *prm, *prm = 0;
    while (~scanf("%d", &n)) {
        memset(p, 0, sizeof p), ct = 0;
        dfs(1, 0, n);
        std::sort(p+1, p+ct+1);
        printf("%d\n", ct);
        for (int i = 1; i <= ct; ++i)
            printf("%d%c", p[i], i<ct?' ':'\n');
    }
    return 0;
}
