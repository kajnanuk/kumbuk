cmake_minimum_required(VERSION 3.16)
project(kumbuk)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_PREFIX_PATH "C:\\Qt\\6.9.2\\mingw_64\\lib\\cmake")
# Find Qt (Widgets module)
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Add nlohmann/json if included as submodule
add_subdirectory(external/json)

# Source files (add all your .cpp files here)
set(SOURCES
        main.cpp
        # src/other_file.cpp
)

add_executable(kumbuk ${SOURCES})

# Link Qt libraries
target_link_libraries(kumbuk PRIVATE Qt6::Widgets)

# Link nlohmann/json
target_link_libraries(kumbuk PRIVATE nlohmann_json::nlohmann_json)

# Enable Qt automoc and uic for signals/slots and .ui files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)  # if using Qt resource files

# Include directories if needed (usually handled by target_link_libraries)
# target_include_directories(kumbuk PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)