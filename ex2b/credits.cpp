#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    cin >> N;

    int *creds = new int[N];
    for (int i = 0; i < N; i++)
        cin >> creds[i];

    int *dp = new int[N];
    for (int i = 0; i < N; i++)
        dp[i] = 0;

    int max_cred = creds[N - 1];
    for (int i = N - 2; i >= 0; i--)
    {
        if (creds[i] > max_cred)
            max_cred = creds[i];

        dp[i] = max(dp[i + 1], max_cred - creds[i]);
    }

    int min_cred = creds[0];
    for (int i = 1; i < N; i++)
    {
        if (creds[i] < min_cred)
            min_cred = creds[i];

        dp[i] = max(dp[i - 1], dp[i] + creds[i] - min_cred);
    }

    cout << dp[N - 1] << "\n";

    //delete [] creds;
    //delete [] dp;

    return 0;
}
