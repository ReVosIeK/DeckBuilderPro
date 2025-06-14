# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\DeckBuilderPro_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\DeckBuilderPro_autogen.dir\\ParseCache.txt"
  "DeckBuilderPro_autogen"
  )
endif()
