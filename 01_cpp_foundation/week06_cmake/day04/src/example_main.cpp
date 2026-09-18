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
    const std::vector<int> scores{72, 45, 90, 61, 88, 59, 100};
    const std::vector<int> sorted = score::sort_desc(scores);
    const std::vector<int> passed = score::filter_at_least(scores, 60);

    print_scores(sorted, "降序成绩");
    print_scores(passed, "及格成绩");
    std::cout << "及格人数: "
              << score::count_at_least(scores, 60) << '\n';
    return 0;
}
