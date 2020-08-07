#include <iostream>
#include "VLVector.hpp"
#include <vector>

int main()
{

    VLVector<int,2> myVec2;
    myVec2.push_back(1);
    myVec2.push_back(2);
    myVec2.push_back(3);
    myVec2.push_back(4);
    myVec2.push_back(5);
    myVec2.push_back(6);
    myVec2.push_back(7);

    VLVector<int,2> myVec = myVec2;

    std::cout << "size: " << myVec.size() << std::endl;
    std::cout << "capacity: " << myVec.capacity() << std::endl << std::endl;
    std::cout << "{ ";
    for (int i : myVec)
    {
        std::cout << i << ", ";
    }
    std::cout << " }" << std::endl;

    auto it1 = myVec.begin();
    auto it2 = myVec.end();
    it1 += 3;
    it2 -= 3;
//    std::vector<int> v = {0};

    it1 = myVec.erase(it1, it2);

    std::cout << "--------------------------------------" << std::endl;
    std::cout << "size: " << myVec.size() << std::endl;
    std::cout << "capacity: " << myVec.capacity() << std::endl << std::endl;
    std::cout << "{ ";
    for (int i : myVec)
    {
        std::cout << i << ", ";
    }
    std::cout << " }" << std::endl;

    std::cout << "'it' points: "<< *it1 << std::endl;

    return 0;
}
