# 生成配置，如路径等都放这里
include(cmake/demo.cmake)

# 是否强制使用transport库源码编译
option(COMPLIE_TRANSPORT_SRC "if complie transport lib by source code force?" OFF)

# transport库文件绝对路径
set(TRANSPORT_LIB_PATH ${TRANSPORT_OUTPUT_PATH}/${TRANSPORT_LIB_NAME})

message(STATUS "Using prebuilt transport library: ${TRANSPORT_LIB_PATH}")
set(TRANSPORT_LIB ${TRANSPORT_LIB_PATH})


# 搜索其他源文件
aux_source_directory(${PROJECT_SRC_PATH} src_dir)

# 生成实际可执行文件
add_executable(${FINAL_PROJECT_NAME} demo.cpp ${src_dir})
target_include_directories(${FINAL_PROJECT_NAME} PRIVATE ${PROJECT_INC_PATH} ${HID_INC_PATH})
target_link_libraries(
    ${FINAL_PROJECT_NAME} 
    PRIVATE 
    ${IMG_PROCESS_LIB}
    ${TRANSPORT_LIB}
    ${HID_LIB}
    
)

set_target_properties(
    ${FINAL_PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${FINAL_OUTPUT_PATH}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ${FINAL_PROJECT_NAME} PROPERTY CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()