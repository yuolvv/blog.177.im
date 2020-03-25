#pragma once

#include <iostream>
#include <map>
#include <vector>

using namespace std;

extern int start[4][4];
extern int target[4][4];
extern int trans[4][2];

class node
{
public:
    int m_State[4][4];

    int m_Zero[2];
    
    node* m_Parent;
    
    vector<node*> m_Childern;
    
    int m_H; // 代价
    
    int m_G; // 深度

    node();

    node(int from[4][4]);

    node(int from[4][4], node* parent);

    void init(int from[4][4]);

    void calcH();

    void calcG();
   
    static void findZero(int state[4][4],int pos[2]);
    void findZero();

    int getPrice();

    void expand();

    bool check(int t[4][4]);

    // 重载操作符，判定节点是否等价
    bool operator==(node b) {
        return this->check(b.m_State);
    }

    bool operator==(int t[4][4]) {
        return this->check(t);
    }
};

extern vector<node*> OPEN;
extern vector<node*> CLOSE;

void print(int t[4][4]);
int output(node* n);
node* findP(int t[4][4]);
node* get();
void updateAll(node* n);