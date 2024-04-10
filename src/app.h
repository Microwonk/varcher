#pragma once

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

auto run(Type rendererType) -> int;
