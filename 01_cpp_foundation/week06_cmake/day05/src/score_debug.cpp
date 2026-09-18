#include "score_debug.h"

namespace score_debug
{

const int* find_score(const std::vector<int>& scores, int target)
{
    for (const int& score : scores)
    {
        if (score == target)
        {
            return &score;
        }
    }

    return nullptr;
}

}  // namespace score_debug
