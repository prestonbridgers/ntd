cmake_minimum_required(VERSION 3.21.2)

# Creating the project
project(ntd LANGUAGES C)

# Finding the curses package and panel and form libraries
find_package(Curses REQUIRED)
find_library(PANEL_LIBRARIES panel REQUIRED)
find_library(FORM_LIBRARIES form REQUIRED)

# Creating the executable
add_executable(${PROJECT_NAME} app.c entry.c ncurses.c util.c)

# Linking the target with the curses, panel, and form libraries
target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES}
                                      ${PANEL_LIBRARIES}
                                      ${FORM_LIBRARIES})
