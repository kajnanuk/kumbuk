cmake_minimum_required(VERSION 3.16)
project(kumbuk)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_PREFIX_PATH "C:\\Qt\\6.9.2\\mingw_64\\lib\\cmake")
# find Qt
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

# Add nlohmann/json if included as submodule
add_subdirectory(external/json)

#headers
set(HEADERS
        include/Add.h
        include/Card.h
        include/Deck.h
        include/Insights.h
        include/Practice.h
        include/Scheduler.h
        include/Statistics.h
        include/Window.h
)

# Source files (add all your .cpp files here)
set(SOURCES
        main.cpp
        src/Scheduler.cpp
        src/Statistics.cpp
        src/Card.cpp
        src/Deck.cpp
        src/Window.cpp
        src/Add.cpp
        src/Practice.cpp
        src/Insights.cpp
        # src/other_file.cpp
)

# Enable Qt automoc and uic for signals/slots and .ui files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)  # if using Qt resource files
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(kumbuk ${SOURCES} ${HEADERS})

# Link Qt libraries and nlohmann json
target_link_libraries(kumbuk PRIVATE Qt6::Core Qt6::Widgets Qt6::Network nlohmann_json::nlohmann_json)

# Include directories if needed (usually handled by target_link_libraries)
target_include_directories(kumbuk PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)