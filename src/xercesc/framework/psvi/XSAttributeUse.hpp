/*
 * Copyright 2003,2004 The Apache Software Foundation.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * $Log$
 * Revision 1.10  2005/01/07 15:22:13  amassari
 * Removed warnings
 *
 * Revision 1.9  2004/09/08 13:56:08  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.8  2004/09/02 19:08:09  cargilld
 * Fix API Doc warning message
 *
 * Revision 1.7  2003/12/19 23:02:43  cargilld
 * Fix compiler messages on OS390.
 *
 * Revision 1.6  2003/12/17 20:50:34  knoaman
 * PSVI: fix for annotation of attributes in attributeGroup/derived types
 *
 * Revision 1.5  2003/11/21 17:19:30  knoaman
 * PSVI update.
 *
 * Revision 1.4  2003/11/14 22:47:53  neilg
 * fix bogus log message from previous commit...
 *
 * Revision 1.3  2003/11/14 22:33:30  neilg
 * Second phase of schema component model implementation.  
 * Implement XSModel, XSNamespaceItem, and the plumbing necessary
 * to connect them to the other components.
 * Thanks to David Cargill.
 *
 * Revision 1.2  2003/11/06 15:30:04  neilg
 * first part of PSVI/schema component model implementation, thanks to David Cargill.  This covers setting the PSVIHandler on parser objects, as well as implementing XSNotation, XSSimpleTypeDefinition, XSIDCDefinition, and most of XSWildcard, XSComplexTypeDefinition, XSElementDeclaration, XSAttributeDeclaration and XSAttributeUse.
 *
 * Revision 1.1  2003/09/16 14:33:36  neilg
 * PSVI/schema component model classes, with Makefile/configuration changes necessary to build them
 *
 */

#if !defined(XSATTRIBUTEUSE_HPP)
#define XSATTRIBUTEUSE_HPP

#include <xercesc/framework/psvi/XSObject.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/**
 * This class describes all properties of a Schema Attribute
 * Use component.
 * This is *always* owned by the validator /parser object from which
 * it is obtained.  
 */

// forward declarations
class XSAttributeDeclaration;

class XMLPARSER_EXPORT XSAttributeUse : public XSObject
{
public:

    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    /** @name Constructors */
    //@{

    /**
      * The default constructor 
      * @param  xsAttDecl
      * @param  xsModel
      * @param  manager     The configurable memory manager
      */
    XSAttributeUse
    (
        XSAttributeDeclaration* const xsAttDecl,
        XSModel* const xsModel,
        MemoryManager* const manager = XMLPlatformUtils::fgMemoryManager
    );

    //@};

    /** @name Destructor */
    //@{
    ~XSAttributeUse();
    //@}

    //---------------------
    /** @name XSAttributeUse methods */

    //@{

    /**
     * [required]: determines whether this use of an attribute declaration 
     * requires an appropriate attribute information item to be present, or 
     * merely allows it. 
     */
    bool getRequired() const;

    /**
     * [attribute declaration]: provides the attribute declaration itself, 
     * which will in turn determine the simple type definition used. 
     */
    XSAttributeDeclaration *getAttrDeclaration() const;

    /**
     * Value Constraint: one of default, fixed. 
     */
    XSConstants::VALUE_CONSTRAINT getConstraintType() const;

    /**
     * Value Constraint: The actual value. 
     */
    const XMLCh *getConstraintValue();

    //@}

    //----------------------------------
    /** methods needed by implementation */

    //@{

    //@}

private:

    // set data
    void set
    (
        const bool isRequired
        , XSConstants::VALUE_CONSTRAINT constraintType
        , const XMLCh* const constraintValue
    );

    friend class XSObjectFactory;
    
    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XSAttributeUse(const XSAttributeUse&);
    XSAttributeUse & operator=(const XSAttributeUse &);

protected:

    // -----------------------------------------------------------------------
    //  data members
    // -----------------------------------------------------------------------
    bool                          fRequired;
    XSConstants::VALUE_CONSTRAINT fConstraintType;   
    const XMLCh*                  fConstraintValue;
    XSAttributeDeclaration*       fXSAttributeDeclaration;
};

inline XSAttributeDeclaration *XSAttributeUse::getAttrDeclaration() const
{
    return fXSAttributeDeclaration;
}

inline bool XSAttributeUse::getRequired() const
{
    return fRequired;
}

inline XSConstants::VALUE_CONSTRAINT XSAttributeUse::getConstraintType() const
{
    return fConstraintType;
}

inline const XMLCh *XSAttributeUse::getConstraintValue()
{
    return fConstraintValue;
}

XERCES_CPP_NAMESPACE_END

#endif
