#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <string.h>


#include <sys/types.h>
#include <stdint.h>


#include "CLIcore.h"
#include "TUItools.h"





/** @brief print PID with highlighting
 *
 */
int streamCTRL_print_procpid(
    int      DispPID_NBchar,
    pid_t    procpid,
    pid_t   *upstreamproc,
    int      NBupstreamproc,
    uint32_t mode
)
{
    //int DispPID_NBchar = 8;
    int activitycolorcode = 0;
    int is_upstream       = 0;
    int upstreamindex     = 0;

    if(mode & PRINT_PID_FORCE_NOUPSTREAM)
    {
        is_upstream = 0;
    }
    else
    {
        for(int i = 0; i < NBupstreamproc; i++)
        {
            if(procpid == upstreamproc[i])
            {
                is_upstream   = 1;
                upstreamindex = i;
                break;
            }
        }
    }

    if(procpid > 0)
    {
        if(getpgid(procpid) >= 0)  // check if pid active
        {
            activitycolorcode = 2;
        }
        else
        {
            if(procpid > 0)
            {
                activitycolorcode = 0;
            }
        }
    }

    if(is_upstream == 1)
    {
        if(activitycolorcode != 2)
        {
            screenprint_setreverse();
        }
        else
        {
            activitycolorcode = 12;
        }
    }

    if(activitycolorcode > 0)
    {
        screenprint_setcolor(activitycolorcode);
    }

    if(is_upstream)
    {
        char upstreamstring[DispPID_NBchar + 1];
        snprintf(upstreamstring, DispPID_NBchar, "%2d >>", upstreamindex);
        TUI_printfw("%*s", DispPID_NBchar, upstreamstring);
    }
    else
    {
        TUI_printfw("%*d", DispPID_NBchar, (int) procpid);
    }

    if(activitycolorcode > 0)
    {
        screenprint_unsetcolor(activitycolorcode);
    }

    if((activitycolorcode != 2) && (is_upstream == 1))
    {
        screenprint_unsetreverse();
    }

    return DispPID_NBchar;
}
