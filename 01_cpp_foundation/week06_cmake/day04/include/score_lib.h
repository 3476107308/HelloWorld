#ifndef SCORE_LIB_H
#define SCORE_LIB_H

#include <vector>

namespace score
{

std::vector<int> sort_desc(const std::vector<int>& scores);

std::vector<int> filter_at_least(
    const std::vector<int>& scores,
    int pass_line);

int count_at_least(
    const std::vector<int>& scores,
    int pass_line);

}  // namespace score

#endif
