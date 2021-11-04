cmake_minimum_required(VERSION 3.21.2)

#####################BEGIN FUNCTION DEFINITIONS######################
function(print var)
    message("${var} = ${${var}}")
endfunction()
#####################END FUNCTION DEFINITIONS######################

# Creating the project
project(ntd LANGUAGES C)

# Finding the curses package and panel and form libraries
find_package(Curses REQUIRED)

find_library(PANEL_LIBRARIES panel REQUIRED)
find_path(PANEL_INCLUDE_PATH panel.h REQUIRED)

print(PANEL_LIBRARIES)
print(PANEL_INCLUDE_DIR)

print(CURSES_LIBRARIES)
print(CURSES_INCLUDE_PATH)

# Creating the executable
add_executable(${PROJECT_NAME} app.c entry.c ncurses.c util.c)

# Adding include directories to the target's INCLUDE_DIRECTORIES
target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_PATH}
                                                   ${PANEL_INCLUDE_PATH})

# Linking the target with the curses, panel, and form libraries
target_link_libraries(${PROJECT_NAME} ${PANEL_LIBRARIES}
                                      ${CURSES_LIBRARIES})
