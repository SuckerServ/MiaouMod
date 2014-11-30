#### Dependencies ####

## Enet ##
find_package(ENet 1.3.12)

if(ENET_FOUND)
    set(ENET_INCLUDE_DIRS ${ENET_INCLUDE_DIR})    
else(ENET_FOUND)
    execute_process(
        COMMAND git submodule update --init src/enet
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

    add_subdirectory(enet)
endif(ENET_FOUND)

## LuaJIT##
option(LUA_USE_LUAJIT "Use LuaJIT instead of default Lua" ON)
unset(Lua_FOUND CACHE)
unset(LUA_INCLUDE_DIR CACHE)
unset(LUA_LIBRARY CACHE)
find_package(LuaJIT REQUIRED)

sed(LUAJIT_INCLUDE_DIRS ${LUA_INCLUDE_DIR})
set(LUA_INCLUDE_DIRS ${LUA_INCLUDE_DIR})
set(LUA_LIBRARY ${LUA_LIBRARY})

## OpenSSL ##
find_package(OpenSSL)

option(WITH_OPENSSL "Enable SSL support" ON)

## MaxmindDB ##
find_package(MMDB)

if(NOT MMDB_FOUND)
    execute_process(
        COMMAND git submodule update --init src/libmaxminddb
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

    set(MMDB_LIBRARY
        maxminddb
        CACHE INTERNAL "mmdb library")

    set(MMDB_INCLUDE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}/libmaxminddb/include
        ${CMAKE_CURRENT_BINARY_DIR}/maxminddb)

    set(MMDB_SOURCES
        libmaxminddb/src/maxminddb.c
    )

    SET_SOURCE_FILES_PROPERTIES(libmaxminddb/src/maxminddb.c PROPERTIES COMPILE_FLAGS -std=c99)
    SET_SOURCE_FILES_PROPERTIES(libmaxminddb/src/maxminddb.c PROPERTIES COMPILE_DEFINITIONS PACKAGE_VERSION="1.0.1")

    CHECK_TYPE_SIZE("unsigned __int128" UINT128)

    if(NOT HAVE_UINT128)
        add_definitions(-DMMDB_UINT128_IS_BYTE_ARRAY=1)
    endif()

    CHECK_TYPE_SIZE("unsigned int __attribute__((mode(TI)))" UINT128_USING_MODE)

    if(HAVE_UINT128_USING_MODE)
        add_definitions(-DMMDB_UINT128_USING_MODE=1)
    endif()


    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/libmaxminddb/include/maxminddb_config.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/maxminddb/maxminddb_config.h
    )

    add_library(${MMDB_LIBRARY} SHARED ${MMDB_SOURCES})

    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        target_link_libraries(${MMDB_LIBRARY} ws2_32)
    endif()

    install(TARGETS
        ${MMDB_LIBRARY}
        LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
        RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR})

endif()

option(WITH_MMDB "Enable MMDB support" ON)

if(WITH_MMDB)
    add_definitions(-DWITH_MMDB)
endif(WITH_MMDB)


## LuaTZ ##

execute_process(
    COMMAND git submodule update --init src/luatz
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

FILE(GLOB LuaTZ_FILES "${CMAKE_CURRENT_SOURCE_DIR}/luatz/luatz/*.lua")

install(FILES
    ${LuaTZ_FILES}
    DESTINATION ${PROJECT_SOURCE_DIR}/script/package/luatz)


## LuaCrypto ##
#execute_process(
#    COMMAND git submodule update --init src/luacrypto
#    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
#)

#add_subdirectory(luacrypto)

## libuv ##
execute_process(
    COMMAND git submodule update --init src/uv
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_subdirectory(uv)


## Luvit ##
execute_process(
    COMMAND git submodule update --init src/luvit
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_subdirectory(luvit)

FILE(GLOB Luvit_FILES "${CMAKE_CURRENT_SOURCE_DIR}/luvit/lib/luvit/*.lua")

install(FILES
    ${Luvit_FILES}
    DESTINATION ${PROJECT_SOURCE_DIR}/script/package/luvit)



#add_subdirectory(lsqlite3)
#add_subdirectory(luasql)
#add_subdirectory(authserver)


include(CheckLibraryExists)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    check_library_exists(rt clock_gettime "" RTLIB_EXISTS)
endif(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/shared
    ${CMAKE_CURRENT_SOURCE_DIR}/game
    ${CMAKE_CURRENT_SOURCE_DIR}/miaoumod
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ENET_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
    ${MMDB_INCLUDE_DIRS}
    ${LUVIT_INCLUDE_DIRS}
    ${UV_INCLUDE_DIRS})

add_definitions(-fsigned-char -DSTANDALONE -DREVISION=$ENV{REVISION})


if(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_definitions( -g)
else(CMAKE_BUILD_TYPE STREQUAL "DEBUG")
    add_definitions( -O3)
endif(CMAKE_BUILD_TYPE STREQUAL "DEBUG")

set(TESSTOOLS_SOURCES
    shared/tools.cpp
    shared/stream.cpp
    shared/crypto.cpp
    engine/command.cpp
    engine/worldio.cpp
    game/entities.cpp
    )

add_library(tesstools STATIC ${TESSTOOLS_SOURCES})

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(tesstools -lrt)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_library(coreServices CoreServices)
    target_link_libraries(tesstools ${coreServices})
endif()

target_link_libraries(tesstools -lz ${ENET_LIBRARY})

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(LUA_MODULES_SOURCES
    miaoumod/lib/md5.c
    miaoumod/utils/time.cpp
    miaoumod/utils/files.cpp
    miaoumod/utils/miaoumod.cpp
    miaoumod/lua/event.cpp
    miaoumod/lua/push_function.cpp
    miaoumod/lua/pcall.cpp
    miaoumod/lua/error_handler.cpp
    miaoumod/lua/push.cpp
    miaoumod/lua/register_class.cpp
    miaoumod/lua/modules/module.cpp
    miaoumod/lua/modules/crypto.cpp
    miaoumod/lua/modules/mmdb.cpp
)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(LUA_MODULES_SOURCES
        ${LUA_MODULES_SOURCES}
        miaoumod/lua/library_extensions.cpp
        miaoumod/lua/modules/filesystem.cpp
    )
endif()


add_library(lua_modules STATIC ${LUA_MODULES_SOURCES})

target_link_libraries(lua_modules 
    tesstools
    ${LUA_LIBRARY}
    ${MMDB_LIBRARY}
    ${OPENSSL_LIBRARIES})

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(lua_modules
        -lcrypto -lssl)
endif()

set(GAME_SERVER_SOURCES
    miaoumod/utils/files.cpp
    miaoumod/startup.cpp
    miaoumod/lua.cpp
    miaoumod/core_bindings.cpp
    miaoumod/events.cpp
    engine/server.cpp
    game/server.cpp
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(CMAKE_RC_COMPILE_OBJECT "${CMAKE_RC_COMPILER} -O coff -I${CMAKE_CURRENT_SOURCE_DIR} <SOURCE> <OBJECT>")
    set(GAME_SERVER_SOURCES
        ${GAME_SERVER_SOURCES}
        win32/mingw.rc
    )
endif()

add_executable(tess_server ${GAME_SERVER_SOURCES})

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" AND NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    target_link_libraries(tess_server -lrt)
endif()

target_link_libraries(tess_server 
    tesstools
    lua_modules
    ${LUVIT_LIBRARIES}
    ${ENET_LIBRARY})

add_executable(keygen miaoumod/standalone/keygen.cpp)
target_link_libraries(keygen tesstools)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
add_executable(monitor miaoumod/standalone/monitor.cpp miaoumod/lib/program_arguments.cpp)
    install(TARGETS
        monitor
        RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR})
endif()

install(TARGETS 
    tess_server
    RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR} 
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR} 
    ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR})

install(TARGETS 
    keygen
    RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR}/utils)
