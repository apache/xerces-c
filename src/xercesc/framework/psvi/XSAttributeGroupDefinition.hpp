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
 * Revision 1.8  2004/09/08 13:56:08  peiyongz
 * Apache License Version 2.0
 *
 * Revision 1.7  2003/12/01 23:23:26  neilg
 * fix for bug 25118; thanks to Jeroen Witmond
 *
 * Revision 1.6  2003/11/21 22:34:45  neilg
 * More schema component model implementation, thanks to David Cargill.
 * In particular, this cleans up and completes the XSModel, XSNamespaceItem,
 * XSAttributeDeclaration and XSAttributeGroup implementations.
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

#if !defined(XSATTRIBUTEGROUPDEFINITION_HPP)
#define XSATTRIBUTEGROUPDEFINITION_HPP

#include <xercesc/framework/psvi/XSObject.hpp>

XERCES_CPP_NAMESPACE_BEGIN

/**
 * This class describes all properties of a Schema Attribute
 * Group Definition component.
 * This is *always* owned by the validator /parser object from which
 * it is obtained.  
 */

// forward declarations
class XSAnnotation;
class XSAttributeUse;
class XSWildcard;
class XercesAttGroupInfo;

class XMLPARSER_EXPORT XSAttributeGroupDefinition : public XSObject
{
public:

    //  Constructors and Destructor
    // -----------------------------------------------------------------------
    /** @name Constructors */
    //@{

    /**
      * The default constructor 
      *
      * @param  xercesAttGroupInfo
      * @param  xsAttList
      * @param  xsWildcard
      * @param  xsAnnot
      * @param  xsModel
      * @param  manager     The configurable memory manager
      */
    XSAttributeGroupDefinition
    (
        XercesAttGroupInfo* const   xercesAttGroupInfo
        , XSAttributeUseList* const xsAttList
        , XSWildcard* const         xsWildcard
        , XSAnnotation* const       xsAnnot
        , XSModel* const            xsModel
        , MemoryManager* const      manager = XMLPlatformUtils::fgMemoryManager
    );

    //@};

    /** @name Destructor */
    //@{
    ~XSAttributeGroupDefinition();
    //@}

    //---------------------
    /** @name overridden XSObject methods */
    //@{

    /**
     * The name of type <code>NCName</code> of this declaration as defined in 
     * XML Namespaces.
     */
    const XMLCh* getName();

    /**
     *  The [target namespace] of this object, or <code>null</code> if it is 
     * unspecified. 
     */
    const XMLCh* getNamespace();

    /**
     * A namespace schema information item corresponding to the target 
     * namespace of the component, if it's globally declared; or null 
     * otherwise.
     */
    XSNamespaceItem* getNamespaceItem();

    //@}

    //--------------------- 
    /** @name XSAttributeGroupDefinition methods */

    //@{

    /**
     * A set of [attribute uses]. 
     */
    XSAttributeUseList *getAttributeUses();

    /**
     * Optional. A [wildcard]. 
     */
    XSWildcard *getAttributeWildcard() const;

    /**
     * Optional. An [annotation]. 
     */
    XSAnnotation *getAnnotation() const;

    //@}

    //----------------------------------
    /** methods needed by implementation */

    //@{

    //@}
private:

    // -----------------------------------------------------------------------
    //  Unimplemented constructors and operators
    // -----------------------------------------------------------------------
    XSAttributeGroupDefinition(const XSAttributeGroupDefinition&);
    XSAttributeGroupDefinition & operator=(const XSAttributeGroupDefinition &);

protected:

    // -----------------------------------------------------------------------
    //  data members
    // -----------------------------------------------------------------------
    XercesAttGroupInfo*     fXercesAttGroupInfo;
    XSAttributeUseList*     fXSAttributeUseList;
    XSWildcard*             fXSWildcard;
    XSAnnotation*           fAnnotation;
};

inline XSAttributeUseList* XSAttributeGroupDefinition::getAttributeUses()
{
    return fXSAttributeUseList;
}

inline XSWildcard* XSAttributeGroupDefinition::getAttributeWildcard() const
{
    return fXSWildcard;
}

inline XSAnnotation* XSAttributeGroupDefinition::getAnnotation() const
{
    return fAnnotation;
}

XERCES_CPP_NAMESPACE_END

#endif
