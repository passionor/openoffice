/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/


#ifndef _XMLOFF_XFORMSEXPORT_HXX
#define _XMLOFF_XFORMSEXPORT_HXX

#include "xmloff/dllapi.h"

class SvXMLExport;
namespace com { namespace sun { namespace star {
    namespace uno { template<typename T> class Reference; }
    namespace uno { template<typename T> class Sequence; }
    namespace frame { class XModel; }
    namespace beans { class XPropertySet; struct PropertyValue; }
    namespace container { class XNameAccess; }
} } }
namespace rtl { class OUString; }


/** export an XForms model. */
void SAL_DLLPRIVATE exportXForms( SvXMLExport& );

rtl::OUString SAL_DLLPRIVATE getXFormsBindName( const com::sun::star::uno::Reference<com::sun::star::beans::XPropertySet>& xBinding );

rtl::OUString SAL_DLLPRIVATE getXFormsListBindName( const com::sun::star::uno::Reference<com::sun::star::beans::XPropertySet>& xBinding );

rtl::OUString SAL_DLLPRIVATE getXFormsSubmissionName( const com::sun::star::uno::Reference<com::sun::star::beans::XPropertySet>& xBinding );


/** returns the settings of the given XForms container, to be exported as document specific settings
*/
void XMLOFF_DLLPUBLIC getXFormsSettings(
        const ::com::sun::star::uno::Reference< ::com::sun::star::container::XNameAccess >& _rXForms,
              ::com::sun::star::uno::Sequence< ::com::sun::star::beans::PropertyValue >& _out_rSettings
    );

#endif
