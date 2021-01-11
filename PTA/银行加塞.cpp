#include <cstdio>
#include <cstring>
#include <algorithm>
const int N = 1e4 + 7, M = 1.8e5 + 7;
//用map将名字字符串散列成下标
int rt[M];
inline int map(char a[]) { return (a[0]-'A') + (a[1]-'A')*26 + (a[2]-'A')*26*26; }
struct People {
    char name[5];
    //t为到达时间，p为业务时间，w为该客户的等待时间
    int t, p, w;
    bool done;//若done为true，表明该客户已经办理业务
} vec[N], pt;
//按照办理顺序存储顾客的名称
char que[N][5];
//并查集：查询两个顾客是否在同一个朋友圈
int getRoot(int a) {
    return rt[a] == a ? a : (rt[a] = getRoot(rt[a]));
}
//将在互为朋友的顾客合并到一个朋友圈内
void merge(int a, int b) {
    int ra = getRoot(a), rb = getRoot(b);
    if (ra == rb) return;
    rt[rb] = ra;
}
int main() {
    //输入输出操作提速
    int n, m;
    scanf("%d%d", &n, &m);
    while (m--) {
        int i, n;
        char name[5], last[5];
        scanf("%d", &n);
        //一次构建朋友圈
        for (i = 1; i <= n; ++i) {
            scanf("%s", name);
            int t = map(name);
            rt[t] = t;
            if (i == 1) strcpy(last, name);
            else merge(t, map(last));
        }
    }
    //依次输入顾客信息
    for (int i = 1; i <= n; ++i) {
        scanf("%s%d%d", vec[i].name, &vec[i].t, &vec[i].p);
        if (vec[i].p > 60) vec[i].p = 60;
        vec[i].w = 0, vec[i].done = false;
    }
    //wait为当前办理业务的顾客的完成时间
    int wait = vec[1].t, sum = 0, c = 0;
    //从前到后依次处理顾客请求
    for (int i = 1; i <= n; ++i) {
        //如果该客户已经办理过，就跳过
        if (vec[i].done) continue;
        //将该客户加入到办理顺序的数组中
        strcpy(que[++c], vec[i].name);
        //如果某顾客来的时候窗口空闲，则wait调整为该顾客到达的时间+他的业务时间
        if (vec[i].t > wait) wait = vec[i].p + vec[i].t;
        //否则，该顾客的等待时间应为当前业务的完成时间-该顾客到达时间，同时更新wait
        else vec[i].w = wait - vec[i].t, wait += vec[i].p;
        //查看能够加塞的顾客
        for (int j = i+1; j <= n; ++j) {
            //如果当前顾客的到达时间晚于顾客i业务完成时间，则无法加塞
            if (vec[j].t > wait) break;
            //如果时间来得及，则与顾客i为朋友的顾客都可以加塞
            if (getRoot(map(vec[j].name)) == getRoot(map(vec[i].name))) {
                strcpy(que[++c], vec[j].name);
                vec[j].w = wait - vec[j].t, wait += vec[j].p;
                vec[i].p += vec[j].p, vec[j].done = true;
            }
        }
    }
    //将所有顾客的等待时间求和
    for (int i = 1; i <= n; ++i) sum += vec[i].w;
    //依次打出顾客姓名
    for (int i = 1; i <= n; ++i) puts(que[i]);
    printf("%.1f", 1.*sum/n);
    return 0;
}