project(app)

add_executable(${PROJECT_NAME} main.cpp)

# Link to main library
target_link_libraries(${PROJECT_NAME} engine)

# Add resources and shaders as dependencies
add_dependencies(${PROJECT_NAME} resource shader)