# Add yours files here.
set(SRC_LIST
    similarity/utils/global_alignment.cpp
    similarity/utils/global_alignment.h
    similarity/utils/helpers.h
    similarity/utils/helpers.cpp
    )

add_library(${PROJECT_NAME}_lib ${SRC_LIST})

add_executable(${PROJECT_NAME} similarity/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib)
