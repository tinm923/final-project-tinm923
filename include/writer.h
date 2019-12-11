#pragma once 

#include <fstream>
#include <iomanip>
#include <vector>

void write_data(const std::vector<std::vector<int>> &sandpile)
{
    std::ofstream ofs("sandpile.dat");
    ofs << std::fixed << std::setprecision(8);

    std::size_t size = sandpile.size();
    std::size_t number_of_particles = sandpile[0].size();
    for (std::size_t i = 0; i < size; ++i)
    {
        for (std::size_t j = 0; j < number_of_particles; ++j)
        {
            ofs << sandpile[i][j];
        }
        ofs << std::endl;
    }
}
/*
Add two more cases accepting argv to make size configurable
*/