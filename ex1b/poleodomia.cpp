// O(n) solution
// code is ugly I know, I am in a hurry

#include <iostream>
#include <limits>
#include <stack>

using namespace std;

long long buildings[1000000];
long long costs[1000000];
int N;

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

    stack<pair<long long, int>> stack_left;
    stack_left.push(make_pair(buildings[0], 1));
    costs[0] = buildings[0];
    long long sum = buildings[0];
    for (int i = 1; i < N; i++)
    {
        pair<int, int> pr;
        if (buildings[i] > buildings[i - 1])
        {
            int popped_count = 1;
            while (!stack_left.empty() && stack_left.top().first < buildings[i])
            {
                pr = stack_left.top();
                stack_left.pop();
                sum -= pr.first * pr.second;
                popped_count += pr.second;
            }
            stack_left.push(make_pair(buildings[i], popped_count));
            sum += buildings[i] * popped_count;
        }
        else if (buildings[i] == buildings[i - 1])
        {
            pr = stack_left.top();
            stack_left.pop();
            stack_left.push(make_pair(pr.first, pr.second + 1));
            sum += buildings[i];
        }
        else
        {
            stack_left.push(make_pair(buildings[i], 1));
            sum += buildings[i];
        }
        costs[i] += sum;
    }

    // I enjoy typing
    stack<pair<long long, int>> stack_right;
    stack_right.push(make_pair(buildings[N - 1], 1));
    costs[N - 1] += buildings[N - 1];
    sum = buildings[N - 1];
    for (int i = N - 2; i >= 0; i--)
    {
        pair<int, int> pr;
        if (buildings[i] > buildings[i + 1])
        {
            int popped_count = 1;
            while (!stack_right.empty() && stack_right.top().first < buildings[i])
            {
                pr = stack_right.top();
                stack_right.pop();
                sum -= pr.first * pr.second;
                popped_count += pr.second;
            }
            stack_right.push(make_pair(buildings[i], popped_count));
            sum += buildings[i] * popped_count;
        }
        else if (buildings[i] == buildings[i + 1])
        {
            pr = stack_right.top();
            stack_right.pop();
            stack_right.push(make_pair(pr.first, pr.second + 1));
            sum += buildings[i];
        }
        else
        {
            stack_right.push(make_pair(buildings[i], 1));
            sum += buildings[i];
        }
        costs[i] += sum;
    }

    long long min_cost = INT64_MAX;
    for (int i = 0; i < N; i++)
    {
        costs[i] -= buildings[i];
        if (costs[i] < min_cost)
            min_cost = costs[i];
    }

    cout << min_cost << "\n";

    return 0;
}
