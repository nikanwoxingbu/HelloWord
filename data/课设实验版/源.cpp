#include<iostream>
#include<fstream>
#include<string>
using namespace std;
#define MAX_VEX 100            // 图中含有顶点的最多个数
#define INF 65535              //如果两个顶点之间不可达，用无穷表示距离
#define OK 1
#define ERROR 0
typedef char VertexType;
typedef int EdgeType;
#define  InfoType int//图中弧或者边包含的信息的类型

typedef struct {
    VertexType Vex[MAX_VEX];    //图中的顶点 
    EdgeType Edge[MAX_VEX][MAX_VEX];  //邻接矩阵 
    int vexnum, arcnum;   //图中的顶点数和边的数量 
}MGraph;

typedef struct ArcNode {
    int adjvex;//邻接点在数组中的位置下标
    struct ArcNode* nextarc;//指向下一个邻接点的指针
    InfoType info;//边的信息
}ArcNode;
typedef struct VNode {
    VertexType data;//顶点的数据域
    ArcNode* firstarc;//指向邻接点的指针
}VNode, AdjList[MAX_VEX];//存储各链表头结点的数组
typedef struct {
    AdjList vertices;//图中顶点的数组
    AdjList converse_vertices;//逆邻接表
    int vexnum, arcnum;//记录图中顶点数和边或弧数
}ALGraph;

//辅助数组Edges的定义(用于Kruskal算法）
struct {
    VertexType Head;						//边的始点
    VertexType Tail;						//边的终点
    EdgeType lowcost;						//边上的权值
}Edge[(MAX_VEX * (MAX_VEX - 1)) / 2];

int Vexset[MAX_VEX];							//辅助数组Vexset的定义

int *Length = new int[MAX_VEX];//记录最短路径长度
bool *S = new bool[MAX_VEX];//标记顶点是否进入S集合
int* path = new int[MAX_VEX];//记录最短路径顶点的前驱

//- - - - -顺序栈的定义- - - - -
typedef struct {
    int* base;
    int* top;
    int stacksize;
}spStack;
//- - - - - - - - - - - - - - - -

int indegree[MAX_VEX];						//数组indegree存放个顶点的入度
spStack sp;

//------------栈的相关操作----------------------
void InitStack(spStack& S) {
    //初始化栈
    S.base = new int[MAX_VEX];
    if (!S.base)
        exit(1);
    S.top = S.base;
    S.stacksize = MAX_VEX;
}//InitStack

void Push(spStack& S, int i) {
    //进栈
    if (S.top - S.base == S.stacksize)
        return;
    *S.top++ = i;
}//Push

void Pop(spStack& S, int& i) {
    //出栈
    if (S.top == S.base)
        return;
    i = *--S.top;
}//Pop

bool StackEmpty(spStack S) {
    //判断栈是否为空
    if (S.top == S.base)
        return true;
    return false;
}//StackEmpty
//-------------------------------------------------


int LocateVex_ADM(MGraph g, VertexType a)//定位顶点a在用邻接矩阵图中的位置
{
    for (int i = 0; i < g.vexnum; ++i)
    {
        if (g.Vex[i] == a)
            return i;
    }

}
int LocateVex_ALM(ALGraph g, VertexType a)//定位顶点a在用邻接表图中的位置
{
    for (int i = 0; i < g.vexnum; ++i)
    {
        if (g.vertices[i].data == a)
            return i;
    }

}
void WriteMGraph(MGraph* g1) {    //图的构造方法 
    ofstream outfile;
    outfile.open("a.txt");
    int i, j, k, w;
    char p, q;
    //cout << "请输入顶点数和边的数量" << endl;
    cout << "顶点数:如6 " << endl;
    cin >> g1->vexnum;
    cout << "边数：如6" << endl;
    cin >> g1->arcnum;
    //outfile << "顶点数:"<< endl;
    outfile << g1->vexnum << endl;
    //outfile << "边数:"  << endl;//可以像使用cout一样地使用ofstream对象
    outfile << g1->arcnum << endl;
    // cin >> g->vexnum >> g->arcnum;
    cout << "请输入顶点的信息：如abcd" << endl;
    for (int i = 0; i < g1->vexnum; i++)
    {
        cin >> g1->Vex[i];
    }
    for (int i = 0; i < g1->vexnum; i++)
    {
        outfile << g1->Vex[i];
    }
    outfile << endl;
    for (i = 0; i <= g1->vexnum; i++) {    //邻接矩阵初始化 
        for (j = 0; j <= g1->vexnum; j++)
        {
            if (i == j) {
                g1->Edge[i][j] = 0;
            }
            else {
                g1->Edge[i][j] = INF;
            }
        }

    }
   cout << "请输入边的信息格式为(出发点，到达点，权值):如a b 4" << endl;
    for (k = 0; k < g1->arcnum; k++) {    //读入每一条边的信息
        //cout << "请输入边的信息格式为(出发点，到达点，权值):" << endl;
        cin >> p >> q >> w;
        i = (int)(p - 'a');
        j = (int)(q - 'a');
        g1->Edge[i][j] = w;
        g1->Edge[j][i] = g1->Edge[i][j];
        outfile << p << "\t" << q << "\t" << w << endl;
    }
    outfile.close();
}

void Read_AD_MGraph(MGraph* g1)//无向图的邻接矩阵
{
    string s;
    ifstream infile;
    infile.open("a.txt");
    if (!infile)
        cout << "error" << endl;

    int i = 0;
    char vi, vj;
    int w, n, m;
    infile >> g1->vexnum;
    infile >> g1->arcnum;
    infile >> s;
    strcpy_s(g1->Vex, s.c_str());//空格也占一个，所有保证输入格式中此处不带空格
    for (int i = 0; i < g1->vexnum; i++) {			     //初始化顶点之间的权值 默认为无穷
        for (int j = 0; j < g1->vexnum; j++) {
            g1->Edge[i][j] = INF;
        }
    }
   // for (int i = 3; i < g1->arcnum; i++)
 //   {
        for (int j = 0; j < g1->arcnum; j++)
        {
            infile >> vi >> vj >> w;
            n = (int)(vi - 'a');
            m = (int)(vj - 'a'); 
            g1->Edge[n][m] = w;
            g1->Edge[m][n] = w;
            Edge[j].lowcost = w;
            Edge[j].Head = vi;
            Edge[j].Tail = vj;
        }
  //  }

    infile.close();
}
void Read_ADM_DIGraph(MGraph* g1)//有向图的邻接矩阵
{
    string s;
    ifstream infile;
    infile.open("a.txt");
    if (!infile)
        cout << "error" << endl;

    int i = 0;
    char vi, vj;
    int w, n, m;
    infile >> g1->vexnum;
    infile >> g1->arcnum;
    infile >> s;
    strcpy_s(g1->Vex, s.c_str());//空格也占一个，所有保证输入格式中此处不带空格
    for (int i = 0; i < g1->vexnum; i++) {			     //初始化顶点之间的权值 默认为无穷
        for (int j = 0; j < g1->vexnum; j++) {
            g1->Edge[i][j] = INF;
        }
    }
    // for (int i = 3; i < g1->arcnum; i++)
  //   {
    for (int j = 0; j < g1->arcnum; j++)
    {
        infile >> vi >> vj >> w;
        n = (int)(vi - 'a');
        m = (int)(vj - 'a');
        g1->Edge[n][m] = w;
       // g1->Edge[m][n] = w;
        Edge[j].lowcost = w;
        Edge[j].Head = vi;
        Edge[j].Tail = vj;
    }
    //  }

    infile.close();
}
void Read_AL_MGraph(ALGraph& G)
{
    string s;
    ifstream infile;
    infile.open("a.txt");
    if (!infile)
        cout << "error" << endl;

    int i = 0;
    char vi, vj;
    int w, n, m;
    char temp[20];
    infile >> G.vexnum;
    infile >> G.arcnum;
    infile >> s;
    strcpy_s(temp, s.c_str());//空格也占一个，所有保证输入格式中此处不带空格
    for (int a = 0; a < G.vexnum; a++)
    {
        G.vertices[a].data = temp[a];//输入顶点值
        G.vertices[a].firstarc = NULL;//初始化指针域为null
    }
    ArcNode* p1, * p2;
    for (int k = 0; k < G.arcnum; ++k)
    {
        int p, q, info;//相邻的两个顶点,边的权重
        infile >> vi >> vj >> info;//输入一条边的两个顶点
       // p = (int)(vi - 'a');
      //  q = (int)(vj - 'a');
        p = LocateVex_ALM(G, vi);
        q = LocateVex_ALM(G, vj);
        p1 = (ArcNode*)malloc(sizeof(ArcNode));//生成一个新的边界点p1
        p1->adjvex = q;//邻接点在数组中的位置为j
        p1->info = info;
        indegree[q]++;
        p1->nextarc = G.vertices[p].firstarc;
        G.vertices[p].firstarc = p1;
        //将新节点*p1插入到顶点vi的边表头部
        p2 = new ArcNode;//生成一个新的边界点p2
        p2->adjvex = p;//邻接点序列号为i
        p2->info = info;
        p2->nextarc = G.vertices[q].firstarc;
        G.vertices[q].firstarc = p2;
        //将新节点*p2插入到顶点vj的边表头部
    }

    infile.close();
}
void Read_AL_DIMGraph(ALGraph& G)//有向图的邻接表
{
    string s;
    ifstream infile;
    infile.open("a.txt");
    if (!infile)
        cout << "error" << endl;

    int i = 0;
    char vi, vj;
    int w, n, m;
    char temp[20];
    infile >> G.vexnum;
    infile >> G.arcnum;
    infile >> s;
    strcpy_s(temp, s.c_str());//空格也占一个，所有保证输入格式中此处不带空格
    for (int a = 0; a < G.vexnum; a++)
    {
        G.vertices[a].data = temp[a];//输入顶点值
        G.converse_vertices[a].data = G.vertices[a].data;
        G.vertices[a].firstarc = NULL;//初始化指针域为null
        G.converse_vertices[a].firstarc = NULL;
    }
    ArcNode* p1,*p2;
    for (int k = 0; k < G.arcnum; ++k)
    {
        int p, q, info;//相邻的两个顶点,边的权重
        infile >> vi >> vj >> info;//输入一条边的两个顶点
       // p = (int)(vi - 'a');
      //  q = (int)(vj - 'a');
        p = LocateVex_ALM(G, vi);
        q = LocateVex_ALM(G, vj);
        p1 = (ArcNode*)malloc(sizeof(ArcNode));//生成一个新的边界点p1
        p1->adjvex = q;//邻接点在数组中的位置为j
        p1->info = info;
        p1->nextarc = G.vertices[p].firstarc;
        G.vertices[p].firstarc = p1;
        indegree[q]++;
        //将新节点*p1插入到顶点vi的边表头部
       p2 = new ArcNode;//生成一个新的边界点p2
        p2->adjvex = p;//邻接点序列号为i
        p2->info = info;
        p2->nextarc = G.converse_vertices[q].firstarc;
        G.converse_vertices[q].firstarc = p2;
        //将新节点*p2插入到顶点vj的边表头部*/
    }

    infile.close();
}

void prin_ADM_Graph(MGraph* g) { //输出邻接矩阵

    //cout << "输出邻接矩阵" << endl;

    int index = 0;
    for (int i = 0; i < g->vexnum; i++) {
        //cout << g->Vex[index++] << " ";
        for (int j = 0; j < g->vexnum; j++) {
            if (g->Edge[i][j] == INF || g->Edge[i][j] == 0) {
                cout << "∞" << " ";
            }
            else {
                cout << g->Edge[i][j] << " ";
            }
        }cout << endl;
    }
}
void prin_AL_Graph(ALGraph G)//输出邻接表
{
    ArcNode* temp = NULL;
    for (int i = 0; i < G.vexnum; i++)
    {
        cout << G.vertices[i].data << "  ";
        temp = G.vertices[i].firstarc;
        while (temp)
        {
            cout << temp->adjvex << " " << temp->info << "  ";
            temp = temp->nextarc;
        }
        cout << endl;

    }
}
void Prim(MGraph* g, VertexType a)//从地点a出发，如果顶点是字符型，注意转换,用邻接矩阵存储图
{
    int i, j = 0;
    int lowcost[MAX_VEX];//lowcoaat[i]记录以i为终点的边的最小权值，当lowcoat[i]=0时表示终点i加入生成树
    int adjvex[MAX_VEX];//adjvex[i]记录对应lowcost[i]的起点，当adjvex[i]=0时表示起点i加入生成树
    int sum = 0;//总代价
    int min = 0;
    int minvex = 0;//最短距离点的int转换
    //int start=(int) a - 'a';

    int start = LocateVex_ADM(*g, a);
    for (int i = 0; i < g->vexnum; i++)
    {
        if (i == start)
            continue;
        else
        {
            lowcost[i] = g->Edge[start][i];//最短距离初始化为其他节点到该点的距离
            adjvex[i] = start;//标记其他所有节点的起点都是选择的初始点
        }
        adjvex[start] = 0;//标记初始点加入生成树

    }
    for (i = 0; i < g->vexnum; i++)
    {
        if (i == start)
            continue;
        min = INF;
        minvex = 0;
        for (int j = 0; j < g->vexnum; j++)//找到满足条件的最小权值边的节点minvex
        {
           if (j == start)
              continue;
            if (lowcost[j] < min && lowcost[j] != 0)//边权值最小且不在生成树中
            {
                min = lowcost[j];
                minvex = j;
            }
        }
       // printf_s("%c - %c: %d\n", adjvex[minvex] + 'a', minvex + 'a', min);//输出
        cout <<(char) (adjvex[minvex] + 'a') << " - " <<(char)( minvex + 'a' )<< "; " << min << endl;
        sum += min;//累加
        lowcost[minvex] = 0;//节点minvex加入生成树

        for (j = 0; j < g->vexnum; j++)
        {
            if (j== start)
                continue;
            if (g->Edge[minvex][j] < lowcost[j])
            {
                lowcost[j] = g->Edge[minvex][j];//更新当前顶点minvex到其他节点的权值
                adjvex[j] = minvex;//更新最小权值边的起点
            }
        }


    }
    cout << "总和：" << sum << endl;

    
}
void sort(MGraph G)//冒泡排序
{
    int n = G.arcnum - 2;
    int flag = 1;
    while((n > 0) && flag == 1)
    {
        flag = 0;
        for (int i = 0; i <= n; i++)
        {
            if (Edge[i].lowcost > Edge[i + 1].lowcost)
            {
                flag = 1;
                VertexType temp1 = Edge[i].Head;
                Edge[i].Head = Edge[i+1].Head;
                Edge[i + 1].Head = temp1;

                VertexType temp2 = Edge[i].Tail;
                Edge[i].Tail = Edge[i+1].Tail;
                Edge[i + 1].Tail = temp2;

                VertexType temp3 = Edge[i].lowcost;
                Edge[i].lowcost= Edge[i+1].lowcost;
                Edge[i + 1].lowcost = temp3;


            }
        }
    }
}

void Kruskal(MGraph g)//用邻接矩阵存储无向图
{
    sort(g);//将边Edge数组排序，按权值由小到大排序
    int sum = 0;
    for (int i = 0; i < g.vexnum; ++i)//利用辅助数组，表示各顶点自成一个连通分量
    {
        Vexset[i] = i;
    }
    int v1, v2, vs1, vs2;
    for (int k = 0;k < g.arcnum; ++k)
    {
       // v1 = (int)Edge[k].Head - 'a';//边起始点的下标
        v1 =LocateVex_ADM(g,Edge[k].Head);//边起始点的下标
       // v2= (int)Edge[k].Tail - 'a';//边终点的下标
        v2 = LocateVex_ADM(g,Edge[k].Tail);//边终点的下标
        vs1 = Vexset[v1];//边Edge[i]的始点所在的连通分量
        vs2 = Vexset[v2];// 边Edge[i]的终点所在的连通分量
        if (vs1 != vs2)//边的两点顶点属于不同的连通分量
        {
            cout << Edge[k].Head << " - " << Edge[k].Tail << ";" << Edge[k].lowcost << endl;
            sum += Edge[k].lowcost;
            for (int j = 0; j < g.vexnum; ++j)//合并vs1和vs2,使用同一编号
            {
                if (Vexset[j] == vs2)
                    Vexset[j] = vs1;
            }
        }
    }
    cout << "总和：" << sum << endl;
}

void Dijkstra(MGraph g, int a)//Dijkstra算法求有向图G的顶点a到其余顶点的最短路径
{
   // int start = (int)(a - 'a');
    for (int i = 0; i < g.vexnum; i++)
    {
        S[i] = false;//将s初始为空集
        Length[i] = g.Edge[a][i];//将初始顶点到各个终点的最短路径长度初始化为边的权值
        if (Length[i] < INF)//如果初始顶点与顶点vex[i]之间有弧，则vex[i]的前驱置为初始点，否则为-1；
            path[i] = a;
        else
            path[i] = -1;
    }
    S[a] = true; //将初始点加入S
    Length[a] = 0;//起点到起点之间的距离为0

    int p,k;
    for (int j = 1; j < g.vexnum; ++j)//通过主循环求得起始点到某个点的最短路径，将该顶点加入S集,对其他顶点依此进行计算
    {
        int min = INF;
        for ( k = 0; k < g.vexnum; ++k)
        {
            if (!S[k] && Length[k] < min) //选择一条当前的最短路径，终点为p
            {
                p = k;
                min = Length[k];
            }
        }
            S[p] = true;//将p加入S
            for (k = 0; k < g.vexnum; ++k)//更新从起始点出发到所有顶点的最短长度
            {
                if (!S[k] && (Length[p] + g.Edge[p][k] < Length[k]))
                {
                    Length[k] = Length[p] + g.Edge[p][k];//更新length[k]
                    path[k] = p;//更改k的前驱为p
                }
            }
        
    }
}
void DisplayPath(MGraph g, int start, int temp)
{
    if (path[temp] != -1)
    {
        DisplayPath(g, start, path[temp]);
        cout << g.Vex[path[temp]] << "-->";
    }
}
void FindInDegree(ALGraph G) {
    //求出各顶点的入度存入数组indegree中 
    int i, count;

    for (i = 0; i < G.vexnum; i++) {
        count = 0;
        ArcNode* p = G.converse_vertices[i].firstarc;
        if (p) {
            while (p) {
                p = p->nextarc;
                count++;
            }
        }
        indegree[i] = count;
    }
}//FindInDegree

int TopologicalSort(ALGraph g, int* topo)//拓扑排序有向无环图，用邻接表存储有向图
{
    FindInDegree(g);
    InitStack(sp);//初始化S
    ArcNode* p;
    int i, m, k;
    for (i = 0; i < g.vexnum; ++i)//检查所有节点中是否有入度为0的节点，如果有就进栈
    {
        if (!indegree[i])//数组为零
        {
            Push(sp, i);
        }
    }

        m = 0;//对输出顶点计数，初始为0
      
        while (!StackEmpty(sp))//当栈不为空时，先出栈，取出栈顶元素，将其记录在topo数组中
        {
            Pop(sp, i);//栈顶vi出栈
            topo[m] = i;//将vi保存在拓扑序列数组topo中
            ++m;//对输出的顶点计数
            p = g.vertices[i].firstarc;//指针p指向vi的第一个邻接点
            while (p != NULL)
            {
                k = p->adjvex; // vk为vi的邻接点
                --indegree[k];//vi的每个邻接点的入度减一
                if (indegree[k] == 0)//若入度为0,则入栈
                    Push(sp, k);
                p = p->nextarc;//p指向vi的下一个邻接点
            }
        }

  //  topo[m] = -1;//为-1时结束
    if (m < g.vexnum)
        return ERROR;
    else
        return OK;
}


int main()
{

    MGraph* G1 = new MGraph;
    MGraph* G2 = new MGraph;
    MGraph* G5 = new MGraph;//利用邻接矩阵存储的有向图
    ALGraph G3;
    ALGraph G4;
    ALGraph G6;//利用邻接表存储的有向图
    cout << "请输入想要存入的图的相关信息" << endl;
    WriteMGraph(G1);
    cout << endl;
    cout << "根据存入的信息得到的无向图的邻接表如下：" << endl;
    Read_AL_MGraph(G3);
    prin_AL_Graph(G3);
    //Read_ADM_DIGraph(G5);//有向图的邻接矩阵
    cout << "根据存入的信息得到的有向图的邻接表如下：" << endl;
    Read_AL_DIMGraph(G6);
    prin_AL_Graph(G6);
    cout << endl;
    cout << "根据存入的信息得到的无向图的邻接矩阵如下：" << endl;
    Read_AD_MGraph(G2);
    prin_ADM_Graph(G2);
    cout << "根据存入的信息得到的有向图的邻接矩阵如下：" << endl;
    Read_ADM_DIGraph(G5);//有向图的邻接矩阵
    prin_ADM_Graph(G5);
    bool flag = true;
    while(flag)
    { cout << "请选择您想测试的算法：1-Prim算法 2-Kruskal算法 3-Dijkstra算法 4-拓扑排序算法"<<endl;
       int p;
       cin >> p;
       switch (p)
       {
       case 1:char c; cout << "请输入Prim算法中起始顶点：" << endl; cin >> c;
           cout << "使用Prim算法得到的最小生成树：" << endl;
           Prim(G2, c); break;
       case 2: cout << "使用Kruskal算法得到的最小生成树：" << endl;
           Kruskal(*G2); break;
       case 3:cout << "使用Dijkstra算法得到最短路径：" << endl;  VertexType start, destination; int s, d;
           cout << "请输入路径的起点和终点：";  cin >> start >> destination;
           s = LocateVex_ADM(*G2, start);  d = LocateVex_ADM(*G2, destination); Dijkstra(*G2, s);
           cout << endl << "最短路径：";   DisplayPath(*G2, s, d); cout << G2->Vex[d] << endl; break;
       case 4:cout << "使用拓扑排序算法得到的拓扑序列：" << endl; int topo[MAX_VEX] = { 0 }; 
           if (TopologicalSort(G6, topo))
           {
               cout << "有向无环图的拓扑排序" << endl;
               for (int i = 0; i < G6.vexnum; i++)
               {
                   cout << G6.vertices[topo[i]].data << " ";
               }
               cout << endl;
           }
           else
           {
               cout << "此有向图有环，请重新输入" << endl;
           }
           break;

       //default:break;
       }
       cout << "是否继续测试(Y-继续，N-退出)" << endl;
       char q;
       cin >> q;
       if (q == 'Y')
           flag = true;
       else
           flag = false;
       
    }

   /* char a;
    cout << "请输入Prim算法中起始顶点：" << endl;
    cin >> a;
    cout << "使用Prim算法得到的最小生成树：" << endl;
    Prim(G2, a);
    cout << "使用Kruskal算法得到的最小生成树：" << endl;
    Kruskal(*G2);
    cout << "使用Dijkstra算法得到最短路径：" << endl;
  //  cout << "有向图的邻接矩阵：" << endl;
   // prin_ADM_Graph(G5);
    VertexType start, destination;
    int s, d;
    cout << "请输入路径的起点和终点：";
    cin >> start >> destination;
    //s = (int)(start - 'a');
    s = LocateVex_ADM(*G2, start);
   // d = (int)(destination - 'a');
    d = LocateVex_ADM(*G2, destination);
    Dijkstra(*G2, s);
    cout << endl << "最短路径：";
    DisplayPath(*G2, s, d);
    cout << G2->Vex[d] << endl;
    cout << "使用拓扑排序算法得到的拓扑序列：" << endl;
    //cout << "有向图的邻接表表示：" << endl;
   // Read_AL_DIMGraph(G6);
 //  prin_AL_Graph(G6);
    
    int topo[MAX_VEX] = { 0 };
  
    if (TopologicalSort(G6, topo))
    {
        cout << "有向无环图的拓扑排序" << endl;
        for (int i = 0;i< G6.vexnum; i++)
        {
            cout << G6.vertices[topo[i]].data<<" ";
        }
        cout << endl;
    }
    else
    {
        cout << "此有向图有环，请重新输入" << endl;
    }*/
    system("pause");
    return 0;

}
