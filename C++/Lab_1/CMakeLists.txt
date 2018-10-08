get_filename_component(DIR_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
set(target "app_${DIR_NAME}_frequent_words_problem")

add_executable(${target} main.cpp)