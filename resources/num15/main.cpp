#include "node.h"

node* execute() {
    // OPEN 为空
    if (OPEN.size() < 1) {
        return NULL;
    }
    
    // 获得估价最小的节点
    node *now = get();
    
    // 删除 OPEN 表中元素
    vector<node*>::iterator it;
    for (it = OPEN.begin(); it != OPEN.end(); it++) {
        if (*it == now) {
            OPEN.erase(it);
            break;
        }
    }

    // 放入 CLOSE 表
    CLOSE.push_back(now);

    // 若抵达目标
    if (*now == target) return now;
    
    // 扩展该节点
    now->expand();

    cout << "OPEN: " << OPEN.size() << "\tCLOSE: " << CLOSE.size() << '\n';

    // 递归调用
    node*n = execute(); 
    
    // 返回目标
    if(n) return n;

    // 查找失败
    return NULL;
}

int main() {
    OPEN.clear();
    CLOSE.clear();
    node *st = new node(start);
    OPEN.push_back(st);
    return output(execute());
}