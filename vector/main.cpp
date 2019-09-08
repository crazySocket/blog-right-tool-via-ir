#include <vector>

void foo(std::vector<int> v)
{
    for(int i=0; i<v.size(); i++)
    {
        v[i] += 1;
    }
}

int main()
{
    foo({1,2,3});
    return 0;
}
