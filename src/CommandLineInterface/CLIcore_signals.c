/**
 * @file CLIcore_signals.c
 * 
 * @brief signals and debugging
 *
 */


#include <sys/resource.h> // getrlimit
#include <termios.h>
#include <stdarg.h>
#include <sys/stat.h>

#include "CommandLineInterface/CLIcore.h"








/**
 * @brief Write entry into debug log
 *
 *
 */
errno_t write_process_log()
{
    FILE *fplog;
    char fname[STRINGMAXLEN_FILENAME];
    pid_t thisPID;

    thisPID = getpid();
    WRITE_FILENAME(fname, "logreport.%05d.log", thisPID);

    struct tm *uttime;
    time_t tvsec0;


    fplog = fopen(fname, "a");
    if(fplog != NULL)
    {
        struct timespec tnow;
        //        time_t now;
        clock_gettime(CLOCK_REALTIME, &tnow);
        tvsec0 = tnow.tv_sec;
        uttime = gmtime(&tvsec0);
        fprintf(fplog, "%04d%02d%02dT%02d%02d%02d.%09ld ",
                1900 + uttime->tm_year, 1 + uttime->tm_mon, uttime->tm_mday, uttime->tm_hour,
                uttime->tm_min,  uttime->tm_sec, tnow.tv_nsec);

        fprintf(fplog, "    File    : %s\n", data.testpoint_file);
        fprintf(fplog, "    Function: %s\n", data.testpoint_func);
        fprintf(fplog, "    Line    : %d\n", data.testpoint_line);
        fprintf(fplog, "    Message : %s\n", data.testpoint_msg);
        fprintf(fplog, "\n");

        fclose(fplog);
    }

    return RETURN_SUCCESS;
}






static void set_terminal_echo_on()
{
    // Terminal settings
    struct termios termInfo;
    if(tcgetattr(0, &termInfo) == -1)
    {
        perror("tcgetattr");
        exit(1);
    }
    termInfo.c_lflag |= ECHO;  /* turn on ECHO */
    tcsetattr(0, TCSADRAIN, &termInfo);
}




static void fprintf_stdout(FILE *f, char const *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    va_start(ap, fmt);
    vfprintf(f, fmt, ap);
    va_end(ap);
}





/** @brief signal catching
 * 
 */
errno_t set_signal_catch()
{
    // catch signals for clean exit
    if(sigaction(SIGTERM, &data.sigact, NULL) == -1)
    {
        printf("\ncan't catch SIGTERM\n");
    }

    if(sigaction(SIGINT, &data.sigact, NULL) == -1)
    {
        printf("\ncan't catch SIGINT\n");
    }

    if(sigaction(SIGABRT, &data.sigact, NULL) == -1)
    {
        printf("\ncan't catch SIGABRT\n");
    }

    if(sigaction(SIGBUS, &data.sigact, NULL) == -1)
    {
        printf("\ncan't catch SIGBUS\n");
    }

    if(sigaction(SIGSEGV, &data.sigact, NULL) == -1)
    {
        printf("\ncan't catch SIGSEGV\n");
    }

    if(sigaction(SIGHUP, &data.sigact, NULL) == -1)
    {
        printf("\ncan't catch SIGHUP\n");
    }

    if(sigaction(SIGPIPE, &data.sigact, NULL) == -1)
    {
        printf("\ncan't catch SIGPIPE\n");
    }

    return RETURN_SUCCESS;
}




/**
 * @brief Write to disk a process report
 *
 * This function is typically called upon crash to help debugging
 *
 * errortypestring describes the type of error or reason to issue report
 *
 */
errno_t write_process_exit_report(
    const char *restrict errortypestring
)
{
    FILE *fpexit;
    char fname[STRINGMAXLEN_FILENAME];
    pid_t thisPID;
    long fd_counter = 0;

    thisPID = getpid();
    
    WRITE_FILENAME(fname, "exitreport-%s.%05d.log", errortypestring, thisPID);

    printf("EXIT CONDITION < %s >: See report in file %s\n", errortypestring,
           fname);
    printf("    File    : %s\n", data.testpoint_file);
    printf("    Function: %s\n", data.testpoint_func);
    printf("    Line    : %d\n", data.testpoint_line);
    printf("    Message : %s\n", data.testpoint_msg);
    fflush(stdout);

    struct tm *uttime;
    time_t tvsec0, tvsec1;


    fpexit = fopen(fname, "w");
    if(fpexit != NULL)
    {
        fprintf_stdout(fpexit, "PID : %d\n", thisPID);

        struct timespec tnow;
        //        time_t now;
        clock_gettime(CLOCK_REALTIME, &tnow);
        tvsec0 = tnow.tv_sec;
        uttime = gmtime(&tvsec0);
        fprintf_stdout(fpexit, "Time: %04d%02d%02dT%02d%02d%02d.%09ld\n\n",
                       1900 + uttime->tm_year, 1 + uttime->tm_mon, uttime->tm_mday, uttime->tm_hour,
                       uttime->tm_min,  uttime->tm_sec, tnow.tv_nsec);

        fprintf_stdout(fpexit, "Last encountered test point\n");
        tvsec1 = data.testpoint_time.tv_sec;
        uttime = gmtime(&tvsec1);
        fprintf_stdout(fpexit, "    Time    : %04d%02d%02dT%02d%02d%02d.%09ld\n",
                       1900 + uttime->tm_year, 1 + uttime->tm_mon, uttime->tm_mday, uttime->tm_hour,
                       uttime->tm_min,  uttime->tm_sec, data.testpoint_time.tv_nsec);

        double timediff = 1.0 * (tvsec0 - tvsec1) + 1.0e-9 * (tnow.tv_nsec -
                          data.testpoint_time.tv_nsec);
        fprintf_stdout(fpexit, "              %.9f sec ago\n", timediff);

        fprintf_stdout(fpexit, "    File    : %s\n", data.testpoint_file);
        fprintf_stdout(fpexit, "    Function: %s\n", data.testpoint_func);
        fprintf_stdout(fpexit, "    Line    : %d\n", data.testpoint_line);
        fprintf_stdout(fpexit, "    Message : %s\n", data.testpoint_msg);
        fprintf_stdout(fpexit, "\n");

        // Check open file descriptors
        struct rlimit rlimits;
        int max_fd_number;

        fprintf_stdout(fpexit, "File descriptors\n");
        getrlimit(RLIMIT_NOFILE, &rlimits);
        max_fd_number = getdtablesize();
        fprintf_stdout(fpexit, "    max_fd_number  : %d\n", max_fd_number);
        fprintf_stdout(fpexit, "    rlim_cur       : %lu\n", rlimits.rlim_cur);
        fprintf_stdout(fpexit, "    rlim_max       : %lu\n", rlimits.rlim_max);
        for(int i = 0; i <= max_fd_number; i++)
        {
            struct stat stats;

            fstat(i, &stats);
            if(errno != EBADF)
            {
                fd_counter++;
            }
        }
        fprintf_stdout(fpexit, "    Open files     : %ld\n", fd_counter);

        fclose(fpexit);
    }

    return RETURN_SUCCESS;
}





/**
 * @brief Signal handler
 *
 *
 */
void sig_handler(
    int signo
)
{
    switch(signo)
    {

        case SIGINT:
            printf("PID %d sig_handler received SIGINT\n", CLIPID);
            data.signal_INT = 1;
            break;

        case SIGTERM:
            printf("PID %d sig_handler received SIGTERM\n", CLIPID);
            data.signal_TERM = 1;
            set_terminal_echo_on();
            exit(EXIT_FAILURE);
            break;

        case SIGUSR1:
            printf("PID %d sig_handler received SIGUSR1\n", CLIPID);
            data.signal_USR1 = 1;
            break;

        case SIGUSR2:
            printf("PID %d sig_handler received SIGUSR2\n", CLIPID);
            data.signal_USR2 = 1;
            break;

        case SIGBUS: // exit program after SIGSEGV
            printf("PID %d sig_handler received SIGBUS \n", CLIPID);
            write_process_exit_report("SIGBUS");
            data.signal_BUS = 1;
            set_terminal_echo_on();
            exit(EXIT_FAILURE);
            break;

        case SIGABRT:
            printf("PID %d sig_handler received SIGABRT\n", CLIPID);
            write_process_exit_report("SIGABRT");
            data.signal_ABRT = 1;
            set_terminal_echo_on();
            exit(EXIT_FAILURE);
            break;

        case SIGSEGV: // exit program after SIGSEGV
            printf("PID %d sig_handler received SIGSEGV\n", CLIPID);
            write_process_exit_report("SIGSEGV");
            data.signal_SEGV = 1;
            set_terminal_echo_on();
            exit(EXIT_FAILURE);
            break;

        case SIGHUP:
            printf("PID %d sig_handler received SIGHUP\n", CLIPID);
            data.signal_HUP = 1;
            break;

        case SIGPIPE:
            printf("PID %d sig_handler received SIGPIPE\n", CLIPID);
            data.signal_PIPE = 1;
            break;
    }
}


