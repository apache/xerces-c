/*
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
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
 * $Log$
 * Revision 1.24  2001/07/04 14:38:20  peiyongz
 * IDDatatypeValidator: created
 * DatatypeValidatorFactory: IDDTV enabled
 * XMLString:isValidName(): to validate Name (XML [4][5])
 *
 * Revision 1.23  2001/06/13 14:07:55  peiyongz
 * isValidaEncName() to validate an encoding name (EncName)
 *
 * Revision 1.22  2001/05/23 15:44:51  tng
 * Schema: NormalizedString fix.  By Pei Yong Zhang.
 *
 * Revision 1.21  2001/05/11 13:26:31  tng
 * Copyright update.
 *
 * Revision 1.20  2001/05/09 18:43:30  tng
 * Add StringDatatypeValidator and BooleanDatatypeValidator.  By Pei Yong Zhang.
 *
 * Revision 1.19  2001/05/03 20:34:35  tng
 * Schema: SchemaValidator update
 *
 * Revision 1.18  2001/05/03 19:17:35  knoaman
 * TraverseSchema Part II.
 *
 * Revision 1.17  2001/03/21 21:56:13  tng
 * Schema: Add Schema Grammar, Schema Validator, and split the DTDValidator into DTDValidator, DTDScanner, and DTDGrammar.
 *
 * Revision 1.16  2001/03/02 20:52:46  knoaman
 * Schema: Regular expression - misc. updates for error messages,
 * and additions of new functions to XMLString class.
 *
 * Revision 1.15  2001/01/15 21:26:34  tng
 * Performance Patches by David Bertoni.
 *
 * Details: (see xerces-c-dev mailing Jan 14)
 * XMLRecognizer.cpp: the internal encoding string XMLUni::fgXMLChEncodingString
 * was going through this function numerous times.  As a result, the top hot-spot
 * for the parse was _wcsicmp().  The real problem is that the Microsofts wide string
 * functions are unbelievably slow.  For things like encodings, it might be
 * better to use a special comparison function that only considers a-z and
 * A-Z as characters with case.  This works since the character set for
 * encodings is limit to printable ASCII characters.
 *
 *  XMLScanner2.cpp: This also has some case-sensitive vs. insensitive compares.
 * They are also much faster.  The other tweak is to only make a copy of an attribute
 * string if it needs to be split.  And then, the strategy is to try to use a
 * stack-based buffer, rather than a dynamically-allocated one.
 *
 * SAX2XMLReaderImpl.cpp: Again, more case-sensitive vs. insensitive comparisons.
 *
 * KVStringPair.cpp & hpp: By storing the size of the allocation, the storage can
 * likely be re-used many times, cutting down on dynamic memory allocations.
 *
 * XMLString.hpp: a more efficient implementation of stringLen().
 *
 * DTDValidator.cpp: another case of using a stack-based buffer when possible
 *
 * These patches made a big difference in parse time in some of our test
 * files, especially the ones are very attribute-heavy.
 *
 * Revision 1.14  2000/10/13 22:47:57  andyh
 * Fix bug (failure to null-terminate result) in XMLString::trim().
 * Patch contributed by Nadav Aharoni
 *
 * Revision 1.13  2000/04/12 18:42:15  roddey
 * Improved docs in terms of what 'max chars' means in the method
 * parameters.
 *
 * Revision 1.12  2000/04/06 19:42:51  rahulj
 * Clarified how big the target buffer should be in the API
 * documentation.
 *
 * Revision 1.11  2000/03/23 01:02:38  roddey
 * Updates to the XMLURL class to correct a lot of parsing problems
 * and to add support for the port number. Updated the URL tests
 * to test some of this new stuff.
 *
 * Revision 1.10  2000/03/20 23:00:46  rahulj
 * Moved the inline definition of stringLen before the first
 * use. This satisfied the HP CC compiler.
 *
 * Revision 1.9  2000/03/02 19:54:49  roddey
 * This checkin includes many changes done while waiting for the
 * 1.1.0 code to be finished. I can't list them all here, but a list is
 * available elsewhere.
 *
 * Revision 1.8  2000/02/24 20:05:26  abagchi
 * Swat for removing Log from API docs
 *
 * Revision 1.7  2000/02/16 18:51:52  roddey
 * Fixed some facts in the docs and reformatted the docs to stay within
 * a reasonable line width.
 *
 * Revision 1.6  2000/02/16 17:07:07  abagchi
 * Added API docs
 *
 * Revision 1.5  2000/02/06 07:48:06  rahulj
 * Year 2K copyright swat.
 *
 * Revision 1.4  2000/01/12 00:16:23  roddey
 * Changes to deal with multiply nested, relative pathed, entities and to deal
 * with the new URL class changes.
 *
 * Revision 1.3  1999/12/18 00:18:10  roddey
 * More changes to support the new, completely orthagonal support for
 * intrinsic encodings.
 *
 * Revision 1.2  1999/12/15 19:41:28  roddey
 * Support for the new transcoder system, where even intrinsic encodings are
 * done via the same transcoder abstraction as external ones.
 *
 * Revision 1.1.1.1  1999/11/09 01:05:52  twl
 * Initial checkin
 *
 * Revision 1.2  1999/11/08 20:45:21  rahul
 * Swat for adding in Product name and CVS comment log variable.
 *
 */

#if !defined(XMLSTRING_HPP)
#define XMLSTRING_HPP

#include <util/XercesDefs.hpp>
#include <util/RefVectorOf.hpp>

class XMLLCPTranscoder;

/**
  * Class for representing native character strings and handling common string
  * operations
  *
  * This class is Unicode compliant. This class is designed primarily
  * for internal use, but due to popular demand, it is being made
  * publicly available. Users of this class must understand that this
  * is not an officially supported class. All public methods of this
  * class are <i>static functions</i>.
  *
  */
class XMLUTIL_EXPORT XMLString
{
public:
    /* Static methods for native character mode string manipulation */
    /** @name Conversion functions */
    //@{

    /** Converts binary data to a text string based a given radix
      *
      * @param toFormat The beginning of the input string to convert
      * @param toFill The buffer that will hold the output on return. The
      *        size of this buffer should at least be 'maxChars + 1'.
      * @param maxChars The maximum number of output characters that can be
      *         accepted. If the result will not fit, it is an error.
      * @param radix The radix of the input data, based on which the conversion
      * will be done
      */
    static void binToText
    (
        const   unsigned int    toFormat
        ,       char* const     toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    /** Converts binary data to a text string based a given radix
      *
      * @param toFormat The beginning of the input string to convert
      * @param toFill The buffer that will hold the output on return. The
      *        size of this buffer should at least be 'maxChars + 1'.
      * @param maxChars The maximum number of output characters that can be
      *         accepted. If the result will not fit, it is an error.
      * @param radix The radix of the input data, based on which the conversion
      * will be done
      */
    static void binToText
    (
        const   unsigned int    toFormat
        ,       XMLCh* const    toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    /** Converts binary data to a text string based a given radix
      *
      * @param toFormat The beginning of the input string to convert
      * @param toFill The buffer that will hold the output on return. The
      *        size of this buffer should at least be 'maxChars + 1'.
      * @param maxChars The maximum number of output characters that can be
      *         accepted. If the result will not fit, it is an error.
      * @param radix The radix of the input data, based on which the conversion
      * will be done
      */
    static void binToText
    (
        const   unsigned long   toFormat
        ,       char* const     toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    /** Converts binary data to a text string based a given radix
      *
      * @param toFormat The beginning of the input string to convert
      * @param toFill The buffer that will hold the output on return. The
      *        size of this buffer should at least be 'maxChars + 1'.
      * @param maxChars The maximum number of output characters that can be
      *         accepted. If the result will not fit, it is an error.
      * @param radix The radix of the input data, based on which the conversion
      * will be done
      */
    static void binToText
    (
        const   unsigned long   toFormat
        ,       XMLCh* const    toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    /** Converts binary data to a text string based a given radix
      *
      * @param toFormat The beginning of the input string to convert
      * @param toFill The buffer that will hold the output on return. The
      *        size of this buffer should at least be 'maxChars + 1'.
      * @param maxChars The maximum number of output characters that can be
      *         accepted. If the result will not fit, it is an error.
      * @param radix The radix of the input data, based on which the conversion
      * will be done
      */
    static void binToText
    (
        const   long            toFormat
        ,       char* const     toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    /** Converts binary data to a text string based a given radix
      *
      * @param toFormat The beginning of the input string to convert
      * @param toFill The buffer that will hold the output on return. The
      *        size of this buffer should at least be 'maxChars + 1'.
      * @param maxChars The maximum number of output characters that can be
      *         accepted. If the result will not fit, it is an error.
      * @param radix The radix of the input data, based on which the conversion
      * will be done
      */
    static void binToText
    (
        const   long            toFormat
        ,       XMLCh* const    toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    /** Converts binary data to a text string based a given radix
      *
      * @param toFormat The beginning of the input string to convert
      * @param toFill The buffer that will hold the output on return. The
      *        size of this buffer should at least be 'maxChars + 1'.
      * @param maxChars The maximum number of output characters that can be
      *         accepted. If the result will not fit, it is an error.
      * @param radix The radix of the input data, based on which the conversion
      * will be done
      */
    static void binToText
    (
        const   int             toFormat
        ,       char* const     toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    /** Converts binary data to a text string based a given radix
      *
      * @param toFormat The beginning of the input string to convert
      * @param toFill The buffer that will hold the output on return. The
      *        size of this buffer should at least be 'maxChars + 1'.
      * @param maxChars The maximum number of output characters that can be
      *         accepted. If the result will not fit, it is an error.
      * @param radix The radix of the input data, based on which the conversion
      * will be done
      */
    static void binToText
    (
        const   int             toFormat
        ,       XMLCh* const    toFill
        , const unsigned int    maxChars
        , const unsigned int    radix
    );

    /**
      * Converts a string of decimal chars to a binary value
      *
      * Note that leading and trailng whitespace is legal and will be ignored
      * but the remainder must be all decimal digits.
      *
      * @param toConvert The string of digits to convert
      * @param toFill    The unsigned int value to fill with the converted
      *                  value.
      */
    static bool textToBin
    (
        const   XMLCh* const    toConvert
        ,       unsigned int&   toFill
    );

    /**
      * Converts a string of decimal chars to a binary value
      *
      * Note that leading and trailng whitespace is legal and will be ignored,
      *
      * Only one and either of (+,-) after the leading whitespace, before
      * any other characters are allowed.
      *
      * but the remainder must be all decimal digits.
      *
      * @param toConvert The string of digits to convert
      */
    static int parseInt
    (
        const   XMLCh* const    toConvert
    );

    //@}

    /** @name String concatenation functions */
    //@{
    /** Concatenates two strings.
      *
      * <code>catString</code> appends <code>src</code> to <code>target</code> and
      * terminates the resulting string with a null character. The initial character
      * of <code>src</code> overwrites the terminating character of <code>target
      * </code>.
      *
      * No overflow checking is performed when strings are copied or appended.
      * The behavior of <code>catString</code> is undefined if source and
      * destination strings overlap.
      *
      * @param target Null-terminated destination string
      * @param src Null-terminated source string
      */
    static void catString
    (
                char* const     target
        , const char* const     src
    );

    /** Concatenates two strings.
      *
      * <code>catString</code> appends <code>src</code> to <code>target</code> and
      * terminates the resulting string with a null character. The initial character of
      * <code>src</code> overwrites the terminating character of <code>target</code>.
      * No overflow checking is performed when strings are copied or appended.
      * The behavior of <code>catString</code> is undefined if source and destination
      * strings overlap.
      *
      *    @param target Null-terminated destination string
      * @param src Null-terminated source string
      */
    static void catString
    (
                XMLCh* const    target
        , const XMLCh* const    src
    );
    //@}

    /** @name String comparison functions */
    //@{
    /** Lexicographically compares lowercase versions of <code>str1</code> and
      * <code>str2</code> and returns a value indicating their relationship.
      * @param str1 Null-terminated string to compare
      * @param str2 Null-terminated string to compare
      *
      * @return The return value indicates the relation of <code>str1</code> to
      * <code>str2</code> as follows
      *  Less than 0 means <code>str1</code> is less than <code>str2</code>
      *  Equal to 0 means <code>str1</code> is identical to <code>str2</code>
      *  Greater than 0 means <code>str1</code> is more than <code>str2</code>
      */
    static int compareIString
    (
        const   char* const     str1
        , const char* const     str2
    );

    /** Lexicographically compares lowercase versions of <code>str1</code> and
      * <code>str2</code> and returns a value indicating their relationship.
      * @param str1 Null-terminated string to compare
      * @param str2 Null-terminated string to compare
      * @return The return value indicates the relation of <code>str1</code> to
      * <code>str2</code> as follows
      *  Less than 0 means <code>str1</code> is less than <code>str2</code>
      *  Equal to 0 means <code>str1</code> is identical to <code>str2</code>
      *  Greater than 0 means <code>str1</code> is more than <code>str2</code>
      */
    static int compareIString
    (
        const   XMLCh* const    str1
        , const XMLCh* const    str2
    );


    /** Lexicographically compares, at most, the first count characters in
      * <code>str1</code> and <code>str2</code> and returns a value indicating the
      * relationship between the substrings.
      * @param str1 Null-terminated string to compare
      * @param str2 Null-terminated string to compare
      * @param count The number of characters to compare
      *
      * @return The return value indicates the relation of <code>str1</code> to
      * <code>str2</code> as follows
      *  Less than 0 means <code>str1</code> is less than <code>str2</code>
      *  Equal to 0 means <code>str1</code> is identical to <code>str2</code>
      *  Greater than 0 means <code>str1</code> is more than <code>str2</code>
      */
    static int compareNString
    (
        const   char* const     str1
        , const char* const     str2
        , const unsigned int    count
    );

    /** Lexicographically compares, at most, the first count characters in
      * <code>str1</code> and <code>str2</code> and returns a value indicating
      * the relationship between the substrings.
      * @param str1 Null-terminated string to compare
      * @param str2 Null-terminated string to compare
      * @param count The number of characters to compare
      *
      * @return The return value indicates the relation of <code>str1</code> to
      * <code>str2</code> as follows
      *  Less than 0 means <code>str1</code> is less than <code>str2</code>
      *  Equal to 0 means <code>str1</code> is identical to <code>str2</code>
      *  Greater than 0 means <code>str1</code> is more than <code>str2</code>
      */
    static int compareNString
    (
        const   XMLCh* const    str1
        , const XMLCh* const    str2
        , const unsigned int    count
    );


    /** Lexicographically compares, at most, the first count characters in
      * <code>str1</code> and <code>str2</code> without regard to case and
      * returns a value indicating the relationship between the substrings.
      *
      * @param str1 Null-terminated string to compare
      * @param str2 Null-terminated string to compare
      * @param count The number of characters to compare
      * @return The return value indicates the relation of <code>str1</code> to
      * <code>str2</code> as follows
      *  Less than 0 means <code>str1</code> is less than <code>str2</code>
      *  Equal to 0 means <code>str1</code> is identical to <code>str2</code>
      *  Greater than 0 means <code>str1</code> is more than <code>str2</code>
      */
    static int compareNIString
    (
        const   char* const     str1
        , const char* const     str2
        , const unsigned int    count
    );

    /** Lexicographically compares, at most, the first count characters in
      * <code>str1</code> and <code>str2</code> without regard to case and
      * returns a value indicating the relationship between the substrings.
      *
      * @param str1 Null-terminated string to compare
      * @param str2 Null-terminated string to compare
      * @param count The number of characters to compare
      *
      * @return The return value indicates the relation of <code>str1</code> to
      * <code>str2</code> as follows
      *  Less than 0 means <code>str1</code> is less than <code>str2</code>
      *  Equal to 0 means <code>str1</code> is identical to <code>str2</code>
      *  Greater than 0 means <code>str1</code> is more than <code>str2</code>
      */
    static int compareNIString
    (
        const   XMLCh* const    str1
        , const XMLCh* const    str2
        , const unsigned int    count
    );

    /** Lexicographically compares <code>str1</code> and <code>str2</code> and
      * returns a value indicating their relationship.
      *
      * @param str1 Null-terminated string to compare
      * @param str2 Null-terminated string to compare
      *
      * @return The return value indicates the relation of <code>str1</code> to
      * <code>str2</code> as follows
      *  Less than 0 means <code>str1</code> is less than <code>str2</code>
      *  Equal to 0 means <code>str1</code> is identical to <code>str2</code>
      *  Greater than 0 means <code>str1</code> is more than <code>str2</code>
      */
    static int compareString
    (
        const   char* const     str1
        , const char* const     str2
    );

    /** Lexicographically compares <code>str1</code> and <code>str2</code> and
      * returns a value indicating their relationship.
      *
      * @param str1 Null-terminated string to compare
      * @param str2 Null-terminated string to compare
      * @return The return value indicates the relation of <code>str1</code> to
      * <code>str2</code> as follows
      *  Less than 0 means <code>str1</code> is less than <code>str2</code>
      *  Equal to 0 means <code>str1</code> is identical to <code>str2</code>
      *  Greater than 0 means <code>str1</code> is more than <code>str2</code>
      */
    static int compareString
    (
        const   XMLCh* const    str1
        , const XMLCh* const    str2
    );

	/** Lexicographically compares <code>str1</code> and <code>str2</code>
	  * regions and returns true if they are equal, otherwise false.
	  *
      * A substring of <code>str1</code> is compared to a substring of
	  * <code>str2</code>. The result is true if these substrings represent
	  * identical character sequences. The substring of <code>str1</code>
      * to be compared begins at offset1 and has length charsCount. The
	  * substring of <code>str2</code> to be compared begins at offset2 and
	  * has length charsCount. The result is false if and only if at least
      * one of the following is true:
      *   offset1 is negative.
      *   offset2 is negative.
      *   offset1+charsCount is greater than the length of str1.
      *   offset2+charsCount is greater than the length of str2.
      *   There is some nonnegative integer k less than charsCount such that:
      *   str1.charAt(offset1+k) != str2.charAt(offset2+k)
      *
      * @param str1 Null-terminated string to compare
	  * @param offset1 Starting offset of str1
      * @param str2 Null-terminated string to compare
	  * @param offset2 Starting offset of str2
	  * @param charsCount The number of characters to compare
      * @return true if the specified subregion of <code>str1</code> exactly
	  *  matches the specified subregion of <code>str2></code>; false
	  *  otherwise.
      */
    static bool regionMatches
    (
        const   XMLCh* const    str1
		, const	int				offset1
        , const XMLCh* const    str2
		, const int				offset2
		, const unsigned int	charCount
    );

	/** Lexicographically compares <code>str1</code> and <code>str2</code>
	  * regions without regard to case and returns true if they are equal,
	  * otherwise false.
	  *
      * A substring of <code>str1</code> is compared to a substring of
	  * <code>str2</code>. The result is true if these substrings represent
	  * identical character sequences. The substring of <code>str1</code>
      * to be compared begins at offset1 and has length charsCount. The
	  * substring of <code>str2</code> to be compared begins at offset2 and
	  * has length charsCount. The result is false if and only if at least
      * one of the following is true:
      *   offset1 is negative.
      *   offset2 is negative.
      *   offset1+charsCount is greater than the length of str1.
      *   offset2+charsCount is greater than the length of str2.
      *   There is some nonnegative integer k less than charsCount such that:
      *   str1.charAt(offset1+k) != str2.charAt(offset2+k)
      *
      * @param str1 Null-terminated string to compare
	  * @param offset1 Starting offset of str1
      * @param str2 Null-terminated string to compare
	  * @param offset2 Starting offset of str2
	  * @param charsCount The number of characters to compare
      * @return true if the specified subregion of <code>str1</code> exactly
	  *  matches the specified subregion of <code>str2></code>; false
	  *  otherwise.
      */
    static bool regionIMatches
    (
        const   XMLCh* const    str1
		, const	int				offset1
        , const XMLCh* const    str2
		, const int				offset2
		, const unsigned int	charCount
    );
    //@}

    /** @name String copy functions */
    //@{
    /** Copies <code>src</code>, including the terminating null character, to the
      * location specified by <code>target</code>.
      *
      * No overflow checking is performed when strings are copied or appended.
      * The behavior of strcpy is undefined if the source and destination strings
      * overlap.
      *
      * @param target Destination string
      * @param src Null-terminated source string
      */
    static void copyString
    (
                char* const     target
        , const char* const     src
    );

    /** Copies <code>src</code>, including the terminating null character, to
      *   the location specified by <code>target</code>.
      *
      * No overflow checking is performed when strings are copied or appended.
      * The behavior of <code>copyString</code> is undefined if the source and
      * destination strings overlap.
      *
      * @param target Destination string
      * @param src Null-terminated source string
      */
    static void copyString
    (
                XMLCh* const    target
        , const XMLCh* const    src
    );

    /** Copies <code>src</code>, upto a fixed number of characters, to the
      * location specified by <code>target</code>.
      *
      * No overflow checking is performed when strings are copied or appended.
      * The behavior of <code>copyNString</code> is undefined if the source and
      * destination strings overlap.
      *
      * @param target Destination string. The size of the buffer should
      *        atleast be 'maxChars + 1'.
      * @param src Null-terminated source string
      * @param maxChars The maximum number of characters to copy
      */
    static bool copyNString
    (
                XMLCh* const    target
        , const XMLCh* const    src
        , const unsigned int    maxChars
    );
    //@}

    /** @name Hash functions */
    //@{
    /** Hashes a string given a modulus
      *
      * @param toHash The string to hash
      * @param hashModulus The divisor to be used for hashing
      * @return Returns the hash value
      */
    static unsigned int hash
    (
        const   char* const     tohash
        , const unsigned int    hashModulus
    );

    /** Hashes a string given a modulus
      *
      * @param toHash The string to hash
      * @param hashModulus The divisor to be used for hashing
      * @return Returns the hash value
      */
    static unsigned int hash
    (
        const   XMLCh* const    toHash
        , const unsigned int    hashModulus
    );

    /** Hashes a string given a modulus taking a maximum number of characters
      * as the limit
      *
      * @param toHash The string to hash
      * @param numChars The maximum number of characters to consider for hashing
      * @param hashModulus The divisor to be used for hashing
      *
      * @return Returns the hash value
      */
    static unsigned int hashN
    (
        const   XMLCh* const    toHash
        , const unsigned int    numChars
        , const unsigned int    hashModulus
    );

    //@}

    /** @name Search functions */
    //@{
    /**
      * Provides the index of the first occurance of a character within a string
      *
      * @param toSearch The string to search
      * @param ch The character to search within the string
      * @return If found, returns the index of the character within the string,
      * else returns -1.
      */
    static int indexOf(const char* const toSearch, const char ch);

    /**
      * Provides the index of the first occurance of a character within a string
      *
      * @param toSearch The string to search
      * @param ch The character to search within the string
      * @return If found, returns the index of the character within the string,
      * else returns -1.
      */
    static int indexOf(const XMLCh* const toSearch, const XMLCh ch);

	    /**
      * Provides the index of the first occurance of a character within a string
      * starting from a given index
      *
      * @param toSearch The string to search
      * @param chToFInd The character to search within the string
      * @param fromIndex The index to start earch from
      * @return If found, returns the index of the character within the string,
      * else returns -1.
      */
    static int indexOf
    (
        const   char* const     toSearch
        , const char            chToFind
        , const unsigned int    fromIndex
    );

    /**
      * Provides the index of the first occurance of a character within a string
      * starting from a given index
      *
      * @param toSearch The string to search
      * @param chToFInd The character to search within the string
      * @param fromIndex The index to start search from
      * @return If found, returns the index of the character within the string,
      * else returns -1.
      */
    static int indexOf
    (
        const   XMLCh* const    toSearch
        , const XMLCh           chToFind
        , const unsigned int    fromIndex
    );

    /**
      * Provides the index of the last occurance of a character within a string
      *
      * @param toSearch The string to search
      * @param ch The character to search within the string
      * @return If found, returns the index of the character within the string,
      * else returns -1.
      */
    static int lastIndexOf(const char* const toSearch, const char ch);

    /**
      * Provides the index of the last occurance of a character within a string
      *
      * @param toSearch The string to search
      * @param ch The character to search within the string
      * @return If found, returns the index of the character within the string,
      * else returns -1.
      */
    static int lastIndexOf(const XMLCh* const toSearch, const XMLCh ch);

    /**
      * Provides the index of the last occurance of a character within a string
      * starting backward from a given index
      *
      * @param toSearch The string to search
      * @param chToFInd The character to search within the string
      * @param fromIndex The index to start backward search from
      * @return If found, returns the index of the character within the string,
      * else returns -1.
      */
    static int lastIndexOf
    (
        const   char* const     toSearch
        , const char            chToFind
        , const unsigned int    fromIndex
    );

    /**
      * Provides the index of the last occurance of a character within a string
      * starting backward from a given index
      *
      * @param toSearch The string to search
      * @param chToFInd The character to search within the string
      * @param fromIndex The index to start backward search from
      * @return If found, returns the index of the character within the string,
      * else returns -1.
      */
    static int lastIndexOf
    (
        const   XMLCh* const    toSearch
        , const XMLCh           ch
        , const unsigned int    fromIndex
    );
    //@}

    /** @name Fixed size string movement */
    //@{
    /** Moves X number of chars
      * @param targetStr The string to copy the chars to
      * @param srcStr The string to copy the chars from
      * @param count The number of chars to move
      */
    static void moveChars
    (
                XMLCh* const    targetStr
        , const XMLCh* const    srcStr
        , const unsigned int    count
    );

    //@}

	    /** @name Substring function */
    //@{
    /** Create a substring of a givend string. The substring begins at the
      * specified beginIndex and extends to the character at index
      * endIndex - 1.
      * @param targetStr The string to copy the chars to
      * @param srcStr The string to copy the chars from
      * @param startIndex beginning index, inclusive.
      * @param endIndex the ending index, exclusive.
      */
    static void subString
    (
                char* const    targetStr
        , const char* const    srcStr
        , const int            startIndex
        , const int            endIndex
    );

    /** Create a substring of a givend string. The substring begins at the
      * specified beginIndex and extends to the character at index
      * endIndex - 1.
      * @param targetStr The string to copy the chars to
      * @param srcStr The string to copy the chars from
      * @param startIndex beginning index, inclusive.
      * @param endIndex the ending index, exclusive.
      */
    static void subString
    (
                XMLCh* const    targetStr
        , const XMLCh* const    srcStr
        , const int             startIndex
        , const int             endIndex
    );

    //@}

    /** @name Replication function */
    //@{
    /** Replicates a string
      * @param toRep The string to replicate
      * @return Returns a pointer to the replicated string
      */
    static char* replicate(const char* const toRep);

    /** Replicates a string
      * @param toRep The string to replicate
      * @return Returns a pointer to the replicated string
      */
    static XMLCh* replicate(const XMLCh* const toRep);

    //@}

    /** @name String query function */
    //@{
    /** Tells if the sub-string appears within a string at the beginning
      * @param toTest The string to test
      * @param prefix The sub-string that needs to be checked
      * @return Returns true if the sub-string was found at the beginning of
      * <code>toTest</code>, else false
      */
    static bool startsWith
    (
        const   char* const     toTest
        , const char* const     prefix
    );

    /** Tells if the sub-string appears within a string at the beginning
      * @param toTest The string to test
      * @param prefix The sub-string that needs to be checked
      * @return Returns true if the sub-string was found at the beginning of
      * <code>toTest</code>, else false
      */
    static bool startsWith
    (
        const   XMLCh* const    toTest
        , const XMLCh* const    prefix
    );

    /** Tells if the sub-string appears within a string at the beginning
      * without regard to case
      *
      * @param toTest The string to test
      * @param prefix The sub-string that needs to be checked
      * @return Returns true if the sub-string was found at the beginning of
      * <code>toTest</code>, else false
      */
    static bool startsWithI
    (
        const   char* const     toTest
        , const char* const     prefix
    );

    /** Tells if the sub-string appears within a string at the beginning
      * without regard to case
      *
      * @param toTest The string to test
      * @param prefix The sub-string that needs to be checked
      *
      * @return Returns true if the sub-string was found at the beginning
      * of <code>toTest</code>, else false
      */
    static bool startsWithI
    (
        const   XMLCh* const    toTest
        , const XMLCh* const    prefix
    );

    /** Tells if the sub-string appears within a string at the end.
      * @param toTest The string to test
      * @param suffix The sub-string that needs to be checked
      * @return Returns true if the sub-string was found at the end of
      * <code>toTest</code>, else false
      */
    static bool endsWith
    (
        const   XMLCh* const    toTest
        , const XMLCh* const    prefix
    );


    /** Tells if a string has any occurance of another string within itself
      * @param toSearch The string to be searched
      * @param searchList The sub-string to be searched within the string
      * @return Returns the pointer to the location where the sub-string was
      * found, else returns 0
      */
    static const XMLCh* findAny
    (
        const   XMLCh* const    toSearch
        , const XMLCh* const    searchList
    );

    /** Tells if a string has any occurance of another string within itself
      * @param toSearch The string to be searched
      * @param searchList The sub-string to be searched within the string
      * @return Returns the pointer to the location where the sub-string was
      * found, else returns 0
      */
    static XMLCh* findAny
    (
                XMLCh* const    toSearch
        , const XMLCh* const    searchList
    );

    /** Get the length of the string
      * @param src The string whose length is to be determined
      * @return Returns the length of the string
      */
    static unsigned int stringLen(const char* const src);

    /** Get the length of the string
      * @param src The string whose length is to be determined
      * @return Returns the length of the string
      */
    static unsigned int stringLen(const XMLCh* const src);

    /**
      * Checks whether an name is a valid NCName.
      * @param name The string to check its NCName validity
      * @return Returns true if name is NCName valid, otherwise false
      */
    static bool isValidNCName(const XMLCh* const name);

    /**
      * Checks whether an name is a valid Name.
      * @param name The string to check its Name validity
      * @return Returns true if name is Name valid, otherwise false
      */
    static bool isValidName(const XMLCh* const name);

    /**
      * Checks whether an name is a valid EncName.
      * @param name The string to check its EncName validity
      * @return Returns true if name is EncName valid, otherwise false
      */
    static bool isValidEncName(const XMLCh* const name);  

    /**
      * Checks whether a character is within [a-zA-Z].
      * @param theChar: the character to check 
      * @return Returns true if within the range, otherwise false
      */

    static bool isAlpha(XMLCh const theChar);

    /**
      * Checks whether a character is within [0-9].
      * @param theChar: the character to check 
      * @return Returns true if within the range, otherwise false
      */
    static bool isDigit(XMLCh const theChar);

    //@}

    /** @name Conversion functions */
    //@{

    /** Cut leading chars from a string
      *
      * @param toCut The string to cut chars from
      * @param count The count of leading chars to cut
      */
    static void cut
    (
                XMLCh* const    toCutFrom
        , const unsigned int    count
    );

    /** Transcodes a string to native code-page
      *
      * NOTE: The returned buffer is dynamically allocated and is the
      * responsibility of the caller to delete it when not longer needed.
      *
      * @param toTranscode The string to be transcoded
      * @return Returns the transcoded string
      */
    static char* transcode
    (
        const   XMLCh* const    toTranscode
    );

    /** Transcodes a string to native code-page
      *
      * Be aware that when transcoding to an external encoding, that each
      * Unicode char can create multiple output bytes. So you cannot assume
      * a one to one correspondence of input chars to output bytes.
      *
      * @param toTranscode The string tobe transcoded
      * @param toFill The buffer that is filled with the transcoded value.
      *        The size of this buffer should atleast be 'maxChars + 1'.
      * @param maxChars The maximum number of bytes that the output
      *         buffer can hold (not including the null, which is why
      *         toFill should be at least maxChars+1.) If the resulting
      *         output cannot fit into this many bytes, it is an error and
      *         false is returned.
      * @return Returns true if successful, false if there was an error
      */
    static bool transcode
    (
        const   XMLCh* const    toTranscode
        ,       char* const     toFill
        , const unsigned int    maxChars
    );

    /** Transcodes a string to native code-page
      *
      * NOTE: The returned buffer is dynamically allocated and is the
      * responsibility of the caller to delete it when not longer needed.
      *
      * @param toTranscode The string to be transcoded
      * @return Returns the transcoded string
      */
    static XMLCh* transcode
    (
        const   char* const     toTranscode
    );

    /** Transcodes a string to native code-page
      * @param toTranscode The string tobe transcoded
      * @param toFill The buffer that is filled with the transcoded value.
      *        The size of this buffer should atleast be 'maxChars + 1'.
      * @param maxChars The maximum number of characters that the output
      *         buffer can hold (not including the null, which is why
      *         toFill should be at least maxChars+1.) If the resulting
      *         output cannot fit into this many characters, it is an error
      *         and false is returned.
      * @return Returns true if successful, false if there was an error
      */
    static bool transcode
    (
        const   char* const     toTranscode
        ,       XMLCh* const    toFill
        , const unsigned int    maxChars
    );

    /** Trims off extra space characters from the start and end of the string,
      * moving the non-space string content back to the start.
      * @param toTrim The string to be trimmed. On return this contains the
      * trimmed string
      */
    static void trim(char* const toTrim);

    /** Trims off extra space characters from the start and end of the string,
      * moving the non-space string content back to the start.
      * @param toTrim The string to be trimmed. On return this contains
      * the trimmed string
      */
    static void trim(XMLCh* const toTrim);

    /** Break a string into tokens with space as delimiter, and
      * stored in a string vector.  The caller owns the string vector
      * that is returned, and is responsible for deleting it.
      * @param tokenizeStr String to be tokenized
      * @return a vector of all the tokenized string
      */
    static RefVectorOf<XMLCh>* tokenizeString(const XMLCh* const tokenizeSrc);

    /** Find is the string appears in the enum list
      * @param toFind the string to be found
      * @param enumList the list
      * return true if found
      */
    static bool isInList(const XMLCh* const toFind, const XMLCh* const enumList);

    //@}

    /** @name Formatting functions */
    //@{
    /** Creates a UName from a URI and base name. It is in the form
      * {url}name, and is commonly used internally to represent fully
      * qualified names when namespaces are enabled.
      *
      * @param pszURI The URI part of the name
      * @param pszName The base part of the name
      * @return Returns the complete formatted UName
      */
    static XMLCh* makeUName
    (
        const   XMLCh* const    pszURI
        , const XMLCh* const    pszName
    );

    /**
      * Internal function to perform token replacement for strings.
      *
      * @param errText The text (NULL terminated) where the replacement
      *        is to be done. The size of this buffer should be
      *        'maxChars + 1' to account for the final NULL.
      * @param maxChars The size of the output buffer, i.e. the maximum
      *         number of characters that it will hold. If the result is
      *         larger, it will be truncated.
      * @param text1 Replacement text-one
      * @param text2 Replacement text-two
      * @param text3 Replacement text-three
      * @param text4 Replacement text-four
      * @return Returns the count of characters that are outputted
      */
    static unsigned int replaceTokens
    (
                XMLCh* const    errText
        , const unsigned int    maxChars
        , const XMLCh* const    text1
        , const XMLCh* const    text2
        , const XMLCh* const    text3
        , const XMLCh* const    text4
    );

    /** Converts a string to uppercase
      * @param toUpperCase The string which needs to be converted to uppercase.
      *        On return, this buffer also holds the converted uppercase string
      */
    static void upperCase(XMLCh* const toUpperCase);

	/** Converts a string to lowercase
      * @param toLowerCase The string which needs to be converted to lowercase.
      *        On return, this buffer also holds the converted lowercase string
      */
    static void lowerCase(XMLCh* const toLowerCase);

	/** Check if string is WhiteSpace:replace
      * @param toCheck The string which needs to be checked.
      */
    static bool isWSReplaced(const XMLCh* const toCheck);

	/** Check if string is WhiteSpace:collapse
      * @param toCheck The string which needs to be checked.
      */
    static bool isWSCollapsed(const XMLCh* const toCheck);

	/** Replace whitespace
      * @param toCheck The string which needs to be whitespace replaced.
      *        On return , this buffer also holds the converted string
      */
    static void replaceWS(XMLCh* const toConvert);

	/** Collapse whitespace
      * @param toConvert The string which needs to be whitespace collapsed.
      *        On return , this buffer also holds the converted string
      */       
    static void collapseWS(XMLCh* const toConvert);
    //@}


private :

    /** @name Constructors and Destructor */
    //@{
    /** Unimplemented default constructor */
    XMLString();
    /** Unimplemented destructor */
    ~XMLString();
    //@}


    /** @name Initialization */
    //@{
    /** Init/Term methods called from XMLPlatformUtils class */
    static void initString(XMLLCPTranscoder* const defToUse);
    static void termString();
    //@}

	/**
	  * Called by regionMatches/regionIMatches to validate that we
	  * have a valid input
	  */
	static bool validateRegion(const XMLCh* const str1, const int offset1,
						const XMLCh* const str2, const int offset2,
						const unsigned int charsCount);

    friend class XMLPlatformUtils;
};


// ---------------------------------------------------------------------------
//  Inline some methods that are either just passthroughs to other string
//  methods, or which are key for performance.
// ---------------------------------------------------------------------------
inline void XMLString::moveChars(       XMLCh* const    targetStr
                                , const XMLCh* const    srcStr
                                , const unsigned int    count)
{
    XMLCh* outPtr = targetStr;
    const XMLCh* inPtr = srcStr;
    for (unsigned int index = 0; index < count; index++)
        *outPtr++ = *inPtr++;
}

inline unsigned int XMLString::stringLen(const XMLCh* const src)
{
    if (src == 0 || *src == 0)
    {
        return 0;
   }
    else
   {
        const XMLCh* pszTmp = src + 1;

        while (*pszTmp)
            ++pszTmp;

        return (unsigned int)(pszTmp - src);
    }
}

inline bool XMLString::startsWith(  const   XMLCh* const    toTest
                                    , const XMLCh* const    prefix)
{
    return (compareNString(toTest, prefix, stringLen(prefix)) == 0);
}

inline bool XMLString::startsWithI( const   XMLCh* const    toTest
                                    , const XMLCh* const    prefix)
{
    return (compareNIString(toTest, prefix, stringLen(prefix)) == 0);
}

inline bool XMLString::endsWith(const XMLCh* const toTest,
                                const XMLCh* const suffix)
{

    unsigned int suffixLen = XMLString::stringLen(suffix);

    return regionMatches(toTest, XMLString::stringLen(toTest) - suffixLen,
                         suffix, 0, suffixLen);
}

inline XMLCh* XMLString::replicate(const XMLCh* const toRep)
{
    // If a null string, return a null string!
    XMLCh* ret = 0;
    if (toRep)
    {
        const unsigned int len = stringLen(toRep);
        ret = new XMLCh[len + 1];
        XMLCh* outPtr = ret;
        const XMLCh* inPtr = toRep;
        for (unsigned int index = 0; index <= len; index++)
            *outPtr++ = *inPtr++;
    }
    return ret;
}

inline bool XMLString::validateRegion(const XMLCh* const str1,
									  const int offset1,
									  const XMLCh* const str2,
									  const int offset2,
									  const unsigned int charsCount)
{

	if (offset1 < 0 || offset2 < 0 ||
		(offset1 + charsCount) > XMLString::stringLen(str1) ||
		(offset2 + charsCount) > XMLString::stringLen(str2) )
		return false;

	return true;
}

#endif
