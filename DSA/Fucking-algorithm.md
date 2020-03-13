# Fucking-algorithm

[toc]

Notes based https://labuladong.gitbook.io/algo/; https://github.com/labuladong/fucking-algorithm

## 0. 必读

### 学习算法和刷题的思路指南

#### 数据结构的存储方式

只有两种

- 数组（顺序存储）
- 链表（链式存储）

比如说「队列」、「栈」这两种数据结构既可以使用链表也可以使用数组实现。用数组实现，就要处理扩容缩容的问题；用链表实现，没有这个问题，但需要更多的内存空间存储节点指针。

「图」的两种表示方法，邻接表就是链表，邻接矩阵就是二维数组。邻接矩阵判断连通性迅速，并可以进行矩阵运算解决一些问题，但是如果图比较稀疏的话很耗费空间。邻接表比较节省空间，但是很多操作的效率上肯定比不过邻接矩阵。

「散列表」（hash table）就是通过散列函数把键映射到一个大数组里。而且对于解决散列冲突的方法，拉链法需要链表特性，操作简单，但需要额外的空间存储指针；线性探查法就需要数组特性，以便连续寻址，不需要指针的存储空间，但操作稍微复杂些。

「树」，用数组实现就是「堆」，因为「堆」是一个完全二叉树，用数组存储不需要节点指针，操作也比较简单；用链表实现就是很常见的那种「树」，因为不一定是完全二叉树，所以不适合用数组存储。为此，在这种链表「树」结构之上，又衍生出各种巧妙的设计，比如二叉搜索树、AVL 树、红黑树、区间树、B 树等等，以应对不同的问题。

数组与链表的优缺点：

- **数组**由于是紧凑连续存储,可以随机访问，通过索引快速找到对应元素，而且相对节约存储空间。但正因为连续存储，内存空间必须一次性分配够，所以说数组如果要扩容，需要重新分配一块更大的空间，再把数据全部复制过去，时间复杂度 $O(N)$；而且你如果想在数组中间进行插入和删除，每次必须搬移后面的所有数据以保持连续，时间复杂度 $O(N)$。
- **链表**因为元素不连续，而是靠指针指向下一个元素的位置，所以不存在数组的扩容问题；如果知道某一元素的前驱和后驱，操作指针即可删除该元素或者插入新元素，时间复杂度 $O(1)$。但是正因为存储空间不连续，你无法根据一个索引算出对应元素的地址，所以不能随机访问；而且由于每个元素必须存储指向前后元素位置的指针，会消耗相对更多的储存空间。

#### 数据结构的基本操作

基本操作：遍历+访问；增删查改

遍历+访问无非两种形式：线性和非线性的。线性就是for/while迭代为代表，非线性就是递归为代表。

*数组遍历，典型线性*

```c++
void traverse(int[] arr) {
    for (int i = 0; i < arr.length; ++i)
        // 迭代访问
}
```

*链表遍历，兼具迭代和递归*

```c++
struct ListNode {
    int val;
    ListNode* next;
}
void traverse(ListNode* head) {
    for (ListNode* p = head; p != nullptr; p = p->next) {
        // 迭代访问
    }
}
void traverse(ListNode* head) {
    travere(head->next);
}
```

*二叉树遍历，典型的非线性递归遍历*

```c++
struct TreeNode {
    int val;
    TreeNode* left, *right;
}
void traverse(TreeNode* root) {
    traverse(root->left);
    traverse(root->right);
}
```

二叉树遍历与链表的递归遍历相似。

二叉树框架可以扩展为N叉树的遍历框架

```c++
struct TreeNode {
    int val;
    TreeNode*[] children;
}
void traverse(TreeNode* root) {
    for (TreeNode* child: root->children)
        traverse(child);
}
```

N 叉树的遍历又可以扩展为图的遍历，因为图就是好几 N 叉棵树的结合体。你说图是可能出现环的？这个很好办，用个布尔数组 visited 做标记就行了

#### 算法刷题指南

**数据结构是工具，算法是通过合适的工具解决特定问题的方法**

**先刷二叉树(binary tree)**; **只要涉及递归的问题，都是树的问题**。

### 学习数据结构和算法读什么书

算法4

例子：二分图；套汇

### 动态规划详解

**动态规划问题的一般形式就是求最值**。求解动态规划的核心问题是穷举。

动态规划

- 存在*重叠子问题*。需要“备忘录”或者“DP table”来优化
- 具有*最优子结构*，能通过子问题的最值求得原问题的最值
- 只有列出正确的*状态转移方程*才能正确地穷举

明确「状态」 -> 定义 dp 数组/函数的含义 -> 明确「选择」-> 明确 base case。

#### 斐波那契数列

```C++
int fib(int N) {
    if (N == 1 || N == 2)
        return 1;
    return fib(N-1) + fib(N-2);  // O(2^n)
}
```

```c++
int fib(int N) {
    if (N < 1)
        return 0;
    vector<int> memo(N+1, 0);
    return helper(memo, N);
}
int helper(vector<int>& memo, int n) {
    if (n == 1 || n == 2)
        return 1;
    if (memo[n] != 0)
        return memo[n];
    memo[n] = helper(memo, n-1) + helper(memo, n-2);  // O(n)
    return memo[n];
}
```

这种方法叫做「自顶向下」，动态规划叫做「自底向上」。

```c++
int fib(int N) {
    vector<int> dp(N+1, 0);  // dp table
    dp[1] = dp[2] = 1;
    for (int i = 3; i <= N; ++i)
        dp[i] = dp[i-1] + dp[i-2];  // state transition function
    return dp[N];
}
```

状态转移方程直接代表着暴力解法。

**千万不要看不起暴力解，动态规划问题最困难的就是写出状态转移方程**，即这个暴力解。优化方法无非是用备忘录或者 DP table，再无奥妙可言。

```c++
int fib(int n) {
    if (n == 1 || n == 2)
        return 1;
    int prev =1, curr = 1;
    for (int i = 3; i <= n; ++i) {
        int sum = prev + curr;
        prev = curr;
        curr = sum;
    }
    return curr;
}
```

上面code降低了空间复杂度为$O(1)$

#### 凑零钱问题

题目：给你 `k` 种面值的硬币，面值分别为 `c1, c2 ... ck`，每种硬币的数量无限，再给一个总金额 `amount`，问你**最少**需要几枚硬币凑出这个金额，如果不可能凑出，算法返回 -1 

```c++
int coinChange(int[] coins, int amount);  // signature; coins: options; amount: final amount
```

要符合「最优子结构」，子问题间必须互相独立

思考**如何列出正确的状态转移方程**？

- **先确定「状态」**，也就是原问题和子问题中变化的变量。由于硬币数量无限，所以唯一的状态就是目标金额 `amount`。
- **然后确定** **`dp`** **函数的定义**：当前的目标金额是 `n`，至少需要 `dp(n)` 个硬币凑出该金额。
- **然后确定「选择」并择优**，也就是对于每个状态，可以做出什么选择改变当前状态。具体到这个问题，无论当的目标金额是多少，选择就是从面额列表 `coins` 中选择一个硬币，然后目标金额就会减少：

```python
def coinChange(coins, amount: int):
    def dp(n):
        # base case
        if n == 0:
            return 0
        if n < 0:
            return -1
        res = float('INF')
        for coin in coins:
            subproblem = dp(n-coin)
            if subproblem == -1:
                continue
            res = min(res, 1+subproblem)
        return res if res != float('INF') else -1
    return dp(amount)
```

以上，时间复杂度是$O(k*n^k)$

```python
def coinChange(coins, amount: int):
    # memory table
    memo = dict()
    def dp(n):
        # base case
        if n in memo:
            return memo[n]
        if n == 0:
            return 0
        if n < 0:
            return -1
        res = float('INF')
        for coin in coins:
            subproblem = dp(n-coin)
            if subproblem == -1:
                continue
            res = min(res, 1+subproblem)
        memo[n] = res if res != float('INF') else -1
        return memo[n]
    return dp(amount)
```

自底而上

```c++
int coinChange(vector<int>& coins, int amount) {
    vector<int> dp(amount+1, amount+1);
    // base case
    dp[0] = 0;
    for (int i = 0; i < dp.size(); ++i) {
        // inner for for each subproblem + 1
        for (int coin: coins) {
            // subproblem no solution
            if (i - coin < 0) continue;
            dp[i] = min(dp[i], 1 + dp[i-coin]);
        }
    }
    return (dp[amount] == amount +1) ? -1 : dp[amount];
}
```

**计算机解决问题其实没有任何奇技淫巧，它唯一的解决办法就是穷举**，穷举所有可能性。算法设计无非就是先思考“如何穷举”，然后再追求“如何聪明地穷举”。

### 动态规划答疑篇

#### 最优子结构详解

「最优子结构」是某些问题的一种特定性质，并不是动态规划问题专有的。

最优子结构并不是动态规划独有的一种性质，能求最值的问题大部分都具有这个性质；**但反过来，最优子结构性质作为动态规划问题的必要条件，一定是让你求最值的**。碰到最值题，思路往动态规划想就对了，套路

找最优子结构的过程，其实就是证明状态转移方程正确性的过程，方程符合最优子结构就可以写暴力解了，写出暴力解就可以看出有没有重叠子问题了，有则优化，无则 OK

#### dp数组的遍历方向

- **1、遍历的过程中，所需的状态必须是已经计算出来的**。
- **2、遍历的终点必须是存储结果的那个位置**。

### 回溯算法详解

**解决一个回溯问题，实际上就是一个决策树的遍历过程**。你只需要思考 3 个问题：

1. 路径：也就是已经做出的选择。
2. 选择列表：也就是你当前可以做的选择。
3. 结束条件：也就是到达决策树底层，无法再做选择的条件。

#### 全排列问题

**前序遍历的代码在进入某一个节点之前的那个时间点执行，后序遍历代码在离开某个节点之后的那个时间点执行**。

**回溯算法的一个特点，不像动态规划存在重叠子问题可以优化，回溯算法就是纯暴力穷举，复杂度一般都很高**。

#### N皇后问题

给你一个 N×N 的棋盘，让你放置 N 个皇后，使得它们不能互相攻击。

PS：皇后可以攻击同一行、同一列、左上左下右上右下四个方向的任意单位。

```c++
vector<vector<string>> res;

// input: board length; output: all legal placement
vector<vector<string>> solveNQueens(int n) {
    // '.' means empty; 'Q' stands for Queen
    vector<string> board(n, string(n, '.'));
    backtrack(board, 0);
    return res;
}

// path: all rows with smaller index than row has Queens already
// options: all colummns in row to put Queen
// ending: row exceeds the last rows of the board
void backtrack(vector<string>& board, int row) {
    // triggering ending condition
    if (row = board.size()) {
        res.push_back(board);
        return;
    }
    int n = board[row].size();
    for (int col = 0; col < n; ++col) {
        // exclude illegal choice
        if (!isValid(board, row, col))
            continue;
        // make choice
        board[row][col] = 'Q';
        // entering next decision
        backtrack(board, row+1);
        // withdraw choice
        board[row][col] = '.';
    }
}

bool isValid(vector<string>& board, int row, int col) {
    int n = board.size();
    // check queen conflict in column
    for (int i = 0; i < n; ++i) {
        if (board[i][col] == 'Q')
            return false;
    }
    // check conflict in right-upper
    for (int i = row-1, j = col + 1; i >=0 && j < n; i--, j++) {
        if (board[i][j] == 'Q')
            return false;
    }
    // check conflict in left-upper
    for (int i = row-1, j = col-1; i >=0 && j >= 0; i--, j--) {
        if (board[i][j] == 'Q')
            return false;
    }
    return true;
}
```

**有的时候，我们并不想得到所有合法的答案，只想要一个答案，怎么办呢**？比如解数独的算法，找所有解法复杂度太高，只要找到一种解法就可以。

```c++
// 函数找到一个答案后就返回 true
bool backtrack(vector<string>& board, int row) {
    // 触发结束条件
    if (row == board.size()) {
        res.push_back(board);
        return true;
    }
    ...
    for (int col = 0; col < n; col++) {
        ...
        board[row][col] = 'Q';
        if (backtrack(board, row + 1))
            return true;
        board[row][col] = '.';
    }

    return false;
}
```

#### 总结

回溯算法就是个多叉树的遍历问题，关键就是在前序遍历和后序遍历的位置做一些操作

**写** **`backtrack`** **函数时，需要维护走过的「路径」和当前可以做的「选择列表」，当触发「结束条件」时，将「路径」记入结果集**。

某种程度上说，动态规划的暴力求解阶段就是回溯算法。只是有的问题具有重叠子问题性质，可以用 dp table 或者备忘录优化，将递归树大幅剪枝，这就变成了动态规划。

### 二分查找详解

二分查找真正的坑在于到底要给 `mid` 加一还是减一，while 里到底用 `<=` 还是 `<`。

最常用的二分查找场景：寻找一个数、寻找左侧边界、寻找右侧边界

**分析二分查找的一个技巧是：不要出现 else，而是把所有情况用 else if 写清楚，这样可以清楚地展现所有细节**。

#### 寻找一个数（基本）

```c++
int binarySearch(vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;
    while (left <= right) {
        int mid = left + (right-left)/2;
        if (nums[mid] == target)
            return mid;
        else if (nums[mid] < target)
            left = mid + 1;
        else if (nums[mid] > target)
            right = mid - 1;
    }
    return -1;
}
```

明确了「搜索区间」这个概念; 左闭右开，还是左闭右闭

#### 寻找左侧边界的二分搜索

```c++
int left_bound(vector<int>& nums, int target) {  // how many smaller than target; not finding value
    if (nums.size() == 0)
        return -1;
    int left = 0;
    int right = nums.size();  // note
    
    while (left < right) {  // note
        int mid = left + (right - left)/2;
        if (nums[mid] == target)
            right = mid;
        else if (nums[mid] < target)
            left = mid+1;
        else if (nums[mid] > target)
            right = mid;  // note
    }
    return left;  // return nums[left] == target ? left : -1;
}
```

对于搜索左右侧边界的二分查找，这种写法比较普遍

**返回** **`left`** **, ** **`right`**都是一样的，因为 while 终止的条件是 `left == right`

寻找右侧边界的二分搜索

```c++
int right_bound(vector<int>& nums, int target) {
    if (nums.size() == 0)
        return -1;
    int left = 0;
    int right = nums.size();
    
    while (left < right) {
        int mid = left + (right -left)/2;
        if (nums[mid] == target)
            left = mid+1;  // note
        else if (nums[mid] < target)
            left = mid+1;
        else if (nums[mid] > target)
            right = mid;
    }
    return right - 1;
}
```

again, finaly left == right

#### 逻辑统一

```c++
int binary_search(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1; 
    while(left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1; 
        } else if(nums[mid] == target) {
            // 直接返回
            return mid;
        }
    }
    // 直接返回
    return -1;
}

int left_bound(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1;
        } else if (nums[mid] == target) {
            // 别返回，锁定左侧边界
            right = mid - 1;
        }
    }
    // 最后要检查 left 越界的情况
    if (left >= nums.size() || nums[left] != target)
        return -1;
    return left;
}


int right_bound(vector<int>& nums, int target) {
    int left = 0, right = nums.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (nums[mid] < target) {
            left = mid + 1;
        } else if (nums[mid] > target) {
            right = mid - 1;
        } else if (nums[mid] == target) {
            // 别返回，锁定右侧边界
            left = mid + 1;
        }
    }
    // 最后要检查 right 越界的情况
    if (right < 0 || nums[right] != target)
        return -1;
    return right;
}
```

**总结**

1. 分析二分查找代码时，不要出现 else，全部展开成 else if 方便理解
2. 如果将「搜索区间」全都统一成两端都闭，好记，只要稍改 `nums[mid] == target` 条件处的代码和返回的逻辑即可

### 双指针技巧总结

分为两类，一类是「快慢指针」，一类是「左右指针」。前者解决主要解决链表中的问题，比如典型的判定链表中是否包含环；后者主要解决数组（或者字符串）中的问题，比如二分查找。

#### 快慢指针的常见算法

判断是否含有环

```c++
bool hasCycle(listNode* head) {
    listNode* fast, *slow;
    fast = slow = head;
    while (fast != nullptr && fast->next != nullptr) {
        fast = fast->next->next;
        slow = slow->next;
        
        if (fast == slow)
            return true;
    }
    return false;
}
```

已知有环，返回这个环的起始位置

```c++
listNode* detectCycle(listNode* head) {
    listNode* fast, *slow;
    fast = slow = head;
    while (fast != nullptr && fast->next != nullptr) {
        fast = fast->next->next;
        slow = slow->next;
        if (fast == slow)
            break;
    }
    // any pointer goes back to head
    slow = head;
    while (slow != fast) {
        fast = fast->next;
        slow = slow->next;
    }
    return slow;  // meet again at the starting point of the cycle
}
```

寻找链表的中点

```c++
while (fast != nullptr && fast->next != nullptr) {
    fast = fast->next->next;
    slow = slow->next;
}
return slow;
```

当链表的长度是奇数时，slow 恰巧停在中点位置；如果长度是偶数，slow 最终的位置是中间偏右

寻找链表中点的一个重要作用是对链表进行归并排序。对于链表，合并两个有序链表是很简单的，难点就在于二分。



寻找列表的倒数第K个元素

```c++
listNode* slow, *fast;
slow = fast = head;
while (k-- > 0)
    fast = fast->next;
whiel (fast != nullptr) {
    slow = slow->next;
    fast = fast->next;
}
return slow;
```



## 1. 动态规划

## 2. 数据结构

## 3. 算法思维

## 4. 高频面试

## 5. 计算机技术