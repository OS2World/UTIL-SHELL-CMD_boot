/*
 *  CMD boot v1.0. Run a shell or some other program if a key is pressed within
 *    x seconds. Good to put optional .EXEs in your CONFIG.SYS
 *
 *  (Optional CMD.EXE before loading the Presentation Manager shell, for
 *    example...)
 *
 *  Copyright (c) Lighting and Sound Technologies, 1996.
 *    Written by Ryan C. Gordon.
 */

/*
 * USAGE : cmdboot.exe [timetowait] [progtorun.exe] [arguments...]
 *
 * [timetowait] is optional # of seconds to wait before running.
 *               Defaults to 5. You must specify this option if you want to
 *               specify...
 * [progtorun.exe] Optional executable to run. Defaults to c:\os2\cmd.exe
 *                 you must specify this option if you want to specify...
 * [arguments...] variable number of arguments to pass to [progtorun.exe]
 */

#define INCL_DOS
#define INCL_KBD
#include <os2.h>
#include <stdio.h>
#include <process.h>

#define DEFAULT_TIMEOUT  5
static UCHAR DEFAULT_SHELL[] = "c:\\os2\\cmd.exe";

static UCHAR MSG_STARTUP[] =
					"CMD boot v1.0\r\n"
					"  Copyright (c) Lighting and Sound Technologies, 1996\r\n"
					"  Written by Ryan C. Gordon.\r\n\r\n";



int main(int argc, char *argv[], char *envp[])
{
	ULONG ulBytesWritten;   /* bytes written. Dummy storage variable. */
	UCHAR ucShell[255];		/* Program to load. 					  */
	ULONG ulTimeout;  		/* Time to wait before continuing...      */
	ULONG ulLooper;			/* FOR...NEXT control variable. 		  */
	BOOL  bGetOut = FALSE;  /* Secondary FOR...NEXT control. 		  */
	char  *pcArgs[argc];	/* for passing args to child. 			  */
	HKBD  hKbd;				/* Handle to the keyboard. 				  */
	KBDKEYINFO kki;			/* For calls to KbdCharIn()... 			  */
	ULONG ulStartTime;		/* For figuring if key has been pressed.  */

	printf("%s", MSG_STARTUP);

	if (argc >= 3)
		strcpy(ucShell, argv[2]);
	else
		strcpy(ucShell, DEFAULT_SHELL);

	if (argc >= 2)
		ulTimeout = atoi(argv[1]);
	else
		ulTimeout = DEFAULT_TIMEOUT;

	pcArgs[0] = ucShell;

	if (argc > 3)
	{
		for (ulLooper = 3; ulLooper <= argc; ulLooper++)
			pcArgs[ulLooper - 2] = argv[ulLooper];
	} /* if */

	else
		pcArgs[1] = NULL;

	KbdOpen(&hKbd);
	KbdGetFocus(IO_WAIT, hKbd);
	KbdCharIn(&kki, IO_NOWAIT, hKbd);  /* get original kki.time value. */
	ulStartTime = kki.time;

	printf("Will run %s if key is pressed within %d seconds...\n",
			ucShell, ulTimeout);

	for (ulLooper = ulTimeout; ((ulLooper > 0) && (!bGetOut)); ulLooper--)
	{
		printf("%d...", ulLooper);
		DosSleep(1000);    /* Wait for 1 second...*/

	    KbdCharIn(&kki, IO_NOWAIT, hKbd);
	    if (kki.time != ulStartTime)
		{
			printf("\nLoading...\n\n");		
			KbdFreeFocus(hKbd);
			KbdClose(hKbd);
			spawnve(P_WAIT | P_DEFAULT, ucShell, pcArgs, envp);
			bGetOut = TRUE;
		} /* if */
	} /* for */

	if (!bGetOut)
	{
		printf("\nTimed out; no program will be loaded.\n");
		KbdFreeFocus(hKbd);
		KbdClose(hKbd);
	} /* if */

	return(0);

} /* main */

