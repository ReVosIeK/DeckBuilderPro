cmake_minimum_required(VERSION 3.16)
project(DeckBuilderPro VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Prosimy już tylko o moduł Core!
find_package(Qt6 COMPONENTS Core REQUIRED)

add_executable(DeckBuilderPro
    main.cpp
    Card.cpp
    Player.cpp
    CardLoader.cpp
    GameManager.cpp # <<< Poprawiono na wielką literę

    Card.h
    Player.h
    CardLoader.h
    GameManager.h # <<< Poprawiono na wielką literę
    json.hpp
)

# Linkujemy już tylko z modułem Core!
target_link_libraries(DeckBuilderPro PRIVATE
    Qt6::Core
)
