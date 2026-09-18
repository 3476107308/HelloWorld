#ifndef SCORE_DEBUG_H
#define SCORE_DEBUG_H

#include <vector>

namespace score_debug
{

// Returns nullptr when target is absent from scores.
const int* find_score(const std::vector<int>& scores, int target);

}  // namespace score_debug

#endif
