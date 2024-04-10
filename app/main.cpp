#include <iostream>
#include "app.h"

int main(int argc, char* argv[]) {
    char archetype;
    if (argc != 2) {
        std::cout << "WARNING: wrong usage. Using standard renderer T (Triangle)" << std::endl;
        std::cout << "Usage: " << argv[0] << " <archetype>" << std::endl;
        archetype = 'T';
    } else {
        archetype = argv[1][0];
    }
    try {
        return run(fromArchetype(archetype));
    } catch (std::invalid_argument &e) {
        std::cerr << "ERROR: could not parse rendering archetype" << std::endl;
        return EXIT_FAILURE;
    }
}
