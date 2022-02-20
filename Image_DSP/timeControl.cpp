#include "timeControl.hpp"

timeControl::timeControl()
{
    start = std::chrono::high_resolution_clock::now();
}

timeControl::~timeControl()
{
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;

    std::cout << "Total time: " << duration.count() * 1000.0f << "ms\n";
    //Timer::printTimer(3);
}

float timeControl::get_time()
{
    current = std::chrono::high_resolution_clock::now();
    duration = current - start;
    return duration.count() * 1000.0f;
}

void timeControl::printTimer(int modo)
{
    float constant = 0.0;
    std::string sufix;



    switch (modo)
    {
    case 1: constant = 1000.0f;
        sufix = "ms";
        break;
    case 2: constant = 1000000.0f;
        sufix = "us";
        break;
    case 3:  constant = 1.0f;
        sufix = "s";
        break;

    default: constant = 1.0f;
    }

    std::cout << "Time stamp: " << this->duration.count() * constant << sufix << "\n";
}