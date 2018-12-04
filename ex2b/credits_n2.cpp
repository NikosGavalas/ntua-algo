#include <iostream>
#include <vector>

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

    int ret = 0;
    for (int i = 0; i < N; i++)
    {
        int len_dec = 1;
        int prev = creds[i];
        for (int j = i; j >= 0; j--)
        {
            if (creds[j] < prev)
            {
                prev = creds[j];
                len_dec++;
            } 
        }
        int len_inc = 1;
        prev = creds[i + 1];
        for (int j = i + 1; j < N; j++)
        {
            if (creds[j] > prev)
            {
                prev = creds[j];
                len_inc++;
            }
        }
        if (len_inc + len_dec > ret)
            ret = len_dec + len_inc;
    }

    cout << ret << "\n";

    //delete [] creds;

    return 0;
}
