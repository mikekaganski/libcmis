/* libcmis
 * Version: MPL 1.1 / GPLv2+ / LGPLv2+
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License or as specified alternatively below. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * Major Contributor(s):
 * Copyright (C) 2011 SUSE <cbosdonnat@suse.com>
 *
 *
 * All Rights Reserved.
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPLv2+"), or
 * the GNU Lesser General Public License Version 2 or later (the "LGPLv2+"),
 * in which case the provisions of the GPLv2+ or the LGPLv2+ are applicable
 * instead of those above.
 */

#include "ws-folder.hxx"

using namespace std;


WSFolder::WSFolder( const WSObject& object ) :
    libcmis::Object( object ),
    libcmis::Folder( const_cast< WSObject& >( object ).getSession( ) ),
    WSObject( object )
{
}

WSFolder::~WSFolder( )
{
}

vector< libcmis::ObjectPtr > WSFolder::getChildren( ) throw ( libcmis::Exception )
{
    string repoId = getSession( )->getRepositoryId( );
    return getSession( )->getNavigationService( ).getChildren( repoId, getId( ) );
}

libcmis::FolderPtr WSFolder::createFolder( const map< string, libcmis::PropertyPtr >& properties )
    throw ( libcmis::Exception )
{
    libcmis::FolderPtr empty;

    // TODO Implement me

    return empty;
}

libcmis::DocumentPtr WSFolder::createDocument( const map< string, libcmis::PropertyPtr >& properties,
                        boost::shared_ptr< ostream > os, string contentType ) throw ( libcmis::Exception )
{
    libcmis::DocumentPtr empty;

    // TODO Implement me

    return empty;
}

vector< string > WSFolder::removeTree( bool allVersion, libcmis::UnfileObjects::Type unfile, bool continueOnError )
    throw ( libcmis::Exception )
{
    string repoId = getSession( )->getRepositoryId( );
    return getSession( )->getObjectService( ).deleteTree( repoId, getId( ), allVersion, unfile, continueOnError );
}