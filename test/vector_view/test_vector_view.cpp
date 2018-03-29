#include <iostream>
#include <initializer_list>

#include <donny/vector_view.hpp>

int main()
{
    using namespace donny;

    int a[] = { 9, 8, 7, 6, 5 };
    vector_view<const int> vv = { 1, 2, 3, 4, 5 };
    vector_view<int> vv2(a);

    std::cout << "Printing \"vector_view<const int> vv = { 1, 2, 3, 4, 5 };\"" << std::endl;
    for (int a : vv)
        std::cout << a << std::endl;

    std::cout << "Adding one and printing \"vector_view<int> vv2(a);\"" << std::endl;
    for (int &a : vv2)
        std::cout << ++a << std::endl;

    std::cout << "Printing again \"vector_view<int> vv2(a);\"" << std::endl;
    for (int a : vv2)
        std::cout << a << std::endl;

    return 0;
}
