#ifndef SPACE_GRAPH_H
#define SPACE_GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VEX 100
#define MAX_EDGE 500
#define IF 0x3f3f3f3f

//展厅、院落、门洞、楼梯和安全出口
typedef enum {
    HALL,TARD,DOOR,STARIR,EXIT
}SpaceType;

typedef enum {
    LOCK,UNLOCK
}isLocked;

//空间节点
typedef struct SpaceNode{
    int id;
    char name[20];
    int floor;
    SpaceType type;
    isLocked iflock;
}SpaceNode;

//边结点
typedef struct EdgeNode {
    int adjVex;
    int weight;
    isLocked iflock;
    struct EdgeNode *next;
}EdgeNode;

//图
typedef struct Graph {
    SpaceNode space[MAX_VEX];
    EdgeNode *head[MAX_VEX];
    int vexNum;
    int edgeNum;
    int maxfloor;
}Graph;

void initGraph(Graph *G);

void buildGraph(Graph *G);
void addSpace(SpaceNode *newSpace,int id,int *maxfloor);
int findSpace(Graph *G,char *name);
void addEdge(Graph *G);

int isSpaceExit(Graph *G,char *name);
int isEdgeExit(Graph *G,int u,int v);

void lockSpace(Graph *G);
void unlockSpace(Graph *G);

void lockEdge(Graph *G);
void unlockEdge(Graph *G);

// 保存图数据到文件
void saveGraph(Graph *G, const char *filename);
// 从文件加载图数据
int loadGraph(Graph *G, const char *filename);
// 永久删除空间（辅助函数）
void deleteSpace(Graph *G, int id);
// 永久删除边（辅助函数）
void deleteEdge(Graph *G, int u, int v);

#endif
