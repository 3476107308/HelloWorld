#include "score_lib.h"

#include <iostream>
#include <vector>

void print_scores(
    const std::vector<int>& scores,
    const char* title)
{
    std::cout << title << ": ";
    for (int score : scores)
    {
        std::cout << score << ' ';
    }
    std::cout << '\n';
}

int main()
{
    const std::vector<int> scores{65, 48, 91, 60, 77, 59};
    const int pass_line = 60;

    // TODO 1: 调用 score::sort_desc，输出降序成绩。
    std::vector<int> sorted = score::sort_desc(scores);
    print_scores(sorted,"降序输出");
    // TODO 2: 调用 score::filter_at_least，输出及格成绩。
    std::vector<int> least = score::filter_at_least(scores,pass_line);
    print_scores(least,"及格成绩");
    // TODO 3: 调用 score::count_at_least，输出及格人数。
    int count = score::count_at_least(scores,pass_line);
    // 要求：这里只包含 score_lib.h，不要包含 score_lib.cpp。
    std::cout << "及格人数" << count << std::endl;
    (void)scores;
    (void)pass_line;
    return 0;
}
