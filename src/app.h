#pragma once

enum Type : char {
    TRIANGLE = 'T',
    VOXEL = 'V',
    GALAXY = 'G',
};

inline auto fromArchetype(char archetype) -> Type {
    return static_cast<Type>(archetype);
}

auto run(Type rendererType) -> int;
