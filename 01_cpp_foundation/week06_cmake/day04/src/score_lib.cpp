#include "score_lib.h"

#include <algorithm>
#include <iterator>

namespace score
{

std::vector<int> sort_desc(const std::vector<int>& scores)
{
    std::vector<int> result = scores;
    std::sort(result.begin(), result.end(), [](int left, int right) {
        return left > right;
    });
    return result;
}

std::vector<int> filter_at_least(
    const std::vector<int>& scores,
    int pass_line)
{
    std::vector<int> result;
    std::copy_if(
        scores.begin(),
        scores.end(),
        std::back_inserter(result),
        [pass_line](int score) {
            return score >= pass_line;
        });
    return result;
}

int count_at_least(
    const std::vector<int>& scores,
    int pass_line)
{
    return static_cast<int>(std::count_if(
        scores.begin(),
        scores.end(),
        [pass_line](int score) {
            return score >= pass_line;
        }));
}

}  // namespace score
