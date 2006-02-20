 #
 # Copyright 2002,2004 The Apache Software Foundation.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #      http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #

WriteMakefile(
    LINKTYPE => 'static',
    'NAME'	=> 'Handler',
    'INC'       => $INCLUDES,
    'OBJECT'    => '$(O_FILES)',
#    'CCFLAGS'   => $CFLAGS,
    'CC'        => $CXX,
    'SKIP'      => [qw( dynamic test makeaperl xs_o)],
  @OPTIMIZE,
  @LDFLAGS,
);

__END__
sub MY::static
{
 '
static  :: libhandler$(LIB_EXT) 

dynamic :: static

libhandler$(LIB_EXT): $(O_FILES) $(MYEXTLIB)
	$(AR) cru libhandler$(LIB_EXT) $(O_FILES)
	$(RANLIB) libhandler$(LIB_EXT)

libhandler.$(DLEXT): $(LDFROM) $(MYEXTLIB)
	$(LD) -o libhandler.$(DLEXT) $(LDDLFLAGS) --whole-archive $(LDFROM) $(OTHERLDFLAGS) $(MYEXTLIB) $(PERL_ARCHIVE) $(LDLOADLIBS) $(EXPORT_LIST)
	$(CHMOD) 755 libhandler.$(DLEXT)

';

}


