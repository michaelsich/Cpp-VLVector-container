#include <iostream>
#include "VLVector.hpp"

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

    VLVector<int,2> myVec(myVec2);

    std::cout << "size: " << myVec.size() << std::endl;
    std::cout << "capacity: " << myVec.capacity() << std::endl << std::endl;
    std::cout << "{ ";
    for (int i = 0; i < myVec.size(); ++i)
    {
        std::cout <<     myVec.at(i) << ", ";
    }
    std::cout << " }" << std::endl;

    myVec.pop_back();
    myVec.pop_back();
    myVec.pop_back();
    myVec.pop_back();
    std::cout << "--------------------------------------" << std::endl;

    std::cout << "size: " << myVec.size() << std::endl;
    std::cout << "capacity: " << myVec.capacity() << std::endl << std::endl;
    std::cout << "{ ";
    for (int i = 0; i < myVec.size(); ++i)
    {
        std::cout <<     myVec.at(i) << ", ";
    }
    std::cout << " }" << std::endl;


    return 0;
}
