set(TARGET_NAME n-puzzle)
set(SOURCE_DIR src)
set(DEST_DIR bin)
set(INCLUDE_DIRS include)
file(GLOB SOURCES ${SOURCE_DIR}/*.cpp)

include_directories(${INCLUDE_DIRS})
add_executable(${TARGET_NAME} ${SOURCES})

set_target_properties(${TARGET_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${DEST_DIR}
    )
