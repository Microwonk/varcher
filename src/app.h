#pragma once

#include <vector>

const std::vector<char> types = {
        'T', // TRIANGLE
        'V', // VOXEL
        'G', // GALAXY
        'F' // FRACTAL
};

auto run(char rendererType) -> int;
