set(CMAKE_TOOLCHAIN_FILE ~/vcpkg/scripts/buildsystems/vcpkg.cmake)
find_package(unofficial-cfitsio REQUIRED SHARED)

# library name
set(LIBNAME "milkCOREMODarith")
set(SRCNAME "COREMOD_arith")

message("SRCNAME = ${SRCNAME} -> LIBNAME = ${LIBNAME}")

set(SOURCEFILES
	${SRCNAME}.c
	set_pixel.c
	image_crop.c
	image_merge3D.c
	image_total.c
	image_stats.c
	image_dxdy.c
	imfunctions.c
	mathfuncs.c
	image_arith__im__im.c
	image_arith__im_im__im.c
	image_arith__Cim_Cim__Cim.c
	image_arith__im_f__im.c
	image_arith__im_f_f__im.c
	execute_arith.c
)

set(INCLUDEFILES
	${SRCNAME}.h
	set_pixel.h
	image_crop.h
	image_merge3D.h
	image_total.h
	image_stats.h
	image_dxdy.h
	imfunctions.h
	mathfuncs.h
	image_arith__im__im.h
	image_arith__im_im__im.h
	image_arith__Cim_Cim__Cim.h
	image_arith__im_f__im.h
	image_arith__im_f_f__im.h
	execute_arith.h
)


# DEFAULT SETTINGS
# Do not change unless needed
# =====================================================================

project(lib_${LIBNAME}_project)

# Library can be compiled from multiple source files
# Convention: the main souce file is named <libname>.c
#
add_library(${LIBNAME} SHARED ${SOURCEFILES})

target_include_directories(${LIBNAME} PUBLIC ${PROJECT_SOURCE_DIR}/..)
target_link_libraries(${LIBNAME} PUBLIC m cfitsio)

install(TARGETS ${LIBNAME} DESTINATION lib)
install(FILES ${INCLUDEFILES} DESTINATION include/${SRCNAME})
