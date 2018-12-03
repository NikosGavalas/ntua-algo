// O(nlogn)

#include <iostream>
#include <vector>

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
        if (v[m] <= key)
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
    for (size_t i = N - 1; i >= 0; i--)
    {
        if (right_aggr.empty() || creds[i] < right_aggr[right_aggr.size() - 1])
            right_aggr.push_back(creds[i]);
        else
            right_aggr[findBottomIndex(right_aggr, creds[i])] = creds[i];

        right[i] = right_aggr.size();
    }

    for (size_t i = 0; i < N; i++)
        cout << left[i] << ", ";
    cout << "\n";

    for (size_t i = 0; i < N; i++)
        cout << right[i] << ", ";
    cout << "\n";

    //delete [] creds;

    return 0;
}
