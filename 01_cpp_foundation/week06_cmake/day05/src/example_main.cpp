#include "score_debug.h"

#include <iostream>
#include <vector>

void print_found_score(const int* score)
{
    // Intentionally unsafe: this function is used only to demonstrate GDB.
    std::cout << "找到的成绩: " << *score << '\n';
}

int main()
{
    const std::vector<int> scores{72, 45, 90, 61, 88};

    const int* existing_score = score_debug::find_score(scores, 90);
    print_found_score(existing_score);

    const int* missing_score = score_debug::find_score(scores, 100);
    print_found_score(missing_score);

    return 0;
}
