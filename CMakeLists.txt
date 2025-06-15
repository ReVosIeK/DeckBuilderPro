cmake_minimum_required(VERSION 3.16)
project(DeckBuilderPro VERSION 0.5 LANGUAGES CXX)

# Opcje kompilacji dla poprawnego kodowania znaków
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

# Konfiguracja automatycznych narzędzi Qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC OFF) # Nie używamy .ui z Qt Widgets

# Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Wyszukiwanie pakietów Qt6 - dodajemy Qml i Quick
find_package(Qt6 COMPONENTS Core Qml Quick REQUIRED)

# Tworzenie pliku wykonywalnego
add_executable(DeckBuilderPro
    main.cpp
    resources.qrc
    Card.h
    Card.cpp
    Player.h
    Player.cpp
    CardLoader.h
    CardLoader.cpp
    GameManager.h
    GameManager.cpp
    json.hpp
    cards.json
    deck_composition.json
    build/Desktop_Qt_6_9_1_llvm_mingw_64_bit-Debug/logs/session.txt
)

# Linkowanie bibliotek Qt6
target_link_libraries(DeckBuilderPro PRIVATE
    Qt6::Core
    Qt6::Qml
    Qt6::Quick
)
