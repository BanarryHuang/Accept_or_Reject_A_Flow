#include <fstream>
#include <float.h>
#include <string.h>
#define N 300
using namespace std;

struct inf_of_edge{
    int order;
    int n1;
    int n2;
    int weight;
    double load;/**for edge**/
    double cost;/**for edge**/
};
typedef inf_of_edge e;

e line[N],flow[N];
double cost[N][N];
double dist[N];
int last[N];

/**Find the smallest cost from v to another node**/
int choose(double dis[], int n, bool found[])
{
    int i, minpos;
    double mini;
    mini = DBL_MAX;
    minpos = -1;
    for (i = 0; i < n; i++)
        if (dis[i] < mini && !found[i]){
            mini = dis[i];
            minpos = i;
        }
    return minpos;
}

/**Find the shortest path from v to j**/
int shortestPath(int v, int j, e x[], int n, bool found[])
{
    int i, u, w;

    for (i=0;i<n;i++) {
        found[i] = 0;
        dist[i] = cost[v][i];
        last[i]=v;
    }
    found[v] = 1;
    last[v] = -1;
    dist[v] = 0;
    for (i=0;i<n-1;i++) {
        u = choose(dist, n, found);
        found[u] = 1;
        if(u==j)
            return j;
        for (w=0; w<n; w++)
            if (!found[w])
                if (dist[u]+cost[u][w]<dist[w]){
                    dist[w] = dist[u]+cost[u][w];
                    last[w] = u;
                }
    }
    if(u==j)
        return j;
    else
        return -1;
}

int main()
{
    ifstream inStream;
    ofstream outStream;
    inStream.open("request.txt");
    outStream.open("re.txt");
    int i,j,k,node,edge,task,rout,apt,a,b;
    int StartPos[N],rowterm[N];/**用於紀錄edge第一點起始位置**/
    int path[N],d=0,tr[N],t=0;/**path:最短路徑所經過的點，tr:最短路徑所經過的路線**/
    int finish[N][N],fin=0,total=0;
    bool f[N];
    /**initialized**/
    for(i=0;i<N;i++){
        path[i]=0;
        tr[i]=0;
    }
    for(i=0;i<N;i++)
        for(j=0;j<N;j++)
            finish[i][j]=0;

    inStream >> node >> edge;

    for(i=0;i<node;i++)
        for(j=0;j<node;j++){
            if(i!=j)
                cost[i][j]=DBL_MAX;
            else
                cost[i][j]=0;
        }

    for(i=0;i<edge;i++){
        inStream >> line[i].order >> line[i].n1 >> line[i].n2 >> line[i].weight;
        line[i].cost=DBL_MIN;
        line[i].load=0;
        cost[line[i].n1][line[i].n2]=line[i].cost;
        cost[line[i].n2][line[i].n1]=line[i].cost;
    }
    /**Find the start position of line[] for every node**/
    for(i=0;i<node;i++)
        rowterm[i]=0;
    for(i=0;i<edge;i++)
        rowterm[line[i].n1]++;
    StartPos[0]=0;
    for(i=1;i<node;i++)
        StartPos[i]=StartPos[i-1]+rowterm[i-1];

    inStream >> task;

    for(i=0;i<task;i++){

        apt=1;

        inStream >> flow[i].order >> flow[i].n1 >> flow[i].n2 >> flow[i].weight;

        rout=shortestPath(flow[i].n1,flow[i].n2,line,node,f);/**尋找最短路徑**/

        while(rout!=-1){/**建立最短路徑節點**/
            path[d++]=rout;
            rout=last[rout];
        }

        for(j=d-1;j>0;j--){/**建立最短路徑edge**/
            a=path[j];
            b=path[j-1];
            if(b>a)
                swap(a,b);
            for(k=StartPos[b];k<edge;k++)
                if(line[k].n2==a)
                    break;
            tr[t++]=k;
            if(line[k].load+flow[i].weight>line[k].weight){
                apt=0;
                break;
            }
        }

        if(apt==1&&d>1){/**可接受路徑，更新其load, cost**/
            total+=flow[i].weight;

            for(j=0;j<t;j++){
                line[tr[j]].load+=flow[i].weight;
                if(line[tr[j]].weight-line[tr[j]].load!=0)
                    line[tr[j]].cost=line[tr[j]].load/(line[tr[j]].weight-line[tr[j]].load);
                else
                    line[tr[j]].cost=1e9;
                cost[line[tr[j]].n1][line[tr[j]].n2]=line[tr[j]].cost;
                cost[line[tr[j]].n2][line[tr[j]].n1]=line[tr[j]].cost;
            }

            k=0;
            finish[fin][k++]=i;
            for(j=d-1;j>-1;j--){
                finish[fin][k++]=path[j];
            }
            finish[fin++][k]=-1;
        }
        /**initialized for next task**/
        for(j=0;j<d;j++)
            path[j]=0;
        d=0;
        for(j=0;j<t;j++)
            tr[j]=0;
        t=0;
    }

    outStream << fin << ' ' << total << endl;

    for(i=0;i<fin;i++){
        j=0;
        while(finish[i][j]!=-1)
            outStream << finish[i][j++] << ' ';
        outStream << endl;
    }
    inStream.close();
    outStream.close();
    return 0;
}
