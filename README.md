# Connect-Four

### 实验目的

*   实现四子棋AI，并尽量使其更加智能。
*   以四子棋博弈游戏为背景，通过剪枝算法、棋局评价的改进，锻炼对人工智能算法的实际应用能力。

### 实验基础

*   极小极大搜索过程
    *   极小极大搜索方法是博弈树搜索的基本方法，其主要思想是考虑双方对弈若干步之后（即在有限的搜索深度范围内进行求解），从可能的走步中选一步相对最佳的走。
    *   为此要定义一个静态估计函数 F，F 的选取相当于棋手对棋局判断的能力，因此，F 的选取是影响 AI 智能与否的一个很大因素。
*   α-β 搜索过程
    *   α-β 是对极小极大搜索过程的一个重要剪枝优化。
    *   α(β) 剪枝：若任一极小(大)值层节点的 β(α) 值小于(大于)或等于它任一先辈极大(小)值居节点的 α(β) 值则没有必要再进行该 MIN(MAX) 节点以下的搜索过程。

### 简单实现

*   首先根据极小极大搜索过程、α-β 剪枝优化实现了初级版本，其中极大搜索过程代码如下：

    ```c++
    int SearchMax(int depth, int alpha, int beta) {	//	DFS
        if (depth >= maxDepth) return F(2) - F(1);	//	超出深度限制
        int maxval = -INF * INF;
        for (int y = 0; y < n; y++) {
            int &&x = Top(y);	if (x == -1) continue; 	//	如果当前列已满
                myboard[x][y] = 2;
                if (machineWin(x, y, m, n, myboard)) {
                  	//	若落子后赢了，则返回∞，同时应该取步数小的，可以乘一个系数
                    myboard[x][y] = 0;
                    return INF * (1 + maxDepth - depth);
                }
                int &&val = SearchMin(depth + 1, max(alpha, maxval), beta);
                myboard[x][y] = 0;	//	回溯
                if (val > maxval) {
                    maxval = val;
                    if (maxval >= beta) return maxval;       //	β 剪枝
                }}}
        return maxval;}
    ```

* 局面评估函数 F 的设计

    *   考虑二连子、三连子的个数，并且乘以相应的权值，这样可以促进自己的 AI 连成更多的连子，同时趋向于打断对方的连子，并且在“长”与“多”之间通过参数的选取达到一个最佳的平衡。是一种基础的做法。

* 初级版本实现完成，经测试发现搜索深度为4不超时，即推测各方各下2步，与100对弈，胜率约30%

### 简单优化

*   开始几步落子时，应尽量落在中间，而不是在两边。
*   评估函数的优化，首先，两头不为对方棋子并且不是禁手，这样的连子才是有意义的，因而增加这一判断。
*   考虑到必胜策略都有所不同，应该仅将步数更少而必胜的策略视为必胜策略，否则，举个反面的极端例子，若每次取步数最多的必胜策略，最终有可能会因为评估函数的不准确而陷入失败。
*   随着深度的增加，α-β 剪枝可以更加灵活，比如接近剪枝条件就进行剪枝，而由于棋盘太小、深度不深，剪枝的效果不大，因此不采用该剪枝。

### 进一步优化

*   评估函数 F 优化
    *   在调试过程中通过输出棋盘信息发现，常常出现“凹”字形的形状，这是由于一方落子后另一方再落子则胜导致的，而打破这种僵局只有等到其余地方无棋可下。
    *   因此，注意到评估函数中两头空的判断，随着棋局的进行，在一僵局的“凹槽”会越来越深，而显然，凹槽两边的棋子不应该被当作一边空的有效的棋子，而应视为堵住的棋子，所以增加这一判断条件，即旁边的空位距离底部不能太远，这样的空位才有利于局势。
    *   经试验认为该优化有效。
*   时间控制优化
    *   单步决策有时间限制3s。调试发现如果设定固定的搜索深度，当接近时限时强制返回，这种策略导致的错误积累可能是致命的，因为其损失了很多搜索的广度。例如：在某列的搜索空间特别大，所有时间几乎都耗费在了这一分支，然而，这与没有进行搜索直接下在这一列是没有区别的。
    *   搜索的广度必须得到充分保证，而若将时间平均分配，由于一定有某些列几步内是必败的，这会造成很大浪费。
    *   采用自适应的深度，即若搜索过程中用时达到时限的一半，则将深度限制 -1，若整个搜索结束后用时过少，则将深度限制+1，以使下次充分利用时间。
    *   经过这样的负反馈，搜索广度得到了保证，并且实际用时稳定在一个可以接受的范围内。
    *   经试验认为该优化很有效。
    *   考虑到 clock() 的调用常数较大，因此实际并不每次都判断。
*   试探性搜索优化
    *   由于 α-β 剪枝利用的特性，想到这样一个优化方案：如果先搜索最优解可能产生的位置，以产生相对较好的解，应该可能最大程度地把之后的搜索剪枝！！
    *   因此，先设定搜索深度为 3，进行试探性的搜索，按此结果进行排序，然后进行原来的搜索。
    *   由于上面的时间控制优化，相当于剪枝的同时自动加深了搜索深度，理论上会得到更好的结果。
    *   经过试验认为该优化很有效。


### α-β 剪枝 实验效果

*   使用 Compete 对2-100.dylib进行测试，按照计分规则，约得到80~90分。



### 蒙特卡洛树搜索（最终提交）

*   由于对 α-β 剪枝进行上述优化后总体效果并不突出，正好 DDL 延期，于是决定再写一个蒙特卡洛树搜索，（报告稍有省略）。

### 实验基础

*   蒙特卡洛规划
*   信心上限算法

>   信心上限树算法(UCT)
>
>     function UCTSEARCH(𝑠0)
>       以状态𝑠0创建根节点𝑣0;
>       while 尚未用完计算时长 do:
>         𝑣𝑙 ←TREEPOLICY(𝑣0);
>         ∆←DEFAULTPOLICY(s(𝑣𝑙 ));
>         BACKUP(𝑣𝑙 , ∆);
>       end while
>       return 𝑎(BESTCHILD(𝑣0, 0));
>
>     function TREEPOLICY(𝑣)
>       while 节点𝑣不是终止节点 do:
>         if 节点𝑣是可扩展的 then:
>           return EXPAND(𝑣)
>         else:
>           𝑣 ← BESTCHILD(𝑣, 𝑐)
>       return 𝑣
>
>     function EXPAND(𝑣)
>       选择行动𝑎 ∈ 𝐴(𝑠𝑡𝑎𝑡𝑒(𝑣))中尚未选择过的行动
>       向节点𝑣添加子节点𝑣′，使得𝑠(𝑣′)= 𝑓(𝑠(𝑣), 𝑎), 𝑎(𝑣′) = 𝑎
>       return 𝑣′
>
>     function BESTCHILD(𝑣, 𝑐)
>       return 𝑎𝑟𝑔𝑚𝑎𝑥𝑣′∈𝑐h𝑖𝑙𝑑𝑟𝑒𝑛 𝑜𝑓 𝑣 (𝑄(𝑣′) + 𝑐√2𝑙𝑛(𝑁(𝑣)))
>
>     function DEFAULTPOLICY(𝑠)
>       while 𝑠不是终止状态 do:
>         以等概率选择行动𝑎 ∈ 𝐴(𝑠)
>         𝑠 ← 𝑓(𝑠, 𝑎)
>         return 状态𝑠的收益
>
>     function BACKUP(𝑣, Δ)
>       while 𝑣 ≠ 𝑁𝑈𝐿𝐿 do:
>         𝑁(𝑣) ← 𝑁(𝑣) + 1
>         𝑄(𝑣) ← 𝑄(𝑣) + ∆
>         ∆← −∆
>         𝑣 ← 𝑣的父节点

*   学习了 “蒙特卡洛树搜索.pdf” 讲义中相关的部分，核心为如上引用的算法框架。

### 简单优化

*   信心上限索引中，常数 C 的值可选取，经测试比较后选 C = 0.8

### 进一步优化

*   状态存储
    *   显然，每个节点保存整个棋盘是不合理的，会产生大量的冗余。考虑每个节点只保存所下的点位和玩家，这样可以最大限度的减小存储的空间和拷贝的时间，通过状态的转移得到完整棋盘；但是，在直到访问到叶子节点之前，每次走这些路径都需要把棋盘一步步更新下来，若搜索深度较深，会导致不必要的浪费。
    *   因此，我考虑的是状态压缩，对每个节点存储 state[], top[] 两个一位 u_short 数组，state[]中用二进制，0 代表 user，1 代表 machine 表示棋盘的状态，可以用位运算 &获取二维棋盘上的状态，用 top[] 存储棋盘顶部的位置，以区分 user 和空位。这样拷贝就可以使用 memcpy()，由于操作的是连续内存且只做一次，认为不比每次下来都更新棋盘上的一两个点慢多少。
    *   同时，由于 top[] 数组的存在，使得找可下点位可以在线性时间内完成。由于在 Expand(), DefaultPolicy() 中都需要多次用到找可下点，其实这个改善不小。（如果用传递棋盘的方法，要实现此优化，也可另外维护并传递 top[] 数组）
    *   另外，由于记录了完整的状态并且做了压缩，更加适合更进一步优化——哈希，即按不同顺序走到相同的棋盘状态所做的模拟应该是可以共用的（充分利用马尔可夫性），但由于哈希回溯处理上的复杂性，没有加入这个优化。
*   扩展方案
    *   使用顺序的扩展而不是随机的扩展并不影响结果。
    *   考虑 Expand()，即一个节点扩展的方案，得益于 top[] 数组的存在，找到一个可扩展的子节点（即可下点位）只需关于棋盘长宽的线性时间，实际上使用合适的技巧可以只需均摊O(1) 时间：从左到右找可下点，之后从上一步的选择点开始即可。

### 更进一步优化

*   搜索树的高效维护
    *   针对信心上限算法决策的特点，每次都选择模拟次数最多的那个走，因此步与步之间的联系常常远比 1/棋盘宽度 大，应充分利用这一点。
    *   因此每次不应该重新建树，而是采用 MoveRoot()，将根根据上一步决策“移动”下来，完全保留之前在此模拟的结果，也省去了很多建立节点的时间。
    *   同时，为了防止内存泄漏以及需要析构的结点都堆积在最后一步，采用边移动根边析构的办法：找到根需要移动的子节点后，将其其余兄弟节点析构，再将根移动下来，可将析构压力均分到各步。
    *   实际实现中，由于析构时间无法控制，可将上一步的决策保存下来，在下一步开始前执行移动根并析构的操作，以充分并安全地利用时限。
*   无用分枝的花费最小化
    *   注意到搜索树的非主要分支往往包含必胜、必败局面，即已经是终态，由于信心上界算法要求，之后必然还会有模拟到达他们之上，而当模拟到达他们上时，这些节点由于无法进一步扩展，于是再模拟一遍自己，这时应避免重复计算。
    *   进一步的优化可以直接将这些无用的终态删去（其实是赋一很大的值而不是 -delta，以使算法对他们彻底失去信心，将更多的访问给更有用的节点），还可参考 α-β 剪枝的思路优化更多的相关分支。

### MCTS 实验效果

*   采用上述优化，采用 2s 时限，最终与 100.dylib 对战的胜率约 85%～95%，总胜率约96%。

### 代码框架

-   由于是凭兴趣尝试第二种方法，因此写代码过程中更注重解耦以提高代码的可复用性，并不追求优化常数。

```c++
class BoardState : public MonteCarloSearchState
{
public:
	BoardState(bool player);
    BoardState(const BoardState &predstate, int x, int y);
	virtual ~BoardState();
	std::pair<int, int> GetPutLocation();
    MonteCarloSearchState *NextChildState() override;
    int DefaultPolicy() override;
	void Print() override;
};

class BoardTreeNode : public MonteCarloSearchTreeNode
{
public:
    BoardTreeNode(MonteCarloSearchState *state, MonteCarloSearchTreeNode *parent) : MonteCarloSearchTreeNode(state, parent)
    {}
    MonteCarloSearchTreeNode *Expand() override;
};

class BoardTree : public MonteCarloSearchTree
{
	float UCB1(MonteCarloSearchTreeNode *parent, MonteCarloSearchTreeNode *child);
	MonteCarloSearchTreeNode *BestChild(MonteCarloSearchTreeNode *parent);
	void BackTrace(MonteCarloSearchTreeNode *node, int value);
public:
	static time_t StartTime;
    BoardTree(bool player) : MonteCarloSearchTree(new BoardTreeNode(new BoardState(player), nullptr))
    {}
    MonteCarloSearchTreeNode *TreePolicy() override;
	void MoveRoot(std::pair<int, int> put);
	std::pair<int, int> MonteCarloTreeSearch();
};
```
