/*
 * The Apache Software License, Version 1.1
 * 
 * Copyright (c) 1999 The Apache Software Foundation.  All rights 
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

/**
 * $Log$
 * Revision 1.1  1999/11/09 01:08:48  twl
 * Initial revision
 *
 * Revision 1.2  1999/11/08 20:44:12  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#ifndef DOMString_HEADER_GUARD_
#define DOMString_HEADER_GUARD_

#include <util/XML4CDefs.hpp>

class DOMStringHandle;
class DOM_NullPtr;

/**
 * <code>DOMString</code> is the generic string class that stores all strings
 * used in the DOM C++ API.
 *
 * Though this class supports most of the common string operations to manipulate
 * strings, it is not meant to be a comphrehensive string class.
 */

class CDOM_EXPORT DOMString {
public:
    /** @name Constructors and assignment operator */
    //@{
    /**
      * Default constructor for DOMString.  The resulting DOMString
      * object refers to no string at all; it will compare == 0.
      *
      */
    DOMString();

    /**
      * Copy constructor.
      *
      * @param other The object to be copied.
      */
    DOMString(const DOMString &other);

    /**
      * Constructor to build a DOMString from an XML character array.
      * (XMLCh is a 16 bit UNICODE character).
      *
      * @param other The null-terminated character array to be 
      *   that provides the initial value for the DOMString.
      */
    DOMString(const XMLCh *other);

    /**
      * Constructor to build a DOMString from a character array of given length.
      *
      * @param other The character array to be imported into the <code>DOMString</code>
      * @param length The length of the character array to be imported
      */
    DOMString(const XMLCh *other, int length);

    /**
      * Constructor to build a DOMString from an 8 bit character array.
      * The char * string will be transcoded to UNICODE using the default
      * code page on the system where the code is running.
      *
      * @param other The character array to be imported into the <code>DOMString</code>
      */
    DOMString(const char *other);

    /**
      * Constructor to build an empty DOMString with the character buffer
      *  preallocated to the specified size.  A DOMString will grow its buffer
      *  on demand, as characters are added, but it can be more efficient to
      *  allocate once in advance, if the size is known.
      *
      * @param initialBufferSize The initial size of the buffer inside the 
      * <code>DOMString</code>, measured in 16 bit characters.
      */
    DOMString(int initalBufferSize);

    /**
      * Assignment operator.  Make destination DOMString refer to the same
      *      underlying string in memory as the source string.
      *
      * @param the source DOMString.
      */
    DOMString &        operator = (const DOMString &other);

    //@}
    /** @name Destructor. */
    //@{

	 /**
	  * Destructor for DOMString
	  *
	  */
    ~DOMString();

    //@}
    /** @name Operators for string manipulation. */
    //@{

    /**
      * Concatenate a DOMString to another.
      *
      * @param other The string to be concatenated.
      * @return The concatenated object
      */
    DOMString   operator + (const DOMString &other);

    //@}
    /** @name Equality and Inequality operators. */
    //@{

    /**
      * Equality operator.  
      *
      * @param other The object to be compared with.
      * @return True if the two DOMStrings refer to the same underlying string
      *  in memory.  
      *  <p>
      *  WARNING:  operator == does NOT compare the contents of
      *  the two  strings.  To do this, use the <code>DOMString::equals()</code>
      *  This behavior is modelled after the String operations in Java, and
      *  is also similar to operator == on the other DOM_* classes.
      */
    bool        operator == (const DOMString &other) const;

    /**
      * Inequality operator.
      *
      * @param other The object to be compared with.
      * @return True if the two DOMStrings refer to different underlying strings in
      *    memory.
      * <p>
      *  WARNING:  operator == does NOT compare the contents of
      *  the two  strings.  To do this, use the <code>DOMString::equals()</code>
      *  This behavior is modelled after the String operations in Java, and
      *  is also similar to operator == on the other DOM_* classes.
      */
    bool        operator != (const DOMString &other) const;

    /**
      * Equality operator.  Test for a null DOMString, which is one that does
      *   not refer to any string at all; similar to a null object reference
      *   variable in Java.
      *
      * @param other must be 0 or null.
      * @return 
      */
    bool        operator == (const DOM_NullPtr *other) const;

    /**
      * Inequality operator, for null test.
      *
      * @param other must be 0 or null.
      * @return True if the two strings are different, false otherwise
      */
    bool        operator != (const DOM_NullPtr *other) const;

    //@}
    /** @name Functions to change the string. */
    //@{

    /**
      * Appends the content of another <code>DOMString</code> to this string.
      *
      * @param other The object to be appended
      */
    void        appendData(const DOMString &other);

    /**
      * Clears the data of this <code>DOMString</code>.
      *
      * @param offset The position from the beginning from which the data must be deleted
      * @param count The count of characters from the offset that must be deleted
      */
    void        deleteData(int offset, int count);

    /**
      * Inserts a string within the existing <code>DOMString</code> at an arbitrary position.
      *
      * @param offset The offset from the beginning at which the insertion needs to be done 
      *               in <code>this</code> object
      * @param data The <code>DOMString</code> containing the data that needs to be inserted
      * @return The object to be returned.
      */
    void        insertData(int offset, const DOMString &data);

    //@}
    /** @name Functions to get properties of the string. */
    //@{

    /**
      * Returns the character at the specified position.
      *
      * @param index The position at which the character is being requested
      * @return Returns the character at the specified position.
      */
    XMLCh       charAt(int index) const;

    /**
      * Returns a handle to the raw buffer in the <code>DOMString</code>.
      *
      * @return The pointer inside the <code>DOMString</code> containg the string data.
      */
    XMLCh       *rawBuffer() const;

    /**
      * Returns a sub-string of the <code>DOMString</code> starting at a specified position.
      *
      * @param offset The offset from the beginning from which the sub-string is being requested.
      * @param count The count of characters in the requested sub-string
      * @return The sub-string of the <code>DOMString</code> being requested
      */
    DOMString   substringData(int offset, int count) const;

    /**
      * Returns the length of the DOMString.
      *
      * @return The length of the string
      */
    int         length() const;

    //@}
    /** @name Cloning function. */
    //@{

    /**
      * Makes a clone of a the DOMString.
      *
      * @return The object to be cloned.
      */
    DOMString   clone() const;

    //@}
    /** @name Print functions. */
    //@{

    /**
      * Dumps the <code>DOMString</code> on the console.
      *
      */
    void        print() const;

    /**
      * Dumps the <code>DOMString</code> on the console with a line feed at the end.
      *
      */
    void        println() const;

    //@}
    /** @name Functions to compare a string with another. */
    //@{

    /**
      * Compares a DOMString with another.
      *
      * This strcmp does not match the semantics of the standard C strcmp.  
      * All it needs to do is define some less than - equals - greater than 
      * ordering of strings.  How doesn't matter.
      *
      *
      * @param other The object to be compared with
      * @return Either -1, 0, or 1 based on the comparison. 
      */
    int         strcmp(const DOMString &other) const;

    /**
      * Tells if a <code>DOMString</code> contains the same character data
      * as another.
      *
      * @param other The DOMString to be compared with.
      * @return True if the two <code>DOMString</code>s are same, false otherwise.
      */
    bool        equals(const DOMString &other) const;


      /**
      * Compare a DOMString with a null-terminated raw 16-bit character
      * string.
      *
      * @param other The character string to be compared with.
      * @return True if the strings are the same, false otherwise.
      */
    bool        equals(const XMLCh  *other) const;


    //@}
    friend      class DOMStringData;
    friend      class DOMStringHandle;
    friend      class DomMemDebug;
private:

    DOMStringHandle         *fHandle;
    static int              gLiveStringHandleCount;
    static int              gTotalStringHandleCount;
    static int              gLiveStringDataCount;
    static int              gTotalStringDataCount;
};

#endif
