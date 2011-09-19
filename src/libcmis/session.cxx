#include <string>

#include <curl/curl.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include "session.hxx"

using namespace std;

namespace
{
    void lcl_RegisterNamespaces( xmlXPathContextPtr pXPathCtx )
    {
        xmlXPathRegisterNs( pXPathCtx, BAD_CAST( "app" ), BAD_CAST( "http://www.w3.org/2007/app" ) );
        xmlXPathRegisterNs( pXPathCtx, BAD_CAST( "atom" ), BAD_CAST( "http://www.w3.org/2005/Atom" ) );
        xmlXPathRegisterNs( pXPathCtx, BAD_CAST( "cmis" ), BAD_CAST( "http://docs.oasis-open.org/ns/cmis/core/200908/" ) );
        xmlXPathRegisterNs( pXPathCtx, BAD_CAST( "cmisra" ), BAD_CAST( "http://docs.oasis-open.org/ns/cmis/restatom/200908/" ) );
    }
}

AtomPubSession::AtomPubSession( string sAtomPubUrl )
{
    m_sAtomPubUrl = sAtomPubUrl;
    
    // Pull the content from sAtomPubUrl and parse it
    http_request( m_sAtomPubUrl, &AtomPubSession::parseServiceDocument );
}

AtomPubSession::~AtomPubSession( )
{
}

void AtomPubSession::readCollections( xmlNodeSetPtr pNodeSet )
{
}

size_t AtomPubSession::parseServiceDocument( void* pBuffer, size_t size, size_t nmemb, void* pUser_data )
{
    AtomPubSession& session = *( static_cast< AtomPubSession* >( pUser_data ) );

    xmlDocPtr pDoc = xmlReadMemory( ( const char * )pBuffer, size * nmemb, session.m_sAtomPubUrl.c_str(), NULL, 0 );

    if ( NULL != pDoc )
    {
        xmlXPathContextPtr pXPathCtx = xmlXPathNewContext( pDoc );

        // Register the Service Document namespaces
        lcl_RegisterNamespaces( pXPathCtx );

        if ( NULL != pXPathCtx )
        {
            xmlXPathObjectPtr pXPathObj = xmlXPathEvalExpression( BAD_CAST( "//app:collection" ), pXPathCtx );
            if ( NULL != pXPathObj )
                session.readCollections( pXPathObj->nodesetval );

            xmlXPathFreeObject( pXPathObj );
        }
        xmlXPathFreeContext( pXPathCtx );
        
    }
    else
        fprintf( stderr, "Failed to parse service document\n" );

    xmlFreeDoc( pDoc );
}

void AtomPubSession::http_request( string Url, size_t (*pCallback)( void *, size_t, size_t, void* ) )
{
    curl_global_init( CURL_GLOBAL_ALL );
    CURL* pHandle = curl_easy_init( );

    // Grab something from the web
    curl_easy_setopt( pHandle, CURLOPT_URL, Url.c_str() );
    curl_easy_setopt( pHandle, CURLOPT_WRITEFUNCTION, pCallback );
    curl_easy_setopt( pHandle, CURLOPT_WRITEDATA, this );

    // Perform the query
    curl_easy_perform( pHandle );

    curl_easy_cleanup( pHandle );
}
