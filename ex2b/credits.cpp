// O(nlogn)

#include <iostream>
#include <vector>

//#define DEBUG

using namespace std;

int findCeilIndex(vector<int> &v, int key)
{
    int l = 0;
    int r = v.size() - 1;

    while (r - l > 1)
    {
        int m = l + (r - l) / 2;
        if (v[m] >= key)
            r = m;
        else
            l = m;
    }

    return r;
}

int findBottomIndex(vector<int> &v, int key)
{
    int l = 0;
    int r = v.size() - 1;

    while (r - l > 1)
    {
        int m = l + (r - l) / 2;
        if (v[m] < key)
            r = m;
        else
            l = m;
    }

    return r;
}

int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    size_t N;
    cin >> N;

    int *creds = new int[N];
    for (size_t i = 0; i < N; i++)
        cin >> creds[i];

    int *left = new int[N];
    vector<int> left_aggr;
    for (size_t i = 0; i < N; i++)
    {
        if (left_aggr.empty() || creds[i] > left_aggr[left_aggr.size() - 1])
            left_aggr.push_back(creds[i]);
        else
            left_aggr[findCeilIndex(left_aggr, creds[i])] = creds[i];

        left[i] = left_aggr.size();
    }

    int *right = new int[N];
    vector<int> right_aggr;
    for (size_t i = N; i > 0; i--)
    {
        if (right_aggr.empty() || creds[i - 1] < right_aggr[right_aggr.size() - 1])
            right_aggr.push_back(creds[i - 1]);
        else
            right_aggr[findBottomIndex(right_aggr, creds[i - 1])] = creds[i - 1];

        right[i - 1] = right_aggr.size();
    }

#ifdef DEBUG
    for (size_t i = 0; i < N; i++)
        cout << left[i] << ", ";
    
    cout << "\n\n";

    for (size_t i = 0; i < N; i++)
        cout << right[i] << ", ";
#endif

    int ret = 0;
    for (size_t i = 0; i < N - 1; i++)
    {
        int sum = left[i] + right[i + 1]; 
        if (sum > ret)
            ret = sum;
    }
    if (ret < left[N - 1])
        ret = left[N - 1];
    if (ret < left[0])
        ret = left[0];

    cout << ret << "\n";

    //delete [] creds;

    return 0;
}
