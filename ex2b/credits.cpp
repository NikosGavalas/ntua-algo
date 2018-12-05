// O(nlogn)

#include <iostream>
#include <vector>
#include <algorithm>

//#define DEBUG

using namespace std;

vector<int> creds(500000);
vector<int> left_lis(500000);
vector<int> right_lis(500000);

int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    size_t N;
    cin >> N;

    int read;
    for (size_t i = 0; i < N; i++)
    {
        cin >> read;
        creds[i] = read;
    }

    vector<int> left_aggr;
    left_aggr.push_back(creds[0]);
    left_lis[0] = 1;
    for (size_t i = 1; i < N; i++)
    {
        if (creds[i] > left_aggr[left_aggr.size() - 1])
            left_aggr.push_back(creds[i]);
        else
        {
            auto it = upper_bound(left_aggr.begin(), left_aggr.end(), creds[i]); 
            *it = creds[i]; 
        }
        left_lis[i] = left_aggr.size();
    }

    vector<int> right_aggr;
    right_aggr.push_back(creds[N - 1]);
    right_lis[N - 1] = 1;
    for (size_t i = N - 1; i > 0; i--)
    {
        if (creds[i - 1] < right_aggr[right_aggr.size() - 1])
            right_aggr.push_back(creds[i - 1]);
        else
        {
            auto it = lower_bound(right_aggr.begin(), right_aggr.end(),
                creds[i - 1], greater<int>()); 
            *it = creds[i - 1];
        }
        right_lis[i - 1] = right_aggr.size();
    }

#ifdef DEBUG
    for (size_t i = 0; i < N; i++)
        cout << left_lis[i] << ", ";
    
    cout << "\n";

    for (size_t i = 0; i < N; i++)
        cout << right_lis[i] << ", ";
    
    cout << "\n";
#endif

    int ret = 0;
    for (size_t i = 0; i < N - 1; i++)
    {
        int sum = left_lis[i] + right_lis[i + 1]; 
        if (sum > ret)
            ret = sum;
    }

    cout << ret << "\n";

    return 0;
}
