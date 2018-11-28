#include <vector>
#include <algorithm>

int CalcMaxValue(const std::vector<std::vector<int>>& weight,
                 const std::vector<std::vector<int>>& value,
                 int max_weight) {

    if (weight.empty())
        return 0;

    std::vector<int> last(max_weight + 1, -1);
    for (int i = 0; i < weight[0].size(); ++i) {
        if (weight[0][i] < max_weight)
            last[weight[0][i]] = std::max(last[weight[0][i]], value[0][i]);
    }

    std::vector<int> current(max_weight + 1);
    for (int i = 1; i < weight.size(); ++i) {
        std::fill(current.begin(), current.end(), -1);
        for (int j = 0; j < weight[i].size(); ++j) {
            for (int k = weight[i][j]; k <= max_weight; ++k) {
                if (last[k - weight[i][j]] > 0)
                    current[k] = std::max(current[k],
                                          last[k - weight[i][j]] + value[i][j]);
            }
        }
        std::swap(current, last);
    }
    
    return *std::max_element(last.begin(), last.end());
}

int main(void) {
    // insert driver code here
    return 0;
}
