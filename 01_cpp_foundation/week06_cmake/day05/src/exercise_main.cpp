#include "score_debug.h"

#include <iostream>
#include <vector>

int main()
{
    const std::vector<int> scores{66, 73, 81, 95};
    const int target = 100;

    // TODO 1: 调用 score_debug::find_score 查找 target，并保存返回值。
    // TODO 2: 使用 GDB 确认找不到 target 时返回的指针值。
    // TODO 3: 编写一个明确的空指针检查；找不到时输出一条提示，不能解引用空指针。
    // TODO 4: 再将 target 改为 scores 中存在的值，验证两个分支。

    (void)scores;
    (void)target;
    std::cout << "请完成 TODO 后使用 GDB 调试。\n";
    return 0;
}
