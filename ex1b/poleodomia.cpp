#include <iostream>
#include <vector>

using namespace std;

int N;
long long buildings[1000000];

long long min_cost(int left, int right)
{
    if (left == right)
        return left;

    long long max = 0;
    long long cost_from_left = 0;
    long long cost_from_right = 0;
    for (int i = left; i <= right; i++)
    {
        if (buildings[i] > max)
            max = buildings[i];

        cost_from_left += max;
    }
    max = 0;
    long long max_index = 0;
    for (int i = right; i >= left; i--)
    {
         if (buildings[i] > max)
         {
            max = buildings[i];
            max_index = i;
         }

        cost_from_right += max;
    }

    if (cost_from_left < cost_from_right)
        right = max_index - 1;
    else
        left = max_index + 1;

    return min_cost(left, right);
}

int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N;

    int inp;
    for (int i = 0; i < N; i++)
    {
        cin >> inp;
        buildings[i] = (long long) inp;
    }

    int min_idx = min_cost(0, N - 1);
    int res = 0, max = 0;
    for (int i = min_idx; i <= N - 1; i++)
    {
        if (buildings[i] > max)
            max = buildings[i];

        res += max;
    }
    max = 0;
    for (int i = min_idx - 1; i >= 0; i--) // <--possible segfault
    {
        if (buildings[i] > max)
            max = buildings[i];
        
        res += max;
    }

    cout << res << "\n";

    return 0;
}
