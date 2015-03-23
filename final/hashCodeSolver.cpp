
#include <iostream>
#include <iomanip>

int main()
{
    std::cout << "Google Hash Code Challenge 2015- Team A211" << std::endl;
    std::cout << std::endl;
    std::cout << "hello world" << std::endl;

    int i;
    double e=1.0;
    double facto=1;

    for(i=1;i<=1000;i++){
        facto=facto*i;
        e+=1/facto;
    }

    std::cout<<"Une approximation du nombre de Neper : "<<std::setprecision(10)<<e<<std::endl<<std::endl;

    return 0;

}



