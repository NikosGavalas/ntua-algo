#include <iostream>
#include <vector>
#include <sstream>
#include <tuple>
#include <algorithm>

//#define DEBUG

using namespace std;

int N, L, K;

enum Team {A, B};

class Particle 
{
public:
    int id;
    Team team;
    int start;
    int velocity;

    Particle(int id, Team team, int start, int velocity)
    {
        this->id = id;
        this->team = team;
        this->start = start;
        this->velocity = velocity;
    }

    string to_string()
    {
        ostringstream stream;
        stream << "{ " << id << ", " << team << ", " << start << ", " << \
            velocity << "}" << "\n";
        return stream.str();
    }

    // O(1)
    int position_at_time(int T)
    {
        if (this->team == A)
            return this->velocity * (T - this->start);
        return (this->velocity * (this->start - T)) + L;
    }
};

vector<Particle*>* a_particles;
vector<Particle*>* b_particles;

// O(n)
Particle* find_furthest(vector<Particle*>* particles, int T)
{
    Team team = particles->at(0)->team;
    Particle* furthest_particle = particles->at(0);
    int distance_of_furthest = furthest_particle->position_at_time(T);
    for (Particle* current_particle : *particles)
    {   
        int current_distance = current_particle->position_at_time(T);
        if (team == A ? current_distance > distance_of_furthest \
                      : current_distance < distance_of_furthest)
        {
            furthest_particle = current_particle;
            distance_of_furthest = current_distance;
        }
    }
    return furthest_particle;
}

// O(n)
tuple<Particle*, Particle*> furthest_particles(vector<Particle*>* a_particles, \
                                               vector<Particle*>* b_particles, int T)
{
    return make_tuple(find_furthest(a_particles, T), find_furthest(b_particles, T));
}

tuple<Particle*, Particle*> binarySearch(vector<Particle*>* a_particles, \
                                         vector<Particle*>* b_particles, int T)
{
    int t1 = 0, t2 = T, mid;
    tuple<Particle*, Particle*> ret;
    while (t1 <= t2)
    {
        mid = t1 + ((t2 - t1) / 2);
        ret = furthest_particles(a_particles, b_particles, mid);
        int distance = get<0>(ret)->position_at_time(mid) - get<1>(ret)->position_at_time(mid);
        if (distance == 0)
            break;
        if (distance > 0)
            t2 = mid - 1;
        if (distance < 0)
            t1 = mid + 1;
    }
    return ret;
}                                         

int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);   
    
    cin >> N >> L >> K;

    a_particles = new vector<Particle*>(0);
    b_particles = new vector<Particle*>(0);

    int start, velocity;
    for (int i = 0; i < N; i++)
    {
        cin >> start >> velocity;
        Particle* par = new Particle(i + 1, A , start, velocity);
        a_particles->push_back(par);
    }
    for (int i = 0; i < N; i++)
    {
        cin >> start >> velocity;
        Particle* par = new Particle(i + 1, B, start, velocity);
        b_particles->push_back(par);
    }

#ifdef DEBUG
    for (Particle* par : *a_particles)
        cout << par->id << "\n";

    for (Particle* par : *b_particles)
        cout << par->id << "\n";
#endif

    // find the time of first collision
    for (int k = 0; k < K; k++)
    {
        tuple<Particle*, Particle*> pair = binarySearch(a_particles, b_particles, L);
        cout << get<0>(pair)->id << " " << get<1>(pair)->id << "\n";
        a_particles->erase(remove(a_particles->begin(), a_particles->end(), get<0>(pair)), \
                           a_particles->end());
        b_particles->erase(remove(b_particles->begin(), b_particles->end(), get<1>(pair)), \
                           b_particles->end());
    }

    return 0;
}
