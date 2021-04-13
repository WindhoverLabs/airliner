
add_executable(ssmsim 
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/LoggerInstance.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/OBC.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Gnd.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Sim.cpp
)

target_link_libraries(ssmsim yaml yaml-cpp pthread)