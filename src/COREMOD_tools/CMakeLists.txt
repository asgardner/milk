set(CMAKE_TOOLCHAIN_FILE ~/vcpkg/scripts/buildsystems/vcpkg.cmake)
find_package(unofficial-cfitsio REQUIRED SHARED)

# library name
set(LIBNAME "milkCOREMODtools")
set(SRCNAME "COREMOD_tools")

message("SRCNAME = ${SRCNAME} -> LIBNAME = ${LIBNAME}")

set(SOURCEFILES
	${SRCNAME}.c
	fileutils.c
	imdisplay3d.c
	linregress.c
	logfunc.c
	mvprocCPUset.c
	quicksort.c
	statusstat.c
	stringutils.c
)

set(INCLUDEFILES
	${SRCNAME}.h
	fileutils.h
	imdisplay3d.h
	linregress.h
	logfunc.h
	mvprocCPUset.h
	quicksort.h
	statusstat.h
	stringutils.h
)

project(lib_${LIBNAME}_project)
add_library(${LIBNAME} SHARED ${SOURCEFILES})
target_include_directories(${LIBNAME} PUBLIC ${PROJECT_SOURCE_DIR}/..)
target_link_libraries(${LIBNAME} PUBLIC cfitsio)
install(TARGETS ${LIBNAME} DESTINATION lib)
install(FILES ${INCLUDEFILES} DESTINATION include/${SRCNAME})
