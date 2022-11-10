cmake_minimum_required(VERSION 3.8)
set(CMAKE_TOOLCHAIN_FILE ~/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(Boost_NO_WARN_NEW_VERSIONS true)
set(ENV{LD_LIBRARY_PATH} "$ENV{HOME}/vcpkg/installed/x64-linux/lib:$ENV{HOME}/vcpkg/installed/x64-linux/lib")
set(SRCNAME "CommandLineInterface")
set(CMAKE_INSTALL_RPATH "/opt/intel/oneapi/vpl/2021.4.0/lib:/opt/intel/oneapi/tbb/2021.4.0/env/../lib/intel64/gcc4.8:/opt/intel/oneapi/mpi/2021.3.1//libfabric/lib:/opt/intel/oneapi/mpi/2021.3.1//lib/release:/opt/intel/oneapi/mpi/2021.3.1//lib:/opt/intel/oneapi/mkl/2021.3.0/lib/intel64:/opt/intel/oneapi/itac/2021.3.0/slib:/opt/intel/oneapi/ipp/2021.3.0/lib/intel64:/opt/intel/oneapi/ippcp/2021.3.0/lib/intel64:/opt/intel/oneapi/ipp/2021.3.0/lib/intel64:/opt/intel/oneapi/debugger/10.1.2/gdb/intel64/lib:/opt/intel/oneapi/debugger/10.1.2/libipt/intel64/lib:/opt/intel/oneapi/debugger/10.1.2/dep/lib:/opt/intel/oneapi/dal/2021.3.0/lib/intel64:/opt/intel/oneapi/compiler/2021.3.0/linux/lib:/opt/intel/oneapi/compiler/2021.3.0/linux/lib/x64:/opt/intel/oneapi/compiler/2021.3.0/linux/lib/emu:/opt/intel/oneapi/compiler/2021.3.0/linux/compiler/lib/intel64_lin:/opt/intel/oneapi/ccl/2021.3.0/lib/cpu_gpu_dpcpp:/opt/pleora/ebus_sdk/RHEL-6-x86_64/lib/genicam/bin/Linux64_x64:/opt/pleora/ebus_sdk/RHEL-6-x86_64/lib")

option(python_module "Compile Python Wrappers" OFF)
if(${python_module})
  add_subdirectory(python_module)
endif()

project(CLIcore C)

set(CMAKE_TOOLCHAIN_FILE ~/vcpkg/scripts/buildsystems/vcpkg.cmake)

find_package(GSL REQUIRED)
find_package(OpenMP)
find_package(unofficial-cfitsio CONFIG REQUIRED)
find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)
find_package(Threads REQUIRED)

bison_target(MilkBison calc_bison.y ${PROJECT_SOURCE_DIR}/calc_bison.c)
flex_target(MilkFlex calc_flex.l ${PROJECT_SOURCE_DIR}/calc_flex.c)
add_flex_bison_dependency(MilkFlex MilkBison)

set(CORE_FILES
    CLIcore.c
    CLIcore_utils.c
    CLIcore_UI.c
    CLIcore_checkargs.c
    CLIcore_datainit.c
    CLIcore_help.c
    CLIcore_memory.c
    CLIcore_modules.c
    CLIcore_setSHMdir.c
    CLIcore_signals.c
    ${BISON_MilkBison_OUTPUTS}
    ${FLEX_MilkFlex_OUTPUTS})

# main
add_library(CLIcore
            SHARED
            fpsCTRL_TUI.c
            fpsCTRL_TUI_process_user_key.c
            procCTRL_TUI.c
            streamCTRL_TUI.c
            processtools_trigger.c
            timeutils.c
            fps_add_entry.c
            fps_checkparameter.c
            fps_connect.c
            fps_connectExternalFPS.c
            fps_CONFstart.c
            fps_CONFstop.c
            fps_disconnect.c
            fps_execFPScmd.c
            fps_FPCONFexit.c
            fps_FPCONFloopstep.c
            fps_FPCONFsetup.c
            fps_FPSremove.c
            fps_GetFileName.c
            fps_getFPSargs.c
            fps_GetParamIndex.c
            fps_GetTypeString.c
            fps_load.c
            fps_loadstream.c
            fps_outlog.c
            fps_paramvalue.c
            fps_printlist.c
            fps_PrintParameterInfo.c
            fps_printparameter_valuestring.c
            fps_processcmdline.c
            fps_process_fpsCMDarray.c
            fps_processinfo_entries.c
            fps_read_fpsCMD_fifo.c
            fps_RUNexit.c
            fps_RUNstart.c
            fps_RUNstop.c
            fps_save2disk.c
            fps_scan.c
            fps_shmdirname.c
            fps_tmux.c
            fps_userinputsetparamvalue.c
            fps_WriteParameterToDisk.c
            TUItools.c
            ${CORE_FILES})

target_include_directories(CLIcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(CLIcore
                      PUBLIC 
                      m
                      cfitsio
                      OpenMP::OpenMP_C
                      dl
                      rt
                      ncurses
                      readline
                      ${GSL_LIBRARIES}
                      ${FFTW_LIBRARIES}
                      ${FFTWF_LIBRARIES}
                      ${CMAKE_THREAD_LIBS_INIT}
                      ${NCURSES_LIBRARIES}
                      milkCOREMODarith milkCOREMODiofits milkCOREMODmemory milkCOREMODtools)

install(TARGETS CLIcore DESTINATION lib)

install(FILES CLIcore.h
              CLIcore_utils.h
              CLIcore_UI.h
              CLIcore_checkargs.h
              CLIcore_datainit.h
              CLIcore_help.h
              CLIcore_memory.h
              CLIcore_modules.h
              CLIcore_setSHMdir.h
              CLIcore_signals.h
              fpsCTRL_TUI.h
              fpsCTRL_TUI_process_user_key.h
              processinfo.h
              streamCTRL_TUI.h
              cmdsettings.h
              milkDebugTools.h
              processinfo.h
              processtools_trigger.h
              timeutils.h
              function_parameters.h
              fps_add_entry.h
              fps_checkparameter.h
              fps_CONFstart.h
              fps_CONFstop.h
              fps_connect.h
              fps_connectExternalFPS.h
              fps_disconnect.h
              fps_execFPScmd.h
              fps_FPCONFexit.h
              fps_FPCONFloopstep.h
              fps_FPCONFsetup.h
              fps_FPSremove.h
              fps_GetFileName.h
              fps_getFPSargs.h
              fps_GetParamIndex.h
              fps_GetTypeString.h
              fps_load.h
              fps_loadstream.h
              fps_outlog.h
              fps_paramvalue.h
              fps_PrintParameterInfo.h
              fps_printparameter_valuestring.h
              fps_processcmdline.h
              fps_process_fpsCMDarray.h
              fps_processinfo_entries.h
              fps_read_fpsCMD_fifo.h
              fps_RUNexit.h
              fps_RUNstart.h
              fps_RUNstop.h
              fps_save2disk.h
              fps_scan.h
              fps_shmdirname.h
              fps_tmux.h
              fps_userinputsetparamvalue.h
              IMGID.h
              DESTINATION include/${SRCNAME})
