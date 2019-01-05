#include <iostream>

#define KEY 1000000103

using namespace std;

typedef struct {
    long long paths[102];
    int pilot;
    int x;
    int y;
} node;

node grid[180][180];
int N, M, K, X;

int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> M >> K >> X;

    grid[M - 1][N - 1].paths[0] = 1;

    int s, e, temp_x, temp_y;
    for (int i = 0; i < K; i++) {
        cin >> s >> e;
        temp_x = s / M;
        temp_y = s % M;
        grid[temp_y][temp_x].pilot = 1;
        grid[temp_y][temp_x].x = e / M;
        grid[temp_y][temp_x].y = e % M;
    }

    for (int i = M - 1; i >= 0; i--) {
        for (int j = N - 1; j >= 0; j--) {
            for (int k = 0; k <= X; k++) {
                int val = grid[i][j].paths[k] % KEY;

                if (grid[i][j].pilot == 1)
                    grid[grid[i][j].y][grid[i][j].x].paths[k + 1] += val;
                else {
                    if (i != 0)
                        grid[i - 1][j].paths[k] += val;
                    if (j != 0)
                        grid[i][j - 1].paths[k] += val;
                }
            }
        }
    }

    long long res = 0;
    for (int k = 0; k <= X; k++) {
        res += grid[0][0].paths[k] % KEY;
    }

    cout << res % KEY << "\n";

    return 0;
}
