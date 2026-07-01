#include "space_graph.h"

void initGraph(Graph *G){
    G->vexNum=0;
    G->edgeNum=0;
    G->maxfloor=0;
    memset(G->head,0,sizeof(G->head));
    memset(G->space,0,sizeof(G->space));
}

void buildGraph(Graph *G){
    printf("请输入你想加入的空间的个数(包括楼梯):\n");
    printf("(注:该程序楼梯仅支持跨越一层楼层)\n");
    scanf("%d",&G->vexNum);
    for(int i=0;i<G->vexNum;i++)addSpace(&G->space[i],i,&((*G).maxfloor));
    printf("请输入你想加入的走廊的个数:\n");
    scanf("%d",&G->edgeNum);
    for(int i=0;i<G->edgeNum;i++)addEdge(G);
}

void addSpace(SpaceNode *newSpace,int id,int *maxfloor){
    int ifexit=0;
    newSpace->id=id;
    printf("请输入当前空间的名字:");
    scanf("%s",newSpace->name);
    ifexit=
    printf("请输入当前空间的类型:\n");
    printf("(1.展厅 2.院落 3.门洞 4.楼梯 5.安全出口)\n");
    scanf("%d",&newSpace->type);
    printf("请输入该空间所处楼层:\n(目前已建立楼层数-%d)\n",(*maxfloor));
    int nowfloor;
    scanf("%d",&nowfloor);
    while(nowfloor>(*maxfloor)+1||nowfloor<0){
        if(nowfloor>(*maxfloor)+1)printf("当前尚未建立%d楼,不可建立%d楼\n",(*maxfloor)+1,nowfloor);
        else if(nowfloor<0)printf("该程序不支持创建地下室\n");
        printf("请重新输入该空间所处楼层:\n");
        scanf("%d",&nowfloor);
    }
    newSpace->iflock=UNLOCK;
}

int findSpace(Graph *G,char *name){
    for(int i=0;i<G->vexNum;i++){
        if(strcmp(G->space[i].name,name)==0)return i;
    }
    return -1;
}

void addEdge(Graph *G){
    char str_u[20],str_v[20];
    int u,v,weight;

    printf("请输入该走廊连接的第一个空间:\n");
    scanf("%s",str_u);
    u=findSpace(G,str_u);
    while(u==-1){
        printf("该空间不存在,请输入正确的空间名!\n");
        scanf("%s",str_u);
        u=findSpace(G,str_u);
    }
    
    printf("请输入该走廊连接的第二个空间:\n");
    scanf("%s",str_v);
    v=findSpace(G,str_v);
    while(u==v||v==-1){
        if(u==v)printf("禁止添加自环走廊,请重新输入\n");
        if(v==-1)printf("该空间不存在,请输入正确的空间名!\n");
        scanf("%s",str_v);
        v=findSpace(G,str_v);
    }

    printf("请输入该走廊的长度:\n");
    scanf("%d",&weight);
    while(weight<0){
        printf("走廊长度不可为负数,请重新输入!\n");
        scanf("%d",&weight);
    }

    EdgeNode *newEdgeU = (EdgeNode *)malloc(sizeof(EdgeNode));
    newEdgeU->adjVex = v;
    newEdgeU->weight = weight;
    newEdgeU->iflock = UNLOCK;
    newEdgeU->next = G->head[u]; // 头插法
    G->head[u] = newEdgeU;

    EdgeNode *newEdgeV = (EdgeNode *)malloc(sizeof(EdgeNode));
    newEdgeV->adjVex = u;
    newEdgeV->weight = weight;
    newEdgeV->iflock = UNLOCK;
    newEdgeV->next = G->head[v];
    G->head[v] = newEdgeV;
}

int isSpaceExit(Graph *G,char *name){
    return findSpace(G, name) != -1 ? 1 : 0;
}

int isEdgeExit(Graph *G, int u, int v) {
    if (u < 0 || u >= G->vexNum || v < 0 || v >= G->vexNum) return 0;
    EdgeNode *p = G->head[u];
    while (p) {
        if (p->adjVex == v && p->iflock == UNLOCK) { // 仅判断未锁定的边
            return 1;
        }
        p = p->next;
    }
    return 0;
}

void lockSpace(Graph *G) {
    char name[20];
    printf("请输入要暂时关闭的空间名称：\n");
    scanf("%s", name);
    int id = findSpace(G, name);
    if (id == -1) {
        printf("该空间不存在！\n");
    } else {
        G->space[id].iflock = LOCK;
        printf("空间 %s 已暂时关闭！\n", name);
    }
}

void unlockSpace(Graph *G){
    char name[20];
    printf("请输入要重新开放的空间名称：\n");
    scanf("%s", name);
    int id = findSpace(G, name);
    if (id == -1) {
        printf("该空间不存在！\n");
    } else {
        G->space[id].iflock = UNLOCK;
        printf("空间 %s 已重新开放！\n", name);
    }
}

void lockEdge(Graph *G){

}

void unlockEdge(Graph *G){

}

// 保存图数据到文件（二进制存储，保证数据完整性）
void saveGraph(Graph *G, const char *filename) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("文件打开失败，保存失败！\n");
        return;
    }

    // 1. 写入基本信息
    fwrite(&G->vexNum, sizeof(int), 1, fp);
    fwrite(&G->edgeNum, sizeof(int), 1, fp);
    fwrite(&G->maxfloor, sizeof(int), 1, fp);

    // 2. 写入所有空间节点
    for (int i = 0; i < G->vexNum; i++) {
        fwrite(&G->space[i], sizeof(SpaceNode), 1, fp);
    }

    // 3. 写入所有边（邻接表）
    for (int i = 0; i < G->vexNum; i++) {
        EdgeNode *p = G->head[i];
        // 先写入当前顶点的边数量
        int edgeCount = 0;
        EdgeNode *tmp = p;
        while (tmp) {
            edgeCount++;
            tmp = tmp->next;
        }
        fwrite(&edgeCount, sizeof(int), 1, fp);

        // 写入每条边的信息
        while (p) {
            fwrite(&p->adjVex, sizeof(int), 1, fp);
            fwrite(&p->weight, sizeof(int), 1, fp);
            fwrite(&p->iflock, sizeof(isLocked), 1, fp);
            p = p->next;
        }
    }

    fclose(fp);
    printf("建筑数据保存成功！\n");
}

// 从文件加载图数据
int loadGraph(Graph *G, const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("未找到历史建筑文件！\n");
        return 0; // 加载失败
    }

    // 先初始化图
    initGraph(G);

    // 1. 读取基本信息
    fread(&G->vexNum, sizeof(int), 1, fp);
    fread(&G->edgeNum, sizeof(int), 1, fp);
    fread(&G->maxfloor, sizeof(int), 1, fp);

    // 2. 读取空间节点
    for (int i = 0; i < G->vexNum; i++) {
        fread(&G->space[i], sizeof(SpaceNode), 1, fp);
    }

    // 3. 读取邻接表边
    for (int i = 0; i < G->vexNum; i++) {
        int edgeCount;
        fread(&edgeCount, sizeof(int), 1, fp);
        EdgeNode *tail = NULL; // 尾指针，用于链表插入
        for (int j = 0; j < edgeCount; j++) {
            EdgeNode *newEdge = (EdgeNode *)malloc(sizeof(EdgeNode));
            fread(&newEdge->adjVex, sizeof(int), 1, fp);
            fread(&newEdge->weight, sizeof(int), 1, fp);
            fread(&newEdge->iflock, sizeof(isLocked), 1, fp);
            newEdge->next = NULL;

            if (G->head[i] == NULL) {
                G->head[i] = newEdge;
                tail = newEdge;
            } else {
                tail->next = newEdge;
                tail = newEdge;
            }
        }
    }

    fclose(fp);
    printf("历史建筑数据加载成功！\n");
    return 1; // 加载成功
}

// 永久删除空间（辅助7、9功能，可选实现）
void deleteSpace(Graph *G, int id) {
    if (id < 0 || id >= G->vexNum) return;
    // 1. 删除该空间的所有出边
    EdgeNode *p = G->head[id];
    while (p) {
        EdgeNode *tmp = p;
        p = p->next;
        free(tmp);
    }
    G->head[id] = NULL;

    // 2. 删除其他空间指向该空间的边
    for (int i = 0; i < G->vexNum; i++) {
        if (i == id) continue;
        EdgeNode *pre = NULL;
        EdgeNode *cur = G->head[i];
        while (cur) {
            if (cur->adjVex == id) {
                if (pre == NULL) {
                    G->head[i] = cur->next;
                } else {
                    pre->next = cur->next;
                }
                free(cur);
                G->edgeNum--;
                break;
            }
            pre = cur;
            cur = cur->next;
        }
    }

    // 3. 移位（将后续空间前移，保证id连续）
    for (int i = id; i < G->vexNum - 1; i++) {
        G->space[i] = G->space[i+1];
        G->head[i] = G->head[i+1];
        // 更新边中指向i+1的id为i
        for (int j = 0; j < G->vexNum; j++) {
            EdgeNode *q = G->head[j];
            while (q) {
                if (q->adjVex == i+1) {
                    q->adjVex = i;
                }
                q = q->next;
            }
        }
    }
    G->vexNum--; // 减少空间数量
}

// 永久删除边（辅助9功能，可选实现）
void deleteEdge(Graph *G, int u, int v) {
    if (u < 0 || u >= G->vexNum || v < 0 || v >= G->vexNum) return;
    EdgeNode *pre = NULL;
    EdgeNode *cur = G->head[u];
    while (cur) {
        if (cur->adjVex == v) {
            if (pre == NULL) {
                G->head[u] = cur->next;
            } else {
                pre->next = cur->next;
            }
            free(cur);
            G->edgeNum--;
            break;
        }
        pre = cur;
        cur = cur->next;
    }
}