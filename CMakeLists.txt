cmake_minimum_required(VERSION 3.16)

project(DeckBuilderPro VERSION 0.4 LANGUAGES CXX)

# === POCZĄTEK POPRAWKI KODOWANIA ===
# Mówimy kompilatorowi, aby traktował pliki źródłowe jako UTF-8
# i tworzył program wykonywalny również w UTF-8.
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()
# === KONIEC POPRAWKI KODOWANIA ===

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core REQUIRED)

add_executable(DeckBuilderPro
    main.cpp
    Card.cpp
    Player.cpp
    CardLoader.cpp
    GameManager.cpp
    Card.h
    Player.h
    CardLoader.h
    GameManager.h
    json.hpp
)

target_link_libraries(DeckBuilderPro PRIVATE
    Qt6::Core
)
