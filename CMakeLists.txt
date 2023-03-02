######################################## 
# chai (aka milk) build environment.

########## 
# Basic setup for CMake, vcpkg, etc.
set(CMAKE_TOOLCHAIN_FILE ~/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(Boost_NO_WARN_NEW_VERSIONS true)
set(ENV{LD_LIBRARY_PATH} "$ENV{HOME}/vcpkg/installed/x64-linux/lib")
enable_testing()
cmake_minimum_required(VERSION 3.14)
project(milk LANGUAGES C CXX)
set(VERSION_MAJOR 1)
set(VERSION_MINOR 03)
set(VERSION_PATCH 16)
set(VERSION_OPTION ".chai" )
set(PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}" )
set(PROJECT_DESCRIPTION "Modular Image processing tooLKit" )
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/chai")
option(INSTALLMAKEDEFAULT "Make this installation the system default" ON )
SET(CMAKE_FIND_PACKAGE_SORT_ORDER NATURAL)
SET(CMAKE_FIND_PACKAGE_SORT_DIRECTION DEC)

########## 
# ccache
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) # Less useful to do it for linking, see edit2
endif(CCACHE_FOUND)

########## 
# milk python.
option(build_python_module "Compile Python Wrappers" OFF)
if(build_python_module)
add_subdirectory (python_module)
endif()

########## 
# Configure a header file to pass some of the CMake settings
# to the source code
configure_file ("${PROJECT_SOURCE_DIR}/src/milk_config.h.in" "${PROJECT_SOURCE_DIR}/src/milk_config.h")
configure_file ("${PROJECT_SOURCE_DIR}/src/config.h.in" "${PROJECT_SOURCE_DIR}/src/config.h")

##########
# Find libraries to build.
list(APPEND libsrcdir src/CommandLineInterface)
list(APPEND libname CLIcore)
list(APPEND libsrcdir src/ImageStreamIO)
list(APPEND libname ImageStreamIO m)
list(APPEND modulelist COREMOD_arith COREMOD_iofits COREMOD_memory COREMOD_tools)
foreach(mname IN LISTS modulelist)
  string(REPLACE "_" "" mname1 "${mname}")
  list(APPEND libsrcdir src/${mname})
  list(APPEND libname milk${mname1})
  message("======= adding module  ${mname}")
endforeach()

##########
# Build type and flags.

set(SANITIZE_FLAGS "-fsanitize-address-use-after-scope")
set(SOURCE_FLAGS "-D_GNU_SOURCE -DMXLIB_MKL")
set(STACK_PROTECT_FLAGS "-fstack-protector-strong -fstack-protector-all -fstack-clash-protection")
set(WARNING_FLAGS "-Wall -Wextra -Werror")
set(C_STD "-std=c18")
set(CXX_STD "-std=c++17")
set(ARCH_DEBUG "-march=x86-64")
set(ARCH_OPT "-march=native")
set(COMMON_FLAGS "-pipe ${WARNING_FLAGS} ${SANITIZE_FLAGS} ${STACK_PROTECT_FLAGS} ${SOURCE_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG   "-O3 -g ${COMMON_FLAGS} ${ARCH_DEBUG} ${C_STD}")
set(CMAKE_CXX_FLAGS_DEBUG "-O3 -g ${COMMON_FLAGS} ${ARCH_DEBUG} ${CXX_STD}")
set(CMAKE_C_FLAGS_RELEASE    "-O3 -g -DNDEBUG ${COMMON_FLAGS} ${ARCH_OPT} ${C_STD}")
set(CMAKE_CXX_FLAGS_RELEASE  "-O3 -g -DNDEBUG ${COMMON_FLAGS} ${ARCH_OPT} ${CXX_STD}")
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# Set a default build type if none was specified
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}")
endif()

message("CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")

##########
# Add plugin libraries to be compiled
execute_process(COMMAND bash -c "find ${PROJECT_SOURCE_DIR}/plugins -mindepth 2 -maxdepth 2 -type d | grep -F -v .git" OUTPUT_VARIABLE libextradir)
string(REGEX REPLACE "\n" "; " libextradir "${libextradir}")
foreach(lsrcdir IN LISTS libsrcdir libextradir)
  string(REGEX REPLACE " " "" lsrcdir "${lsrcdir}")
  if (NOT ${lsrcdir} EQUAL "")
    add_subdirectory ( ${lsrcdir} )
  endif()
endforeach()
message("libextradir              = ${libextradir}")

##########
# Build the core milk executable.
find_package(Boost REQUIRED COMPONENTS system thread unit_test_framework)
add_executable(milk src/CLImain.cc) 
target_link_directories(milk PUBLIC $ENV{HOME}/vcpkg/installed/x64-linux/lib /opt/intel/oneapi/ipp/2021.6.0/lib/intel64/)
target_link_libraries(milk PUBLIC 
  ${libname}
  ${Boost_SYSTEM_LIBRARY} 
  ${Boost_THREAD_LIBRARY}
  boost_atomic
  boost_date_time
  ippcore ippvm ipps ippi
)
set(BUILD_FLAGS "-DPACKAGE_NAME=\\\"milk\\\" -DCONFIGDIR=\\\"${PROJECT_SOURCE_DIR}/config\\\" -DINSTALLDIR=\\\"${CMAKE_INSTALL_PREFIX}\\\" -DSOURCEDIR=\\\"${PROJECT_SOURCE_DIR}\\\" -DABSSRCTOPDIR=\\\"${PROJECT_SOURCE_DIR}\\\" -DPACKAGE_BUGREPORT=\\\"https://github.com/milk-org/milk/issues\\\"" )
set_target_properties(milk PROPERTIES COMPILE_FLAGS "${BUILD_FLAGS}" )

set(PROGRAM_PERMISSIONS_DEFAULT
    OWNER_WRITE OWNER_READ OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

install(TARGETS milk DESTINATION bin PERMISSIONS ${PROGRAM_PERMISSIONS_DEFAULT} SETUID)

set(CMAKE_INSTALL_PREFIX_LINK "/usr/local/${PROJECT_NAME}")
if(INSTALLMAKEDEFAULT)
    install(CODE "
    EXECUTE_PROCESS(COMMAND ln -snf ${CMAKE_INSTALL_PREFIX} ${CMAKE_INSTALL_PREFIX_LINK})
    ")
endif(INSTALLMAKEDEFAULT)

foreach(lname IN LISTS libname)
string(APPEND LINKSTRING "-l${lname} ")
endforeach()

set(INSTALL_PKGCONFIG_DIR "lib/pkgconfig"
    CACHE PATH "Installation directory for pkgconfig (.pc) files")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/milk.pc.in
               ${CMAKE_CURRENT_BINARY_DIR}/milk.pc @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/milk.pc
        DESTINATION "${INSTALL_PKGCONFIG_DIR}")


install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/milk_config.h
              ${CMAKE_CURRENT_SOURCE_DIR}/src/config.h
        DESTINATION include
        )

install(TARGETS CLIcore DESTINATION bin)

install(CODE "execute_process(COMMAND cp -v ${CMAKE_SOURCE_DIR}/plugins/mmtao-main/mmtao-main/aorc-config/lib/systemd/system/indi.service /etc/systemd/system/)")
install(CODE "execute_process(COMMAND cp -v ${CMAKE_SOURCE_DIR}/plugins/mmtao-main/mmtao-main/aorc-config/lib/systemd/system/indiproxy2.service /etc/systemd/system/)")
install(CODE "execute_process(COMMAND systemctl daemon-reload)")

install(CODE "execute_process(COMMAND cp -v ${CMAKE_SOURCE_DIR}/plugins/mmtao-main/mmtao-main/bin/runindionaorc /usr/local/chai/bin)")

install(CODE "execute_process(COMMAND cp -v ${CMAKE_SOURCE_DIR}/plugins/cacao-addons/milk-zmq/milk-zmq.service /etc/systemd/system)")
install(CODE "execute_process(COMMAND systemctl daemon-reload)")
install(CODE "execute_process(COMMAND systemctl enable --now milk-zmq)")