#include<iostream>
#include<fstream>
#include<string>
using namespace std;
#define MAX_VEX 100            // ͼ�к��ж����������
#define INF 65535              //�����������֮�䲻�ɴ�������ʾ����
#define OK 1
#define ERROR 0
typedef char VertexType;
typedef int EdgeType;
#define  InfoType int//ͼ�л����߱߰�������Ϣ������

typedef struct {
    VertexType Vex[MAX_VEX];    //ͼ�еĶ��� 
    EdgeType Edge[MAX_VEX][MAX_VEX];  //�ڽӾ��� 
    int vexnum, arcnum;   //ͼ�еĶ������ͱߵ����� 
}MGraph;

typedef struct ArcNode {
    int adjvex;//�ڽӵ��������е�λ���±�
    struct ArcNode* nextarc;//ָ����һ���ڽӵ��ָ��
    InfoType info;//�ߵ���Ϣ
}ArcNode;
typedef struct VNode {
    VertexType data;//�����������
    ArcNode* firstarc;//ָ���ڽӵ��ָ��
}VNode, AdjList[MAX_VEX];//�洢������ͷ��������
typedef struct {
    AdjList vertices;//ͼ�ж��������
    AdjList converse_vertices;//���ڽӱ�
    int vexnum, arcnum;//��¼ͼ�ж������ͱ߻���
}ALGraph;

//��������Edges�Ķ���(����Kruskal�㷨��
struct {
    VertexType Head;						//�ߵ�ʼ��
    VertexType Tail;						//�ߵ��յ�
    EdgeType lowcost;						//���ϵ�Ȩֵ
}Edge[(MAX_VEX * (MAX_VEX - 1)) / 2];

int Vexset[MAX_VEX];							//��������Vexset�Ķ���

int *Length = new int[MAX_VEX];//��¼���·������
bool *S = new bool[MAX_VEX];//��Ƕ����Ƿ����S����
int* path = new int[MAX_VEX];//��¼���·�������ǰ��

//- - - - -˳��ջ�Ķ���- - - - -
typedef struct {
    int* base;
    int* top;
    int stacksize;
}spStack;
//- - - - - - - - - - - - - - - -

int indegree[MAX_VEX];						//����indegree��Ÿ���������
spStack sp;

//------------ջ����ز���----------------------
void InitStack(spStack& S) {
    //��ʼ��ջ
    S.base = new int[MAX_VEX];
    if (!S.base)
        exit(1);
    S.top = S.base;
    S.stacksize = MAX_VEX;
}//InitStack

void Push(spStack& S, int i) {
    //��ջ
    if (S.top - S.base == S.stacksize)
        return;
    *S.top++ = i;
}//Push

void Pop(spStack& S, int& i) {
    //��ջ
    if (S.top == S.base)
        return;
    i = *--S.top;
}//Pop

bool StackEmpty(spStack S) {
    //�ж�ջ�Ƿ�Ϊ��
    if (S.top == S.base)
        return true;
    return false;
}//StackEmpty
//-------------------------------------------------


int LocateVex_ADM(MGraph g, VertexType a)//��λ����a�����ڽӾ���ͼ�е�λ��
{
    for (int i = 0; i < g.vexnum; ++i)
    {
        if (g.Vex[i] == a)
            return i;
    }

}
int LocateVex_ALM(ALGraph g, VertexType a)//��λ����a�����ڽӱ�ͼ�е�λ��
{
    for (int i = 0; i < g.vexnum; ++i)
    {
        if (g.vertices[i].data == a)
            return i;
    }

}
void WriteMGraph(MGraph* g1) {    //ͼ�Ĺ��췽�� 
    ofstream outfile;
    outfile.open("a.txt");
    int i, j, k, w;
    char p, q;
    //cout << "�����붥�����ͱߵ�����" << endl;
    cout << "������:��6 " << endl;
    cin >> g1->vexnum;
    cout << "��������6" << endl;
    cin >> g1->arcnum;
    //outfile << "������:"<< endl;
    outfile << g1->vexnum << endl;
    //outfile << "����:"  << endl;//������ʹ��coutһ����ʹ��ofstream����
    outfile << g1->arcnum << endl;
    // cin >> g->vexnum >> g->arcnum;
    cout << "�����붥�����Ϣ����abcd" << endl;
    for (int i = 0; i < g1->vexnum; i++)
    {
        cin >> g1->Vex[i];
    }
    for (int i = 0; i < g1->vexnum; i++)
    {
        outfile << g1->Vex[i];
    }
    outfile << endl;
    for (i = 0; i <= g1->vexnum; i++) {    //�ڽӾ����ʼ�� 
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
   cout << "������ߵ���Ϣ��ʽΪ(�����㣬����㣬Ȩֵ):��a b 4" << endl;
    for (k = 0; k < g1->arcnum; k++) {    //����ÿһ���ߵ���Ϣ
        //cout << "������ߵ���Ϣ��ʽΪ(�����㣬����㣬Ȩֵ):" << endl;
        cin >> p >> q >> w;
        i = (int)(p - 'a');
        j = (int)(q - 'a');
        g1->Edge[i][j] = w;
        g1->Edge[j][i] = g1->Edge[i][j];
        outfile << p << "\t" << q << "\t" << w << endl;
    }
    outfile.close();
}

void Read_AD_MGraph(MGraph* g1)//����ͼ���ڽӾ���
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
    strcpy_s(g1->Vex, s.c_str());//�ո�Ҳռһ�������б�֤�����ʽ�д˴������ո�
    for (int i = 0; i < g1->vexnum; i++) {			     //��ʼ������֮���Ȩֵ Ĭ��Ϊ����
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
void Read_ADM_DIGraph(MGraph* g1)//����ͼ���ڽӾ���
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
    strcpy_s(g1->Vex, s.c_str());//�ո�Ҳռһ�������б�֤�����ʽ�д˴������ո�
    for (int i = 0; i < g1->vexnum; i++) {			     //��ʼ������֮���Ȩֵ Ĭ��Ϊ����
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
    strcpy_s(temp, s.c_str());//�ո�Ҳռһ�������б�֤�����ʽ�д˴������ո�
    for (int a = 0; a < G.vexnum; a++)
    {
        G.vertices[a].data = temp[a];//���붥��ֵ
        G.vertices[a].firstarc = NULL;//��ʼ��ָ����Ϊnull
    }
    ArcNode* p1, * p2;
    for (int k = 0; k < G.arcnum; ++k)
    {
        int p, q, info;//���ڵ���������,�ߵ�Ȩ��
        infile >> vi >> vj >> info;//����һ���ߵ���������
       // p = (int)(vi - 'a');
      //  q = (int)(vj - 'a');
        p = LocateVex_ALM(G, vi);
        q = LocateVex_ALM(G, vj);
        p1 = (ArcNode*)malloc(sizeof(ArcNode));//����һ���µı߽��p1
        p1->adjvex = q;//�ڽӵ��������е�λ��Ϊj
        p1->info = info;
        indegree[q]++;
        p1->nextarc = G.vertices[p].firstarc;
        G.vertices[p].firstarc = p1;
        //���½ڵ�*p1���뵽����vi�ı߱�ͷ��
        p2 = new ArcNode;//����һ���µı߽��p2
        p2->adjvex = p;//�ڽӵ����к�Ϊi
        p2->info = info;
        p2->nextarc = G.vertices[q].firstarc;
        G.vertices[q].firstarc = p2;
        //���½ڵ�*p2���뵽����vj�ı߱�ͷ��
    }

    infile.close();
}
void Read_AL_DIMGraph(ALGraph& G)//����ͼ���ڽӱ�
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
    strcpy_s(temp, s.c_str());//�ո�Ҳռһ�������б�֤�����ʽ�д˴������ո�
    for (int a = 0; a < G.vexnum; a++)
    {
        G.vertices[a].data = temp[a];//���붥��ֵ
        G.converse_vertices[a].data = G.vertices[a].data;
        G.vertices[a].firstarc = NULL;//��ʼ��ָ����Ϊnull
        G.converse_vertices[a].firstarc = NULL;
    }
    ArcNode* p1,*p2;
    for (int k = 0; k < G.arcnum; ++k)
    {
        int p, q, info;//���ڵ���������,�ߵ�Ȩ��
        infile >> vi >> vj >> info;//����һ���ߵ���������
       // p = (int)(vi - 'a');
      //  q = (int)(vj - 'a');
        p = LocateVex_ALM(G, vi);
        q = LocateVex_ALM(G, vj);
        p1 = (ArcNode*)malloc(sizeof(ArcNode));//����һ���µı߽��p1
        p1->adjvex = q;//�ڽӵ��������е�λ��Ϊj
        p1->info = info;
        p1->nextarc = G.vertices[p].firstarc;
        G.vertices[p].firstarc = p1;
        indegree[q]++;
        //���½ڵ�*p1���뵽����vi�ı߱�ͷ��
       p2 = new ArcNode;//����һ���µı߽��p2
        p2->adjvex = p;//�ڽӵ����к�Ϊi
        p2->info = info;
        p2->nextarc = G.converse_vertices[q].firstarc;
        G.converse_vertices[q].firstarc = p2;
        //���½ڵ�*p2���뵽����vj�ı߱�ͷ��*/
    }

    infile.close();
}

void prin_ADM_Graph(MGraph* g) { //����ڽӾ���

    //cout << "����ڽӾ���" << endl;

    int index = 0;
    for (int i = 0; i < g->vexnum; i++) {
        //cout << g->Vex[index++] << " ";
        for (int j = 0; j < g->vexnum; j++) {
            if (g->Edge[i][j] == INF || g->Edge[i][j] == 0) {
                cout << "��" << " ";
            }
            else {
                cout << g->Edge[i][j] << " ";
            }
        }cout << endl;
    }
}
void prin_AL_Graph(ALGraph G)//����ڽӱ�
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
void Prim(MGraph* g, VertexType a)//�ӵص�a����������������ַ��ͣ�ע��ת��,���ڽӾ���洢ͼ
{
    int i, j = 0;
    int lowcost[MAX_VEX];//lowcoaat[i]��¼��iΪ�յ�ıߵ���СȨֵ����lowcoat[i]=0ʱ��ʾ�յ�i����������
    int adjvex[MAX_VEX];//adjvex[i]��¼��Ӧlowcost[i]����㣬��adjvex[i]=0ʱ��ʾ���i����������
    int sum = 0;//�ܴ���
    int min = 0;
    int minvex = 0;//��̾�����intת��
    //int start=(int) a - 'a';

    int start = LocateVex_ADM(*g, a);
    for (int i = 0; i < g->vexnum; i++)
    {
        if (i == start)
            continue;
        else
        {
            lowcost[i] = g->Edge[start][i];//��̾����ʼ��Ϊ�����ڵ㵽�õ�ľ���
            adjvex[i] = start;//����������нڵ����㶼��ѡ��ĳ�ʼ��
        }
        adjvex[start] = 0;//��ǳ�ʼ�����������

    }
    for (i = 0; i < g->vexnum; i++)
    {
        if (i == start)
            continue;
        min = INF;
        minvex = 0;
        for (int j = 0; j < g->vexnum; j++)//�ҵ�������������СȨֵ�ߵĽڵ�minvex
        {
           if (j == start)
              continue;
            if (lowcost[j] < min && lowcost[j] != 0)//��Ȩֵ��С�Ҳ�����������
            {
                min = lowcost[j];
                minvex = j;
            }
        }
       // printf_s("%c - %c: %d\n", adjvex[minvex] + 'a', minvex + 'a', min);//���
        cout <<(char) (adjvex[minvex] + 'a') << " - " <<(char)( minvex + 'a' )<< "; " << min << endl;
        sum += min;//�ۼ�
        lowcost[minvex] = 0;//�ڵ�minvex����������

        for (j = 0; j < g->vexnum; j++)
        {
            if (j== start)
                continue;
            if (g->Edge[minvex][j] < lowcost[j])
            {
                lowcost[j] = g->Edge[minvex][j];//���µ�ǰ����minvex�������ڵ��Ȩֵ
                adjvex[j] = minvex;//������СȨֵ�ߵ����
            }
        }


    }
    cout << "�ܺͣ�" << sum << endl;

    
}
void sort(MGraph G)//ð������
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

void Kruskal(MGraph g)//���ڽӾ���洢����ͼ
{
    sort(g);//����Edge�������򣬰�Ȩֵ��С��������
    int sum = 0;
    for (int i = 0; i < g.vexnum; ++i)//���ø������飬��ʾ�������Գ�һ����ͨ����
    {
        Vexset[i] = i;
    }
    int v1, v2, vs1, vs2;
    for (int k = 0;k < g.arcnum; ++k)
    {
       // v1 = (int)Edge[k].Head - 'a';//����ʼ����±�
        v1 =LocateVex_ADM(g,Edge[k].Head);//����ʼ����±�
       // v2= (int)Edge[k].Tail - 'a';//���յ���±�
        v2 = LocateVex_ADM(g,Edge[k].Tail);//���յ���±�
        vs1 = Vexset[v1];//��Edge[i]��ʼ�����ڵ���ͨ����
        vs2 = Vexset[v2];// ��Edge[i]���յ����ڵ���ͨ����
        if (vs1 != vs2)//�ߵ����㶥�����ڲ�ͬ����ͨ����
        {
            cout << Edge[k].Head << " - " << Edge[k].Tail << ";" << Edge[k].lowcost << endl;
            sum += Edge[k].lowcost;
            for (int j = 0; j < g.vexnum; ++j)//�ϲ�vs1��vs2,ʹ��ͬһ���
            {
                if (Vexset[j] == vs2)
                    Vexset[j] = vs1;
            }
        }
    }
    cout << "�ܺͣ�" << sum << endl;
}

void Dijkstra(MGraph g, int a)//Dijkstra�㷨������ͼG�Ķ���a�����ඥ������·��
{
   // int start = (int)(a - 'a');
    for (int i = 0; i < g.vexnum; i++)
    {
        S[i] = false;//��s��ʼΪ�ռ�
        Length[i] = g.Edge[a][i];//����ʼ���㵽�����յ�����·�����ȳ�ʼ��Ϊ�ߵ�Ȩֵ
        if (Length[i] < INF)//�����ʼ�����붥��vex[i]֮���л�����vex[i]��ǰ����Ϊ��ʼ�㣬����Ϊ-1��
            path[i] = a;
        else
            path[i] = -1;
    }
    S[a] = true; //����ʼ�����S
    Length[a] = 0;//��㵽���֮��ľ���Ϊ0

    int p,k;
    for (int j = 1; j < g.vexnum; ++j)//ͨ����ѭ�������ʼ�㵽ĳ��������·�������ö������S��,�������������˽��м���
    {
        int min = INF;
        for ( k = 0; k < g.vexnum; ++k)
        {
            if (!S[k] && Length[k] < min) //ѡ��һ����ǰ�����·�����յ�Ϊp
            {
                p = k;
                min = Length[k];
            }
        }
            S[p] = true;//��p����S
            for (k = 0; k < g.vexnum; ++k)//���´���ʼ����������ж������̳���
            {
                if (!S[k] && (Length[p] + g.Edge[p][k] < Length[k]))
                {
                    Length[k] = Length[p] + g.Edge[p][k];//����length[k]
                    path[k] = p;//����k��ǰ��Ϊp
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
    //������������ȴ�������indegree�� 
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

int TopologicalSort(ALGraph g, int* topo)//�������������޻�ͼ�����ڽӱ�洢����ͼ
{
    FindInDegree(g);
    InitStack(sp);//��ʼ��S
    ArcNode* p;
    int i, m, k;
    for (i = 0; i < g.vexnum; ++i)//������нڵ����Ƿ������Ϊ0�Ľڵ㣬����оͽ�ջ
    {
        if (!indegree[i])//����Ϊ��
        {
            Push(sp, i);
        }
    }

        m = 0;//����������������ʼΪ0
      
        while (!StackEmpty(sp))//��ջ��Ϊ��ʱ���ȳ�ջ��ȡ��ջ��Ԫ�أ������¼��topo������
        {
            Pop(sp, i);//ջ��vi��ջ
            topo[m] = i;//��vi������������������topo��
            ++m;//������Ķ������
            p = g.vertices[i].firstarc;//ָ��pָ��vi�ĵ�һ���ڽӵ�
            while (p != NULL)
            {
                k = p->adjvex; // vkΪvi���ڽӵ�
                --indegree[k];//vi��ÿ���ڽӵ����ȼ�һ
                if (indegree[k] == 0)//�����Ϊ0,����ջ
                    Push(sp, k);
                p = p->nextarc;//pָ��vi����һ���ڽӵ�
            }
        }

  //  topo[m] = -1;//Ϊ-1ʱ����
    if (m < g.vexnum)
        return ERROR;
    else
        return OK;
}


int main()
{

    MGraph* G1 = new MGraph;
    MGraph* G2 = new MGraph;
    MGraph* G5 = new MGraph;//�����ڽӾ���洢������ͼ
    ALGraph G3;
    ALGraph G4;
    ALGraph G6;//�����ڽӱ�洢������ͼ
    cout << "��������Ҫ�����ͼ�������Ϣ" << endl;
    WriteMGraph(G1);
    cout << endl;
    cout << "���ݴ������Ϣ�õ�������ͼ���ڽӱ����£�" << endl;
    Read_AL_MGraph(G3);
    prin_AL_Graph(G3);
    //Read_ADM_DIGraph(G5);//����ͼ���ڽӾ���
    cout << "���ݴ������Ϣ�õ�������ͼ���ڽӱ����£�" << endl;
    Read_AL_DIMGraph(G6);
    prin_AL_Graph(G6);
    cout << endl;
    cout << "���ݴ������Ϣ�õ�������ͼ���ڽӾ������£�" << endl;
    Read_AD_MGraph(G2);
    prin_ADM_Graph(G2);
    cout << "���ݴ������Ϣ�õ�������ͼ���ڽӾ������£�" << endl;
    Read_ADM_DIGraph(G5);//����ͼ���ڽӾ���
    prin_ADM_Graph(G5);
    bool flag = true;
    while(flag)
    { cout << "��ѡ��������Ե��㷨��1-Prim�㷨 2-Kruskal�㷨 3-Dijkstra�㷨 4-���������㷨"<<endl;
       int p;
       cin >> p;
       switch (p)
       {
       case 1:char c; cout << "������Prim�㷨����ʼ���㣺" << endl; cin >> c;
           cout << "ʹ��Prim�㷨�õ�����С��������" << endl;
           Prim(G2, c); break;
       case 2: cout << "ʹ��Kruskal�㷨�õ�����С��������" << endl;
           Kruskal(*G2); break;
       case 3:cout << "ʹ��Dijkstra�㷨�õ����·����" << endl;  VertexType start, destination; int s, d;
           cout << "������·���������յ㣺";  cin >> start >> destination;
           s = LocateVex_ADM(*G2, start);  d = LocateVex_ADM(*G2, destination); Dijkstra(*G2, s);
           cout << endl << "���·����";   DisplayPath(*G2, s, d); cout << G2->Vex[d] << endl; break;
       case 4:cout << "ʹ�����������㷨�õ����������У�" << endl; int topo[MAX_VEX] = { 0 }; 
           if (TopologicalSort(G6, topo))
           {
               cout << "�����޻�ͼ����������" << endl;
               for (int i = 0; i < G6.vexnum; i++)
               {
                   cout << G6.vertices[topo[i]].data << " ";
               }
               cout << endl;
           }
           else
           {
               cout << "������ͼ�л�������������" << endl;
           }
           break;

       //default:break;
       }
       cout << "�Ƿ��������(Y-������N-�˳�)" << endl;
       char q;
       cin >> q;
       if (q == 'Y')
           flag = true;
       else
           flag = false;
       
    }

   /* char a;
    cout << "������Prim�㷨����ʼ���㣺" << endl;
    cin >> a;
    cout << "ʹ��Prim�㷨�õ�����С��������" << endl;
    Prim(G2, a);
    cout << "ʹ��Kruskal�㷨�õ�����С��������" << endl;
    Kruskal(*G2);
    cout << "ʹ��Dijkstra�㷨�õ����·����" << endl;
  //  cout << "����ͼ���ڽӾ���" << endl;
   // prin_ADM_Graph(G5);
    VertexType start, destination;
    int s, d;
    cout << "������·���������յ㣺";
    cin >> start >> destination;
    //s = (int)(start - 'a');
    s = LocateVex_ADM(*G2, start);
   // d = (int)(destination - 'a');
    d = LocateVex_ADM(*G2, destination);
    Dijkstra(*G2, s);
    cout << endl << "���·����";
    DisplayPath(*G2, s, d);
    cout << G2->Vex[d] << endl;
    cout << "ʹ�����������㷨�õ����������У�" << endl;
    //cout << "����ͼ���ڽӱ��ʾ��" << endl;
   // Read_AL_DIMGraph(G6);
 //  prin_AL_Graph(G6);
    
    int topo[MAX_VEX] = { 0 };
  
    if (TopologicalSort(G6, topo))
    {
        cout << "�����޻�ͼ����������" << endl;
        for (int i = 0;i< G6.vexnum; i++)
        {
            cout << G6.vertices[topo[i]].data<<" ";
        }
        cout << endl;
    }
    else
    {
        cout << "������ͼ�л�������������" << endl;
    }*/
    system("pause");
    return 0;

}
