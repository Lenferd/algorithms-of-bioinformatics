get_filename_component(TASK ${CMAKE_CURRENT_LIST_DIR} NAME)
set(target "app_${LAB_NAME}_${TASK}")
add_executable(${target} main.cpp)