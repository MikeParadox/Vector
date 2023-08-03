#include "../includes/Vector.h"
#include <iostream>
#include <vector>


int main()
{
    std::vector<int> vec{1, 2, 3, 4, 5};
    std::cout << "space is " << vec.capacity() << ' ' << " size is "
              << vec.size() << '\n';
    vec.reserve(10);
    std::cout << "After reserve(): \n";
    std::cout << "space is " << vec.capacity() << ' ' << " size is "
              << vec.size() << '\n';
    for (const auto& i : vec)
    {
        std::cout << i << ' ';
    }
    std::cout << '\n';


    std::cout << "After resize(): \n";
    vec.resize(11);
    std::cout << "space is " << vec.capacity() << ' ' << " size is "
              << vec.size() << '\n';
    for (const auto& i : vec)
    {
        std::cout << i << ' ';
    }

    return 0;
}


// TODO mark all the needed member functions (including constructors) constexpr
// TODO mark all the needed functions noexcept
// TODO implement swap
