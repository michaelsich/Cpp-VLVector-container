#include <iostream>
#include "VLVector.hpp"

int main()
{

    VLVector<int,2> myVec;
    myVec.push_back(1);
    myVec.push_back(2);
    myVec.push_back(3);
    myVec.push_back(4);
    myVec.push_back(5);
    myVec.push_back(6);
    myVec.push_back(7);

    std::cout << "size: " << myVec.size() << std::endl;
    std::cout << "capacity: " << myVec.capacity() << std::endl << std::endl;
    int * data = myVec.data();
    std::cout << "{ ";
    for (int i = 0; i < 3; ++i)
    {
        std::cout << data[i] << ", ";
    }
    std::cout << " }" << std::endl;


    return 0;
}
