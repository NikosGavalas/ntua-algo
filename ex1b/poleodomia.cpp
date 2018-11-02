#include <iostream>
#include <vector>
#include <limits>

using namespace std;

int N;
long long buildings[1000000];

long long calculate_cost(int index)
{
    long long res = 0, max = 0;
    for (int i = index; i <= N - 1; i++)
    {
        if (buildings[i] > max)
            max = buildings[i];

        res += max;
    }
    max = 0;
    for (int i = index - 1; i >= 0; i--)
    {
        if (buildings[i] > max)
            max = buildings[i];

        res += max;
    }

    return res;
}

int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N;

    long long inp;
    for (int i = 0; i < N; i++)
    {
        cin >> inp;
        buildings[i] = inp;
    }

    long long min_cost = INT64_MAX;
    long long cost = 0;
    for (int i = 0; i < N; i++)
    {
        cost = calculate_cost(i);
        if (cost < min_cost)
            min_cost = cost;
    }

    cout << min_cost << "\n";

    return 0;
}
