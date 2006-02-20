$self->{CCFLAGS} .= " -DHAS_BOOL -Dbool=int";

__END__
This was derived from the following email:

From: "Derek Fountain" <fountai@hursley.ibm.com>
Subject: Re: Xerces Perl on AIX
To: xerces-p-dev@xml.apache.org
Date: Mon, 23 Jul 2001 11:22:39 +0100

> Good detective work, derek. This is most definately the problem. Perl
> thinks your bool's should be char's, and Xerces-C thinks they should
> be int's. Try adding -Dbool=int to the CFLAGS in your Makefile.PL and
> recompile Xerces.pm.
>
> You want to convince them to both use the same encoding, so you either
> get Xerces-C to accept bool=char, or the other way 'round.

Ah-ha! I've got it working! The samples work! The job's not done yet, but the 
end is in sight...

Setting -Dbool=int in the Makefile.PL didn't help. The compiler issues a 
warning about the value being redefined, then uses the new value. The new 
value appears to come from my perl-5.6.1/lib/5.6.1/aix/CORE/handy.h file, 
which resets bool=char. I forced a fix by going into this file and changing 
the line there before doing the compile of Xerces for Perl. Specifically:

===
#ifndef HAS_BOOL
# if defined(UTS) || defined(VMS)
#  define bool int
# else
// #  define bool char
#  define bool int
# endif
# define HAS_BOOL 1
#endif
===

Of course, this isn't the real solution. I've had to change the file back to 
prevent breakage in anything else I try to compile. I had a look in the 
xerces library code and found this in src/util/Transcoders/Iconv400/utypes.h:

===
#if ! HAVE_BOOL_T
typedef int8_t bool_t;
#endif
===

I'm not sure if that's the right file, but clearly the intention in the 
xerces code is for integer type booleans.

So, if my assessment is right, the xerces library code wants ints, and the 
Perl library code wants chars, which means the Xerces-Perl code is going to 
have to bend around the issue, probably by ignoring the Perl header file and 
defining bool to be an int itself when compiled under AIX (or, presumably, 
any UNIX platform which Perl thinks should have char based bools).

Quite how to sort this one out is, I think, a job for someone who has a 
better idea of how things fit together. :P

---------------------------------------------------------------------
To unsubscribe, e-mail: xerces-p-dev-unsubscribe@xml.apache.org
For additional commands, e-mail: xerces-p-dev-help@xml.apache.org



