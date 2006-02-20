# the following info helpful reported by Martin Raspe:
# From: "Martin Raspe" <raspe@uni-trier.de>
# Subject: XML::Xerces-2.3.0-0 on Windows
# To: xerces-p-dev@xml.apache.org
# Date: Thu, 12 Jun 2003 14:34:02 +0200
# --
# Without "-DHAS_BOOL" a lot of "unresolved external ..." linker errors
# are produced. -TP forces C++ compilation (not needed if the file is
# named *.cpp), -GX is asked for by the compiler.

use Config;

$self->{CCFLAGS} .= " $Config::Config{cppflags} -DHAS_BOOL -TP -GX";
