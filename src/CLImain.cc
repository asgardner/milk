#include <assert.h>
#include <linux/limits.h>
#include <omp.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <iostream>

extern "C" {
  #include <CommandLineInterface/CLIcore.h>
  #include <CommandLineInterface/CLIcore/CLIcore_UI.h>
  #include "milk_config.h"
}

#define STYLE_BOLD "\033[1m"
#define STYLE_NO_BOLD "\033[22m"
#define STRINGMAXLEN_VERSIONSTRING 80
#define STRINGMAXLEN_APPNAME 40

struct timespec exec_install_time() {
  char path[PATH_MAX] = {};
  #pragma GCC diagnostic ignored "-Wunused-result"
  readlink("/proc/self/exe", path, PATH_MAX);
  struct stat s;
  memset(&s, 0, sizeof(s));
  stat(path, &s);
  return s.st_mtim;
}

int main(int argc, char *argv[]) {
  std::cout << "\r";  // Force c++ libraries to be linked/loaded in CLI before
                      // shared libraries.

  char AppName[STRINGMAXLEN_APPNAME];

  char *CLI_APPNAME = getenv("MILKCLI_APPNAME");
  if (CLI_APPNAME != NULL) {
    strncpy(AppName, CLI_APPNAME, STRINGMAXLEN_APPNAME - 1);
  } else {
    strncpy(AppName, "milk", STRINGMAXLEN_APPNAME - 1);
  }

  if (getenv("MILK_QUIET")) {
    data.quiet = 1;
  } else {
    data.quiet = 0;
  }

  if (getenv("MILK_ERROREXIT")) {
    data.errorexit = 1;
  } else {
    data.errorexit = 0;
  }

  // Allocate data.testpointarray
#ifndef NDEBUG
  printf("        [ENABLED]  Code test point tracing\n");
  // allocate circular buffer memory
  data.testpointarray =
      (CODETESTPOINT *)malloc(sizeof(CODETESTPOINT) * CODETESTPOINTARRAY_NBCNT);
  data.testpointarrayinit = 1;
  // initialize loop counter
  // loop counter increments when reaching end of circular buffer
  data.testpointloopcnt = 0;
  // set current entry index to zero
  data.testpointcnt = 0;
#endif

  char versionstring[STRINGMAXLEN_VERSIONSTRING];
  snprintf(versionstring, STRINGMAXLEN_VERSIONSTRING, "%d.%02d.%02d%s",
           VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_OPTION);

  struct timespec ts = exec_install_time();
  struct tm t;
  char time_string[64];
  localtime_r(&(ts.tv_sec), &t);
  strftime(time_string, 64, "%F %T", &t);
  if (data.quiet == 0) {
    printf(STYLE_BOLD);
#ifdef __INTEL_COMPILER
    printf("\n        %s v %s (INTEL %s) (build time=%s)\n", 
           PROJECT_NAME,
           versionstring, __VERSION__,
           time_string);
#else
    printf("\n        %s v %s (GCC %d.%d.%d) (build time=%s)\n", PROJECT_NAME,
           versionstring, __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__,
           time_string);
#endif
#ifndef NDEBUG
    printf(
        "        === DEBUG MODE : assert() & DEBUG_TRACEPOINT  enabled ===\n");
#endif
    printf(STYLE_NO_BOLD);
    if (data.errorexit == 1) {
      printf("        EXIT-ON-ERROR mode\n");
    }
  }

  strcpy(data.package_name, PACKAGE_NAME);

  data.package_version_major = VERSION_MAJOR;
  data.package_version_minor = VERSION_MINOR;
  data.package_version_patch = VERSION_PATCH;

  strcpy(data.package_version, versionstring);

  strcpy(data.sourcedir, SOURCEDIR);
  strcpy(data.configdir, CONFIGDIR);
  strcpy(data.installdir, INSTALLDIR);

  if (data.quiet == 0) {
    // printf("        %s version %s\n", data.package_name,
    // data.package_version);
#ifdef IMAGESTRUCT_VERSION
    printf("        ImageStreamIO v %s\n", IMAGESTRUCT_VERSION);
#endif
    // printf("        GNU General Public License v3.0\n");
    // printf("        Report bugs to : %s\n", PACKAGE_BUGREPORT);
    // printf("        Type \"help\" for instructions\n");
    printf("        \n");
  }

  // default exit code
  data.exitcode = RETURN_SUCCESS;

  runCLI(argc, argv, AppName);

  // errno_t CLIretval = RETURN_SUCCESS;

  if (data.quiet == 0) {
    printf("EXIT CODE %d\n", data.exitcode);
  } else {
    printf("\n");
  }

  // clean-up calling thread
  // pthread_exit(NULL);

#ifndef NDEBUG

  if (getenv("MILK_WRITECODETRACE")) {
    write_tracedebugfile();
  }
  printf("De-allocating test circular buffer\n");
  fflush(stdout);
  data.testpointarrayinit = 0;
  free(data.testpointarray);
#endif

  return data.exitcode;
}
