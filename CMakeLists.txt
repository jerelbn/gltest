cmake_minimum_required (VERSION 3.5.1)
project (gltest)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Build type: Release")
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
 
include_directories(include
    ${OPENGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
)

add_executable(SimpleDraw src/SimpleDraw.cpp)
target_link_libraries(SimpleDraw
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

add_executable(SimpleAnim src/SimpleAnim.cpp)
target_link_libraries(SimpleAnim
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

add_executable(Solar src/Solar.cpp)
target_link_libraries(Solar
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

add_executable(ConnectDots src/ConnectDots.cpp)
target_link_libraries(ConnectDots
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

add_executable(SimpleLight src/SimpleLight.cpp)
target_link_libraries(SimpleLight
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

add_executable(WrapTorus src/WrapTorus.cpp)
target_link_libraries(WrapTorus
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

add_executable(LightTorus src/LightTorus.cpp)
target_link_libraries(LightTorus
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

add_executable(TextureBMP src/TextureBMP.cpp src/RgbImage.cpp)
target_link_libraries(TextureBMP
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

add_executable(FourTextures src/FourTextures.cpp src/RgbImage.cpp)
target_link_libraries(FourTextures
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

add_executable(TextureTorus src/TextureTorus.cpp src/RgbImage.cpp)
target_link_libraries(TextureTorus
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

add_executable(GluCylinders src/GluCylinders.cpp)
target_link_libraries(GluCylinders
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

add_executable(SimpleNurbs src/SimpleNurbs.cpp)
target_link_libraries(SimpleNurbs
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)
