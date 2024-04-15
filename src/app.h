#pragma once

#include <vector>

enum Type : char {
    TRIANGLE = 'T',
    VOXEL = 'V',
    GALAXY = 'G',
};

const auto types = {
    TRIANGLE,
    VOXEL,
    GALAXY
};

inline auto fromArchetype(char archetype) -> Type {
    for (const Type type: types) {
        if (static_cast<char>(type) == archetype) {
            return type;
        }
    }
    throw std::invalid_argument("Invalid archetype character");
}

static inline auto getArchetypes() -> std::vector<char> {
    std::vector<char> archetypes;
    for (const Type type: types) {
        archetypes.push_back(static_cast<char>(type));
    }
    return archetypes;
}

auto run(Type rendererType) -> int;
