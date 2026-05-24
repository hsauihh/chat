# Qt6 聊天客户端构建配置
cmake_minimum_required(VERSION 3.10)

# 查找 Qt6 库
find_package(Qt6 REQUIRED COMPONENTS Widgets Network)

# 启用 Qt 元对象编译器和 UI 编译器
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# 收集源文件
aux_source_directory(. SRC_LIST)

# 收集头文件（供 AUTOMOC 处理 Q_OBJECT 宏）
file(GLOB HEADER_LIST ${PROJECT_SOURCE_DIR}/include/qtclient/*.hpp)

# 收集 UI 文件（供 AUTOUIC 生成 ui_xxx.h）
file(GLOB UI_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)

# 指定生成可执行文件
add_executable(QtChatClient ${SRC_LIST} ${HEADER_LIST} ${UI_LIST})

# 添加头文件搜索路径
target_include_directories(QtChatClient PRIVATE ${PROJECT_SOURCE_DIR}/include/qtclient)

# 强制 UTF-8 编码，解决中文显示乱码
target_compile_options(QtChatClient PRIVATE -finput-charset=UTF-8 -fexec-charset=UTF-8)

# 指定可执行文件链接时依赖库文件
target_link_libraries(QtChatClient Qt6::Widgets Qt6::Network pthread)
