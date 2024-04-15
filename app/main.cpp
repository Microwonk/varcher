#include <iostream>
#include <filesystem>
#include "app.h"

#define STANDARD_RENDERER 'T'

inline auto printArchetypes() {
    std::cout << "Currently available archetypes are: ";
    auto archetypes = getArchetypes();
    for (const char a: archetypes) {
        std::cout << a << (a == archetypes.back() ? "" : ", ");
    }
    std::cout << std::endl;
}

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    // fixes relative path issues when running from different fs path
    // (e.g.: ./varcher/debug/app/app T or .\varcher\debug\app\app.exe T)
    fs::path exe_path = fs::canonical(fs::path(argv[0])).parent_path();
    fs::current_path(exe_path);

    char archetype;
    if (argc != 2) {
        std::cout << "WARNING: Using standard renderer: " << STANDARD_RENDERER << std::endl;
        std::cout << "If you want to use a different renderer: " << argv[0] << " <archetype>" << std::endl;
        printArchetypes();
        archetype = STANDARD_RENDERER;
    } else {
        archetype = argv[1][0];
    }
    try {
        return run(fromArchetype(archetype));
    } catch (std::invalid_argument &e) {
        std::cerr << "ERROR: could not parse rendering archetype" << std::endl;
        printArchetypes();
        return EXIT_FAILURE;
    }
}