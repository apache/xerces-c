#!/bin/sh

java -Djava.awt.headless=true -classpath "tools/jars/stylebook-1.0-b2.jar:tools/jars/xalan.jar:tools/jars/xerces.jar" org.apache.stylebook.StyleBook "targetDirectory=doc/html" doc/xerces-c_book.xml doc/style
