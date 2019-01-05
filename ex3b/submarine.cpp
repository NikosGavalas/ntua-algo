// O(N * M * K)

#include <iostream>
#include <utility>

#define KEY 1000000103

using namespace std;

long long grid[180][180][101];
bool is_passage[180][180];
pair<int, int> exits[180][180];
int N, M, K, X;

int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> M >> K >> X;

    grid[M - 1][N - 1][0] = 1;

    // sexy solution
    int s, e, x, y;
    for (int i = 0; i < K; i++) {
        cin >> s >> e;

        x = s / M;
        y = s % M;
        is_passage[y][x] = true;
        exits[y][x] = make_pair(e / M, e % M);
    }

    for (int i = M - 1; i >= 0; i--) {
        for (int j = N - 1; j >= 0; j--) {
            for (int k = 0; k <= X; k++) {
                int paths = grid[i][j][k] % KEY;

                if (is_passage[i][j]) {
                    auto ext = exits[i][j];
                    grid[ext.second][ext.first][k + 1] += paths;
                }
                else {
                    if (i != 0)
                        grid[i - 1][j][k] += paths;
                    if (j != 0)
                        grid[i][j - 1][k] += paths;
                }
            }
        }
    }

    long long res = 0;
    for (int k = 0; k <= X; k++) {
        res += grid[0][0][k] % KEY;
    }

    cout << res % KEY << "\n";

    return 0;
}
