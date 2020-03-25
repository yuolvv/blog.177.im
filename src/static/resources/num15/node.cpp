#include "node.h"
#include <fstream>

int start[4][4] = { { 11,9,4,15 },
{ 1,3,0,12 },
{ 7,5,8,6 },
{ 13,2,10,14 } };

int target[4][4] = { { 1,2,3,4 },
{ 5,6,7,8 },
{ 9,10,11,12 },
{ 13,14,15,0 } };

int trans[4][2] = { { -1,0 },{ 1,0 },{ 0,1 },{ 0,-1 } };

void findPosition(int ori[4][4],int v,int* p) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (ori[i][j] == v) {
                p[0] = i;
                p[1] = j;
                return;
            }
        }
    }
}

node::node(){
    m_Childern.clear();
    m_Parent = NULL;
    m_H = m_G = 0;
}

node::node(int from[4][4]) {
    m_Childern.clear();
    m_Parent = NULL;
    m_G = 0;
    this->init(from);
    calcH();
}

node::node(int from[4][4], node* parent) {
    m_Childern.clear();
    this->init(from);
    this->m_Parent = parent;
    if (parent) calcG();
    else m_G = 0;
    calcH();
}

void node::init(int from[4][4]) {
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            m_State[j][i] = from[j][i];
        }
    }
    findZero();
}

void node::calcG() {
    if (m_Parent) m_G = m_Parent->m_G + 1;
    else m_G = 0;
}

void node::calcH(){
    m_H = 0;
    // 计算当前状态中不在位数码相对目标位置的曼哈顿距离和
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int v = m_State[i][j];
            int tp[2];
            findPosition(target, v, tp);
            m_H += abs(tp[0] - i) + abs(tp[1] - j);
        }
    }
}

void node::findZero(int state[4][4],int pos[2]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (state[i][j] == 0) {
                pos[0] = i;
                pos[1] = j;
                break;
            }
        }
    }
}

void node::findZero(){
    node::findZero(m_State,m_Zero);
}

int node::getPrice() {
    return m_H + m_G;
}

bool node::check(int t[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (m_State[i][j] != t[i][j])
                return false;
        }
    }
    return true;
}

void node::expand() {
    for (int index = 0; index < 4; index++) {
        int newZP[2] = { m_Zero[0] + trans[index][0],m_Zero[1] + trans[index][1] };
        if (newZP[0] < 0 || newZP[0]>3 || newZP[1] < 0 || newZP[1]>3) continue;

        // 新的数码位置
        int newState[4][4];

        // 初始化为当前状态
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                newState[i][j] = m_State[i][j];
            }
        }

        // 交换 m_Zero 与 newZP 两个位置的数
        int t = newState[m_Zero[0]][m_Zero[1]];
        newState[m_Zero[0]][m_Zero[1]] = newState[newZP[0]][newZP[1]];
        newState[newZP[0]][newZP[1]] = t;

        // 排除与父节点相同的节点
        if (this->m_Parent && *(this->m_Parent) == newState) continue;

        node* P = findP(newState);
        if (!P) {
            // 新建节点并加入 OPEN
            node *newN = new node(newState, this);
            OPEN.push_back(newN);
            m_Childern.push_back(newN);
        }
        else {
            // 已存在，若当前节点代价更小，更改已存在的节点指针及其子节点代价
            node *newN = new node(newState, this);
            if (P->getPrice() > newN->getPrice()) {
                // 修改 P 的父节点为当前节点
                P->m_Parent = this;
                P->calcG();
                updateAll(P);
            }
            else {
                delete newN;
                newN = NULL;
            }
        }
    }
}

void print(int t[4][4]) {
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            cout << t[j][i] << ' ';
        }
        cout << '\n';
    }
    cout << '\n';
}

int output(node* n) {
    if (n) {
        //TODO: 输出路径
        cout << "GOT!\n";
        cout << "OPEN: " << OPEN.size() << "\tCLOSE: " << CLOSE.size() << '\n';
        cout << "G: " << n->m_G << "\tH: " << n->m_H << '\n';
        node *t = n;
        ofstream ofs("out.txt");
        while (t)
        {
            for (int j = 0; j < 4; j++) {
                for (int i = 0; i < 4; i++) {
                    ofs << (t->m_State)[j][i] << ' ';
                }
                ofs << '\n';
            }
            ofs << '\n';
            t = t->m_Parent;
        }
        ofs.close();
    }
    else {
        cout << "No path found!\n";
    }
    return 0;
}

vector<node*>OPEN = vector<node*>();
vector<node*>CLOSE = vector<node*>();

int findInverseNum(int s[4][4]){
    int num = 0;
    int *state = (int*)s;
    for (int i = 0; i < 16; i++){
        if (state[i] == 0) continue;
        for (int j = 0; j < i; j++){
            if (state[j] > state[i]) num++;
        }
    }
    return num;
}

node* get() {
    node* n = NULL;
    int f = LONG_MAX;
    for (int i = 0; i < OPEN.size(); i++) {
        if (OPEN[i]->getPrice() < f) {
            f = OPEN[i]->getPrice();
            n = OPEN[i];
        }
    }
    return n;
}

void updateAll(node* n){
    if (n->m_Childern.size() < 1)  return;
    vector<node*>::iterator it;
    for (it = n->m_Childern.begin(); it != n->m_Childern.end(); it++) {
        (*it)->calcG();
        // 递归调用本函数，更新其所有后继节点的g，h不便
        updateAll(*it);
    }
}

node * findP(int t[4][4])
{
    vector<node*>::iterator it;
    for (it = OPEN.begin(); it != OPEN.end(); it++) {
        if (*(*it) == t) return *it;
    }
    for (it = CLOSE.begin(); it != CLOSE.end(); it++) {
        if (*(*it) == t) return *it;
    } 
    return NULL;
}