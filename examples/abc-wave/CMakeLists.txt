project(abc-wave)
add_executable(${PROJECT_NAME} camera.cpp main.cpp openglwindow.cpp model.cpp)
enable_abcg(${PROJECT_NAME})
