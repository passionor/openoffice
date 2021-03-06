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



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_dbaccess.hxx"

/**************************************************************************
								TODO
 **************************************************************************

 *************************************************************************/

#include <vector>

#ifndef _UCBHELPER_CONTENTIDENTIFIER_HXX
#include <ucbhelper/contentidentifier.hxx>
#endif
#ifndef _UCBHELPER_PROVIDERHELPER_HXX
#include <ucbhelper/providerhelper.hxx>
#endif

#ifndef DBA_DATASUPPLIER_HXX
#include "myucp_datasupplier.hxx"
#endif
#ifndef DBA_CONTENTHELPER_HXX
#include "ContentHelper.hxx"
#endif
#ifndef _COM_SUN_STAR_CONTAINER_XHIERARCHICALNAMEACCESS_HPP_
#include <com/sun/star/container/XHierarchicalNameAccess.hpp>
#endif
#ifndef _TOOLS_DEBUG_HXX
#include <tools/debug.hxx>
#endif

using namespace ::com::sun::star::uno;
using namespace ::com::sun::star::ucb;
using namespace ::com::sun::star::beans;
using namespace ::com::sun::star::lang;
using namespace ::com::sun::star::sdbc;
using namespace ::com::sun::star::io;
using namespace ::com::sun::star::container;

// @@@ Adjust namespace name.
using namespace dbaccess;

// @@@ Adjust namespace name.
namespace dbaccess
{

//=========================================================================
//
// struct ResultListEntry.
//
//=========================================================================

struct ResultListEntry
{
	rtl::OUString						aId;
	Reference< XContentIdentifier >		xId;
	::rtl::Reference< OContentHelper > 	xContent;
	Reference< XRow > 					xRow;
	const ContentProperties& 			rData;

	ResultListEntry( const ContentProperties& rEntry ) : rData( rEntry ) {}
};

//=========================================================================
//
// ResultList.
//
//=========================================================================

typedef std::vector< ResultListEntry* > ResultList;

//=========================================================================
//
// struct DataSupplier_Impl.
//
//=========================================================================

struct DataSupplier_Impl
{
	osl::Mutex					                 m_aMutex;
	ResultList					                 m_aResults;
	rtl::Reference< ODocumentContainer >     	     m_xContent;
	Reference< XMultiServiceFactory >			 m_xSMgr;
  	sal_Int32					                 m_nOpenMode;
  	sal_Bool					                 m_bCountFinal;

	DataSupplier_Impl( const Reference< XMultiServiceFactory >& rxSMgr,
						const rtl::Reference< ODocumentContainer >& rContent,
					   sal_Int32 nOpenMode )
	: m_xContent(rContent)
	, m_xSMgr( rxSMgr )
	, m_nOpenMode( nOpenMode )
	, m_bCountFinal( sal_False ) {}
	~DataSupplier_Impl();
};

//=========================================================================
DataSupplier_Impl::~DataSupplier_Impl()
{
	ResultList::const_iterator it  = m_aResults.begin();
	ResultList::const_iterator end = m_aResults.end();

	while ( it != end )
	{
		delete (*it);
		it++;
	}
}

}

//=========================================================================
//=========================================================================
//
// DataSupplier Implementation.
//
//=========================================================================
//=========================================================================
DBG_NAME(DataSupplier)

DataSupplier::DataSupplier( const Reference< XMultiServiceFactory >& rxSMgr,
						   const rtl::Reference< ODocumentContainer >& rContent,
							sal_Int32 nOpenMode )
: m_pImpl( new DataSupplier_Impl( rxSMgr, rContent,nOpenMode ) )
{
    DBG_CTOR(DataSupplier,NULL);

}

//=========================================================================
// virtual
DataSupplier::~DataSupplier()
{

    DBG_DTOR(DataSupplier,NULL);
}

//=========================================================================
// virtual
rtl::OUString DataSupplier::queryContentIdentifierString( sal_uInt32 nIndex )
{
	osl::Guard< osl::Mutex > aGuard( m_pImpl->m_aMutex );

	if ( (size_t)nIndex < m_pImpl->m_aResults.size() )
	{
		rtl::OUString aId = m_pImpl->m_aResults[ nIndex ]->aId;
		if ( aId.getLength() )
		{
			// Already cached.
			return aId;
		}
	}

	if ( getResult( nIndex ) )
	{
		rtl::OUString aId
			= m_pImpl->m_xContent->getIdentifier()->getContentIdentifier();

		if ( aId.getLength() )
			aId += ::rtl::OUString(RTL_CONSTASCII_USTRINGPARAM("/"));

		aId += m_pImpl->m_aResults[ nIndex ]->rData.aTitle;

		m_pImpl->m_aResults[ nIndex ]->aId = aId;
		return aId;
	}
	return rtl::OUString();
}

//=========================================================================
// virtual
Reference< XContentIdentifier > 
DataSupplier::queryContentIdentifier( sal_uInt32 nIndex )
{
	osl::Guard< osl::Mutex > aGuard( m_pImpl->m_aMutex );

	if ( (size_t)nIndex < m_pImpl->m_aResults.size() )
	{
		Reference< XContentIdentifier > xId = m_pImpl->m_aResults[ nIndex ]->xId;
		if ( xId.is() )
		{
			// Already cached.
			return xId;
		}
	}

	rtl::OUString aId = queryContentIdentifierString( nIndex );
	if ( aId.getLength() )
	{
		Reference< XContentIdentifier > xId = new ::ucbhelper::ContentIdentifier( aId );
		m_pImpl->m_aResults[ nIndex ]->xId = xId;
		return xId;
	}
	return Reference< XContentIdentifier >();
}

//=========================================================================
// virtual
Reference< XContent > 
DataSupplier::queryContent( sal_uInt32 _nIndex )
{
	osl::Guard< osl::Mutex > aGuard( m_pImpl->m_aMutex );

	if ( (size_t)_nIndex < m_pImpl->m_aResults.size() )
	{
		Reference< XContent > xContent = m_pImpl->m_aResults[ _nIndex ]->xContent.get();
		if ( xContent.is() )
		{
			// Already cached.
			return xContent;
		}
	}

	Reference< XContentIdentifier > xId = queryContentIdentifier( _nIndex );
	if ( xId.is() )
	{
		try
		{
			Reference< XContent > xContent;
			::rtl::OUString sName = xId->getContentIdentifier();
			sal_Int32 nIndex = sName.lastIndexOf('/') + 1;
			sName = sName.getToken(0,'/',nIndex);

			m_pImpl->m_aResults[ _nIndex ]->xContent = m_pImpl->m_xContent->getContent(sName);
			
			xContent = m_pImpl->m_aResults[ _nIndex ]->xContent.get();
			return xContent;

		}
		catch ( IllegalIdentifierException& )
		{
		}
	}
	return Reference< XContent >();
}

//=========================================================================
// virtual
sal_Bool DataSupplier::getResult( sal_uInt32 nIndex )
{
	osl::ClearableGuard< osl::Mutex > aGuard( m_pImpl->m_aMutex );

	if ( (size_t)nIndex < m_pImpl->m_aResults.size() )
	{
		// Result already present.
		return sal_True;
	}

	// Result not (yet) present.

	if ( m_pImpl->m_bCountFinal )
		return sal_False;

	// Try to obtain result...

	sal_uInt32 nOldCount = m_pImpl->m_aResults.size();
	sal_Bool bFound = sal_False;
	sal_uInt32 nPos = nOldCount;

	// @@@ Obtain data and put it into result list...
	Sequence< ::rtl::OUString> aSeq = m_pImpl->m_xContent->getElementNames();
	if ( nIndex < sal::static_int_cast< sal_uInt32 >( aSeq.getLength() ) )
	{
		const ::rtl::OUString* pIter = aSeq.getConstArray();
		const ::rtl::OUString* pEnd	  = pIter + aSeq.getLength();
		for(pIter = pIter + nPos;pIter != pEnd;++pIter,++nPos)
		{
			m_pImpl->m_aResults.push_back(
							new ResultListEntry( m_pImpl->m_xContent->getContent(*pIter)->getContentProperties() ) );

			if ( nPos == nIndex )
			{
				// Result obtained.
				bFound = sal_True;
				break;
			}
		}
	}

	if ( !bFound )
		m_pImpl->m_bCountFinal = sal_True;

	rtl::Reference< ::ucbhelper::ResultSet > xResultSet = getResultSet().get();
	if ( xResultSet.is() )
	{
		// Callbacks follow!
		aGuard.clear();

		if ( (size_t)nOldCount < m_pImpl->m_aResults.size() )
			xResultSet->rowCountChanged(
									nOldCount, m_pImpl->m_aResults.size() );

		if ( m_pImpl->m_bCountFinal )
			xResultSet->rowCountFinal();
	}

	return bFound;
}

//=========================================================================
// virtual
sal_uInt32 DataSupplier::totalCount()
{
	osl::ClearableGuard< osl::Mutex > aGuard( m_pImpl->m_aMutex );

	if ( m_pImpl->m_bCountFinal )
		return m_pImpl->m_aResults.size();

	sal_uInt32 nOldCount = m_pImpl->m_aResults.size();

	// @@@ Obtain data and put it into result list...
	Sequence< ::rtl::OUString> aSeq = m_pImpl->m_xContent->getElementNames();
	const ::rtl::OUString* pIter = aSeq.getConstArray();
	const ::rtl::OUString* pEnd	  = pIter + aSeq.getLength();
	for(;pIter != pEnd;++pIter)
		m_pImpl->m_aResults.push_back(
						new ResultListEntry( m_pImpl->m_xContent->getContent(*pIter)->getContentProperties() ) );

	m_pImpl->m_bCountFinal = sal_True;

	rtl::Reference< ::ucbhelper::ResultSet > xResultSet = getResultSet().get();
	if ( xResultSet.is() )
	{
		// Callbacks follow!
		aGuard.clear();

		if ( (size_t)nOldCount < m_pImpl->m_aResults.size() )
			xResultSet->rowCountChanged(
									nOldCount, m_pImpl->m_aResults.size() );

		xResultSet->rowCountFinal();
	}

	return m_pImpl->m_aResults.size();
}

//=========================================================================
// virtual
sal_uInt32 DataSupplier::currentCount()
{
	return m_pImpl->m_aResults.size();
}

//=========================================================================
// virtual
sal_Bool DataSupplier::isCountFinal()
{
	return m_pImpl->m_bCountFinal;
}

//=========================================================================
// virtual
Reference< XRow > 
DataSupplier::queryPropertyValues( sal_uInt32 nIndex  )
{
	osl::Guard< osl::Mutex > aGuard( m_pImpl->m_aMutex );

	if ( (size_t)nIndex < m_pImpl->m_aResults.size() )
	{
		Reference< XRow > xRow = m_pImpl->m_aResults[ nIndex ]->xRow;
		if ( xRow.is() )
		{
			// Already cached.
			return xRow;
		}
	}

	if ( getResult( nIndex ) )
	{
		if ( !m_pImpl->m_aResults[ nIndex ]->xContent.is() )
			queryContent(nIndex);

		Reference< XRow > xRow = m_pImpl->m_aResults[ nIndex ]->xContent->getPropertyValues(getResultSet()->getProperties());
		m_pImpl->m_aResults[ nIndex ]->xRow = xRow;
		return xRow;
	}

	return Reference< XRow >();
}

//=========================================================================
// virtual
void DataSupplier::releasePropertyValues( sal_uInt32 nIndex )
{
	osl::Guard< osl::Mutex > aGuard( m_pImpl->m_aMutex );

	if ( (size_t)nIndex < m_pImpl->m_aResults.size() )
		m_pImpl->m_aResults[ nIndex ]->xRow = Reference< XRow >();
}

//=========================================================================
// virtual
void DataSupplier::close()
{
}

//=========================================================================
// virtual
void DataSupplier::validate()
	throw( ResultSetException )
{
}

