// O(nlogn)

#include <iostream>
#include <vector>
#include <algorithm>

#define DEBUG

using namespace std;

vector<int> creds(500000);
vector<int> left_lis(500000);
vector<int> right_lis(500000);

int LongestIncreasingSubsequenceLength(std::vector<int>& v) 
{ 
    if (v.size() == 0) 
        return 0; 
  
    std::vector<int> tail(v.size(), 0); 
    int length = 1; 
  
    tail[0] = v[0]; 
    for (size_t i = 1; i < v.size(); i++) { 
        if (v[i] > tail[length - 1]) 
            tail[length++] = v[i]; 
        else { 
            auto it = find(tail.begin(), tail.begin() + length, v[i]); 
            if (it != tail.begin() + length) { 
                continue; 
            } 
            it = upper_bound(tail.begin(), tail.begin() + length, v[i]); 
            *it = v[i]; 
        } 
    } 
  
    return length; 
}

int main(int argc, char const *argv[])
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    size_t N;
    cin >> N;

    int read;
    for (size_t i = 0; i < N; i++)
    {
        cin >> read;
        creds.push_back(read);
    }

    vector<int> left_aggr;
    left_aggr.push_back(creds[0]);
    for (size_t i = 1; i < N; i++)
    {
        if (creds[i] > left_aggr[left_aggr.size() - 1])
            left_aggr.push_back(creds[i]);
        else
        {
            // TO check whether the element is not present before hand 
            /* auto it = find(left_aggr.begin(), left_aggr.end(), creds[i]); 
            if (it != left_aggr.end()) { 
                continue; 
            }  */
            // If not present change the tail element to v[i] 
            auto it = upper_bound(left_aggr.begin(), left_aggr.end(), creds[i]); 
            *it = creds[i]; 
        }
        left_lis[i] = left_aggr.size();
    }

#ifdef DEBUG
    for (size_t i = 0; i < N; i++)
        cout << left_lis[i] << ", ";
    
    cout << "\n";

/*     for (size_t i = 0; i < N; i++)
        cout << left_lis[i] << ", ";
    
    cout << "\n"; */
#endif

    //delete [] creds;

    return 0;
}
