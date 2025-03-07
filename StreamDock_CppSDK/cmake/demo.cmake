# [宏]打印某个路径名称
macro(printPath _path)
    message(STATUS "${_path}: ${${_path}}")
endmacro()

# 工程实际路径
get_filename_component(REAL_PROJECT_PATH ${CMAKE_CURRENT_LIST_DIR} PATH)

# transport库源文件路径
set(TRANSPORT_SRC_PATH ${REAL_PROJECT_PATH}/transport)
printPath(TRANSPORT_SRC_PATH)

# transport库生成路径
set(TRANSPORT_OUTPUT_PATH ${REAL_PROJECT_PATH}/lib)
printPath(TRANSPORT_OUTPUT_PATH)

# 头文件包含路径
set(PROJECT_INC_PATH ${REAL_PROJECT_PATH}/include)
printPath(PROJECT_INC_PATH)

# 源文件目录
set(PROJECT_SRC_PATH ${REAL_PROJECT_PATH}/src)
printPath(PROJECT_SRC_PATH)

# 第三方库链接路径
set(PROJECT_LIB_PATH ${REAL_PROJECT_PATH}/lib) 
printPath(PROJECT_LIB_PATH)

# 最终生成目录
set(FINAL_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
printPath(FINAL_OUTPUT_PATH)

# 跨平台不同库名称
set(FINAL_PROJECT_NAME "main")
if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    set(TRANSPORT_LIB_NAME "transport.lib")
    set(HID_INC_PATH "${PROJECT_INC_PATH}/windows")
    set(HID_LIB "${PROJECT_LIB_PATH}/hidapi.lib")
    set(IMG_PROCESS_LIB "${PROJECT_LIB_PATH}/FreeImage.lib" "${PROJECT_LIB_PATH}/FreeImagePlus.lib" )
endif()