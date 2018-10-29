#include <iostream>
#include <vector>
#include <limits>

using namespace std;

typedef struct particle {
    int start;
    int velocity;
} particle;

int N, L, K;

vector<particle> a_particles;
vector<particle> b_particles;

double find_colision(particle a, particle b)
{
    return (L + a.velocity * a.start + b.velocity * b.start) / (a.velocity +  b.velocity);
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
            par = {start, velocity};

            if (i == 0) 
                a_particles.push_back(par);
            else
                b_particles.push_back(par);
        }
    }

/*     for (int i = 0; i < a_particles.size(); i++)
    {
        cout << a_particles[i].id << ", " << a_particles[i].start << ", " << a_particles[i].velocity << "\n";
    }
    for (int i = 0; i < b_particles.size(); i++)
    {
        cout << b_particles[i].id << ", " << b_particles[i].start << ", " << b_particles[i].velocity << "\n";
    } */

    for (int k = 0; k < K; k++)
    {
        int i = 0;
        int j = 0;
        double current_colision;
        double min = 10000;
        while ((current_colision = find_colision(a_particles[i], b_particles[j])) < min)
        {
            i++;
            min = current_colision;
        }
        i--;
        j++;
        while ((current_colision = find_colision(a_particles[i], b_particles[j])) < min)
        {
            j++;
            min = current_colision;
        }
        j--;
        
        cout << i + 1 << " " << j + 1 << "\n";

        a_particles.erase(a_particles.begin() + i);
        b_particles.erase(b_particles.begin() + j);
    }

    return 0;
}
