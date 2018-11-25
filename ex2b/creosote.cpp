#include <iostream>
#include <math.h>

using namespace std;

int N, a, b, c;

int delicacies[10000];
long long partial_sums[10000];

long long dp[10000];

int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> a >> b >> c;

    for (int i = 0; i < N; i++)
    {
        cin >> delicacies[i];
    }

    long long ongoing_sum = 0;
    for (int i = 0; i < N; i++)
    {
        ongoing_sum += delicacies[i];
        partial_sums[i] = ongoing_sum;
    }

    dp[0] = delicacies[0];
    for (int i = 1; i < N; i++)
    {
        long long max = 0;
        for (int j = 1; j < i; j++)
        {
            long long sums = partial_sums[i] - partial_sums[j - 1];
            long long curr = dp[j - 1] + a * pow(sums, 2) + b * sums + c;
            if (curr > max)
                max = curr;
        }
        dp[i] = max;
    }

    cout << dp[N - 1] << "\n";

    return 0;
}
