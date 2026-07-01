#include "space_graph.h"

int main()
{
    Graph G;
    initGraph(&G);
    int choice;// 用户选择的操作
    int tag=0;// 标记是否需要重新显示菜单
    int newbuild=0;// 标记是否导入了新的建筑
    int change=0;// 标记是否进行了更改
    while(1){
        if(tag==0){
        printf("=======智慧文旅系统=======\n");
        printf("===请输入你想进行的操作====\n");
        printf("1. 导入新的建筑\n");
        printf("2. 打开上一次的建筑\n");
        printf("3. 查询相邻空间\n");
        printf("4. 判断两空间是否连通\n");
        printf("5. 查询最短参观路径\n");
        printf("6. 暂时关闭某个展厅\n");
        printf("7. 永久关闭某个展厅\n");
        printf("8. 暂时关闭某条走廊\n");
        printf("9. 永久关闭某条走廊\n");
        printf("10.补添空间\n");
        printf("11.补添走廊\n");
        printf("12.查询当前建筑的所有空间和走廊\n");
        printf("0.退出系统\n");
        printf("==========================\n");
        }
        tag=0;
        scanf("%d",&choice);
        if(tag>=6&&tag<=11)change=1;
        switch(choice){
            case 1:{
                newbuild=1;
                buildGraph(&G);
                change=1;
                break;
            }
            case 2:{
                // 加载历史建筑数据（指定固定文件名，如"graph_data.bin"）
                int loadRes = loadGraph(&G, "graph_data.bin");
                if (loadRes) {
                    newbuild = 1; // 标记为已导入建筑
                    change = 0;   // 加载后未修改
                }
                break;
            }
            case 3:{
                char name[20];
                printf("请输入要查询的空间名称：\n");
                scanf("%s", name);
                int id = findSpace(&G, name);
                if (id == -1) {
                    printf("该空间不存在！\n");
                    break;
                }
                printf("空间 %s 的相邻空间（未关闭）：\n", name);
                EdgeNode *p = G.head[id];
                int hasNeighbor = 0;
                while (p) {
                    // 仅显示未锁定的空间和未锁定的走廊
                    if (G.space[p->adjVex].iflock == UNLOCK && p->iflock == UNLOCK) {
                        printf(" - %s（走廊长度：%d）\n", G.space[p->adjVex].name, p->weight);
                        hasNeighbor = 1;
                    }
                    p = p->next;
                }
                if (!hasNeighbor) {
                    printf("无可用的相邻空间！\n");
                }
                break;
            }
            case 4:{
                break;
            }
            case 5:{
                break;
            }
            case 6:{
                //暂时关闭某个展厅
                char name[20];
                printf("请输入要暂时关闭的展厅名称：\n");
                scanf("%s", name);
                int id = findSpace(&G, name);
                if (id == -1) {
                    printf("该展厅不存在！\n");
                } else {
                    G.space[id].iflock = LOCK;
                    printf("展厅 %s 已暂时关闭！\n", name);
                    change=1;
                }
                break;
            }
            case 7:{
                //永久关闭某个展厅(删除空间)
                char name[20];
                printf("请输入要永久关闭的展厅名称：\n");
                scanf("%s", name);
                int id = findSpace(&G, name);
                if (id == -1) {
                    printf("该展厅不存在！\n");
                } else {
                    deleteSpace(&G, id);
                    printf("展厅 %s 已永久关闭！\n", name);
                    change=1;
                }
                break;
            }
            case 8:{
                //暂时关闭某条走廊(锁定边)
                char uName[20], vName[20];
                printf("请输入走廊连接的第一个空间名称：\n");
                scanf("%s", uName);
                printf("请输入走廊连接的第二个空间名称：\n");
                scanf("%s", vName);
                int u = findSpace(&G, uName);
                int v = findSpace(&G, vName);
                if (u == -1 || v == -1) {
                    printf("空间不存在！\n");
                    break;
                }
                // 锁定 u->v 的边
                EdgeNode *p = G.head[u];
                while (p && p->adjVex != v) p = p->next;
                if (p) {
                    p->iflock = LOCK;
                    // 无向图，同时锁定 v->u 的边
                    EdgeNode *q = G.head[v];
                    while (q && q->adjVex != u) q = q->next;
                    if (q) q->iflock = LOCK;
                    printf("走廊 %s-%s 已暂时关闭！\n", uName, vName);
                    change=1;
                } else {
                    printf("该走廊不存在！\n");
                }
                break;
            }
            case 9:{
                //永久关闭某条走廊(删除边)
                char uName[20], vName[20];
                printf("请输入走廊连接的第一个空间名称：\n");
                scanf("%s", uName);
                printf("请输入走廊连接的第二个空间名称：\n");
                scanf("%s", vName);
                int u = findSpace(&G, uName);
                int v = findSpace(&G, vName);
                if (u == -1 || v == -1) {
                    printf("空间不存在！\n");
                    break;
                }
                deleteEdge(&G, u, v);
                deleteEdge(&G, v, u); //无向图，双向删除
                printf("走廊 %s-%s 已永久关闭！\n", uName, vName);
                change=1;
                break;
                break;
            }
            case 10:{
                //补添空间
                if (G.vexNum >= MAX_VEX) {
                    printf("空间数量已达上限，无法补添！\n");
                    break;
                }
                addSpace(&G.space[G.vexNum], G.vexNum, &G.maxfloor);
                G.vexNum++;
                change=1;
                break;
            }
            case 11:{
                //补添走廊
                if (G.edgeNum >= MAX_EDGE) {
                    printf("走廊数量已达上限，无法补添！\n");
                    break;
                }
                addEdge(&G);
                G.edgeNum++;
                change=1;
                break;
            }
            case 12:{
                // 查询当前建筑的所有空间和走廊
                printf("当前建筑的所有空间：\n");
                for (int i = 0; i < G.vexNum; i++) {
                    printf("  %s (类型: %d, 楼层: %d)\n", G.space[i].name, G.space[i].type, G.space[i].floor);
                }
                printf("当前建筑的所有走廊：\n");
                for (int i = 0; i < G.vexNum; i++) {
                    EdgeNode *p = G.head[i];
                    while (p) {
                        if (p->iflock == UNLOCK) { // 仅显示未锁定的走廊
                            printf("  %s - %s (长度: %d)\n", G.space[i].name, G.space[p->adjVex].name, p->weight);
                        }
                        p = p->next;
                    }
                }
                break;
            }
            case 0:{
                int op1=0,op2=0;
                if(newbuild==1){
                    printf("是否保存本次导入的建筑:\n");
                    printf("(注:保存此次建筑将会覆盖原有的建筑)\n");
                    printf("1.保存 2.不保存\n");
                    scanf("%d",&op1);
                    while(op1!=1&&op1!=2){
                        printf("输入错误,请重新输入!\n");
                        scanf("%d",&op1);
                    }
                }else if(change==1){
                    printf("是否保存本轮更改:\n");
                    printf("1.保存 2.不保存\n");
                    scanf("%d",&op2);
                    while(op2!=1&&op2!=2){
                        printf("输入错误,请重新输入!\n");
                        scanf("%d",&op2);
                    }
                }
                if(op1==1||op2==1){
                    saveGraph(&G, "graph_data.bin"); 
                }
                exit(0);
                break;
            }
            default:{
                printf("输入错误,请重新输入!\n");
                tag=1;
            }
        }
    }

    return 0;
}