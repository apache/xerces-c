/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution,
 *    if any, must include the following acknowledgment:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowledgment may appear in the software itself,
 *    if and wherever such third-party acknowledgments normally appear.
 *
 * 4. The names "Xerces" and "Apache Software Foundation" must
 *    not be used to endorse or promote products derived from this
 *    software without prior written permission. For written
 *    permission, please contact apache\@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache",
 *    nor may "Apache" appear in their name, without prior written
 *    permission of the Apache Software Foundation.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 * This software consists of voluntary contributions made by many
 * individuals on behalf of the Apache Software Foundation, and was
 * originally based on software copyright (c) 1999, International
 * Business Machines, Inc., http://www.ibm.com .  For more information
 * on the Apache Software Foundation, please see
 * <http://www.apache.org/>.
 */

/*
 * $Id$
 */

/*
	The code in this file should be compiled into your Metrowerks Runtime PPC code.
	The net result will be that you can build generic unix-style C code without
	Macintosh modifications to setup argc/argv. The Xerces sample projects expect
	this modification.
	
	To use:
	
		- Add the code below to your Metrowerks Startup.c file (in runtime libraries)
		- Rebuild the various targets of project MSL All.mcp.
		- In the PPC Linker section of your project, set the start code to be
		  __start_ccommand instead of the typical __start. This will cause ccommand
		  to be invoked before main() is started.
		  
	This code has been submitted as a suggestion to Metrowerks: hopefully they'll
	add it to the startup code so this modification won't always be necessary.
	
	!!!NOTE!!!
	
	If the above seems too odious for your needs, you may also modify the "main" file
	of each sample by adding a line such as the following near the start of the main()
	function:
	
		argc = ccommand(&argv);

	This will invoke the command line dialog manually.
	
	- 8/3/00 James Berry <jberry@criticalpath.com>
*/


//	External declaration
int ccommand(char ***arg);

//	Forward declaration
pascal void __start_ccommand(void);

/*
 *	__start_ccommand	-	Optional special startup routine for Metrowerks C++ (PowerPC)
 *
 *	This routine should be specified as the PEF main routine in the container
 *	for any monolithic application that requires arguments via argc/argv.
 *  The program startup/termination sequence is:
 *
 *	1.	Register the exception-handling info for the application
 *	2.	Call all static initializers
 *	3.	Call ccommand to set up default values for 'argc' and 'argv' and call main()
 *	4.	Call exit() to perform required cleanup and termination, including
 *		destroying all static objects, closing open files, closing console window, etc.
 *
 *	We defer all details of proper program termination to the ANSI exit() routine.
 *
 */
pascal void __start_ccommand(void)
{
	int argc;
	char **argv;

	//	set the stack frame back-link to 0 to improve debugger stack display
	clear_stackframe_backlink();

	//	register this code fragment with the Exception Handling mechanism
	fragmentID = __register_fragment(__code_start__, __code_end__,
									__data_start__, __data_end__,
									__exception_table_start__, __exception_table_end__,
									__RTOC());
	
	__init_critical_regions();
	//	call all static initializers
	__sinit();
	
	argc = ccommand(&argv);
	
	//	call main(argc, argv)
	main(argc, argv);
	
	//	call exit() to terminate the program properly--will not return
	exit(0);
	
	//	unregister this code fragment with the Exception Handling mechanism
//	__unregister_fragment(fragmentID);
}


