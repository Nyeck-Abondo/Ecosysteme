#include <Core/Structs.h>
#include <iostream>
#include <windows.h>

int main() {
    SetConsoleOutputCP(CP_UTF8);
    Ecosystem::Core::Vector2D vect1 (10, 20);
    std::cout << std::endl;
    Ecosystem::Core::Vector2D vect2 (5, 12);
    std::cout << std::endl;
    std::cout << "Distance :" << vect1.Distance(vect2) << std::endl;
    Ecosystem::Core::Color couleur =Ecosystem::Core::Color::Red();
    std::cout <<"Couleur RGBA :("<< static_cast<int> (couleur.r)<<", "<<static_cast<int> (couleur.g)<<", "
                <<static_cast<int> (couleur.b) << ", "<<static_cast<int> (couleur.a)<<")"<< std::endl;
    return 0;
}