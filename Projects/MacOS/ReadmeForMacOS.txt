Disclaimer:

    XML4C is not officially supported under MacOS. These instructions
    were provided by one of the contributers/users of XML4C and have
    been included in the XML4C distribution just for convenience and
    benefit of other users. XML4C team, will not be able to maintain
    these files, but will be very willing to accept changes as
    submitted by the users. All such changes may be sent to
    xml4c@us.ibm.com.


The directions in this file cover installing and building XML4C and
ICU under the MacOS using CodeWarrior.

1.  Create a folder that will contain the XML4C and ICU distributions.
      For future reference I will refer to this folder as "src drop".
      
2.  Download and uncompress the ICU 1.2.5 source distribution and the
      XML4C 2.3.1 source distribution.  You might also want to download
      the binary distributions because they may contain documentation
      not present in the source distribution.  This will create two additional
      folders: "xml4csrc2_3_1" and "icu".  Move these two folders into the
      "src drop" folder.
      
3.  Drag the xml4csrc2_3_1 folder and drop it on to the "rename files" application
      located in the same folder as this readme.  This is a MacPerl
      script that renames files with names too long to fit in the HFS/HFS+
      filesystem.  It also searches through all of the source code and
      changes the #include statements to refer to the new file names.
      
4.  Move all the files from the "XML4C MacOS Drop:icu changes:common:" folder
      to the "src drop:icu:source:common:" folder, replacing the existing files
      in the icu source drop.
      
5.  Move the file from the "XML4C MacOS Drop:icu changes:extra:ustdio:" folder
      to the "src drop:icu:source:extra:ustdio:" folder, replacing the existing file
      in the icu source drop.
      
6.  Move the MacOS folder (in the Projects folder) to "src drop:xml4csrc2_3_1:Projects".

7.  You should be able to open the CodeWarrior project file
       "src drop:xml4c2:Projects:MacOS:xml4c:xml4c" and build the xml4c library.
       
8.  You should also be able to open the CodeWarrior project file
       "src drop:xml4c2_3_1:Projects:MacOS:icu:icu" and build the ICU library.
       
9.  If you wish you can create projects for and build the rest of
       the tools and test suites.  They are not needed if you just want to
       use xml4c.  I suggest that you use the binary data files distributed
       with the binary distribution of ICU instead of creating your own from
       the text data files in the ICE source distribution.
       
There are some things to be aware of when creating your own projects using
xml4c.

1.  You will need to link against both the ICU and XML4C libraries.

2.  The options "Always search user paths" and "Interpret DOS and Unix Paths"
       are very useful.  Some of the code won't compile without them set.
       
3.  Most of the tools and test code will require slight modification to compile
       and run correctly (typecasts, command line parameters, etc), but it is
       possible to get them working correctly.
       
4.  You will most likely have to set up the Access Paths.  The access paths in the
       xml4c projects should serve as a good example.
       
If you are having problems getting xml4c working feel free to email
me at jbellardo@alumni.calpoly.edu.  I may or may not be able to help
(depending on things out of my control like work/class schedules) but I will
try.
