/*
    Failed attempt to solve the problem using the Convex Hull trick.
    I don't think this solution is feasible. 
*/

#include <iostream>
#include <vector>

using namespace std;

typedef struct particle {
    int id;
    int start;
    int velocity;
} particle;

int N, L, K;

vector<particle> a_particles;
vector<particle> b_particles;

double find_colision(particle a, particle b)
{
    return ((double) (L + a.velocity * a.start + b.velocity * b.start)) / (a.velocity +  b.velocity);
}

// see http://wcipeg.com/wiki/Convex_hull_trick
int main(int argc, char const *argv[])
{
    // faster I/O
    // see https://www.geeksforgeeks.org/fast-io-for-competitive-programming/
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> N >> L >> K;

    int start, velocity;
    particle par;
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> start >> velocity;
            par = {j + 1, start, velocity};

            if (i == 0)
                a_particles.push_back(par);
            else
                b_particles.push_back(par);
        }
    }

    for (int k = 0; k < K; k++)
    {
        int indexes[2] = {0, 0};
        int min_indexes[2] = {0, 0};
        int i = 0;
        double current_colision;
        double min = 10000000;
        while (indexes[0] < a_particles.size() && indexes[1] < b_particles.size())
        {
            current_colision = find_colision(a_particles[indexes[0]], b_particles[indexes[1]]);
            if (current_colision < min)
            {
                min = current_colision;
                min_indexes[0] = indexes[0];
                min_indexes[1] = indexes[1];
            }
            else
            {
                i++;
            }

            indexes[i & 1]++;
        }
        
        cout << a_particles[min_indexes[0]].id << " " << b_particles[min_indexes[1]].id << "\n";

        a_particles.erase(a_particles.begin() + min_indexes[0]);
        b_particles.erase(b_particles.begin() + min_indexes[1]);
    }

    return 0;
}
