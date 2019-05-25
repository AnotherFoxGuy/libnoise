add_executable(complexplanet complexplanet.cpp)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/noiseutils)
target_link_libraries(complexplanet noise noiseutils)
