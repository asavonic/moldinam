cmake_minimum_required(VERSION 2.8.7)
set(PROJECT_NAME_STR moldinam)
project(${PROJECT_NAME_STR} C CXX)

set ( GLUT_LIB_NAME glut )
set ( GLUT_DIR ${CMAKE_CURRENT_LIST_DIR} )

find_package( OpenGL )
if( NOT OPENGL_FOUND )
    return()
endif()

set_property( GLOBAL PROPERTY GLUT_INCLUDE "${${GLUT_DIR}/include}" )
set_property( GLOBAL PROPERTY GLUT_LIB ${${GLUT_DIR}/include} )

add_library(
    ${GLUT_LIB_NAME}
    STATIC
    ${GLUT_DIR}/src/freeglut_callbacks.c
    ${GLUT_DIR}/src/freeglut_cursor.c
    ${GLUT_DIR}/src/freeglut_display.c
    ${GLUT_DIR}/src/freeglut_ext.c
    ${GLUT_DIR}/src/freeglut_font.c
    ${GLUT_DIR}/src/freeglut_font_data.c
    ${GLUT_DIR}/src/freeglut_gamemode.c
    ${GLUT_DIR}/src/freeglut_geometry.c
    ${GLUT_DIR}/src/freeglut_glutfont_definitions.c
    ${GLUT_DIR}/src/freeglut_init.c
    ${GLUT_DIR}/src/freeglut_input_devices.c
    ${GLUT_DIR}/src/freeglut_joystick.c
    ${GLUT_DIR}/src/freeglut_main.c
    ${GLUT_DIR}/src/freeglut_menu.c
    ${GLUT_DIR}/src/freeglut_misc.c
    ${GLUT_DIR}/src/freeglut_overlay.c
    ${GLUT_DIR}/src/freeglut_spaceball.c
    ${GLUT_DIR}/src/freeglut_state.c
    ${GLUT_DIR}/src/freeglut_stroke_mono_roman.c
    ${GLUT_DIR}/src/freeglut_stroke_roman.c
    ${GLUT_DIR}/src/freeglut_structure.c
    ${GLUT_DIR}/src/freeglut_teapot.c
    ${GLUT_DIR}/src/freeglut_videoresize.c
    ${GLUT_DIR}/src/freeglut_window.c
)

if( UNIX )
    target_link_libraries( ${GLUT_LIB_NAME} X11 ) 
endif()
