#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int max_value(const vector<vector<int>>& weight,
              const vector<vector<int>>& value,
              int max_weight) {
    if (weight.empty())
        return 0;

    vector<int> last(max_weight + 1, -1);
    for (int i = 0; i < weight[0].size(); i++) {
        if (weight[0][i] < max_weight)
            last[weight[0][i]] = max(last[weight[0][i]], value[0][i]);
    }

    vector<int> current(max_weight + 1);
    for (int i = 1; i < weight.size(); ++i) {
        fill(current.begin(), current.end(), -1);
        for (int j = 0; j < weight[i].size(); j++) {
            for (int k = weight[i][j]; k <= max_weight; k++) {
                if (last[k - weight[i][j]] > 0)
                    current[k] = max(current[k],
                                     last[k - weight[i][j]] + value[i][j]);
            }
        }
        swap(current, last);
    }

    return *max_element(last.begin(), last.end());
}

// driver code
int main(int argc, char const* argv[]) {
    vector<int> values_class_1;
    values_class_1.push_back(2);
    values_class_1.push_back(3);

    vector<int> weights_class_1;
    weights_class_1.push_back(3);
    weights_class_1.push_back(4);

    vector<int> values_class_2;
    values_class_2.push_back(1);
    values_class_2.push_back(2);

    vector<int> weights_class_2;
    weights_class_2.push_back(2);
    weights_class_2.push_back(3);

    vector<vector<int>> values;
    values.push_back(values_class_1);
    values.push_back(values_class_2);
    vector<vector<int>> weights;
    weights.push_back(weights_class_1);
    weights.push_back(weights_class_2);

    int max_weight = 7;

    cout << max_value(weights, values, max_weight) << endl;

    return 0;
}
