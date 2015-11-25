/*	File:		MoreAEObjects.c	Contains:	Functions to help you when you are working with Apple event objects.	DRI:		George Warner	Copyright:	Copyright (c) 2000-2001 by Apple Computer, Inc., All Rights Reserved.	Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc.				("Apple") in consideration of your agreement to the following terms, and your				use, installation, modification or redistribution of this Apple software				constitutes acceptance of these terms.  If you do not agree with these terms,				please do not use, install, modify or redistribute this Apple software.				In consideration of your agreement to abide by the following terms, and subject				to these terms, Apple grants you a personal, non-exclusive license, under Apple�s				copyrights in this original Apple software (the "Apple Software"), to use,				reproduce, modify and redistribute the Apple Software, with or without				modifications, in source and/or binary forms; provided that if you redistribute				the Apple Software in its entirety and without modifications, you must retain				this notice and the following text and disclaimers in all such redistributions of				the Apple Software.  Neither the name, trademarks, service marks or logos of				Apple Computer, Inc. may be used to endorse or promote products derived from the				Apple Software without specific prior written permission from Apple.  Except as				expressly stated in this notice, no other rights or licenses, express or implied,				are granted by Apple herein, including but not limited to any patent rights that				may be infringed by your derivative works or by other works in which the Apple				Software may be incorporated.				The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO				WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED				WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR				PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN				COMBINATION WITH YOUR PRODUCTS.				IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR				CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE				GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)				ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION				OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT				(INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN				ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.	Change History (most recent first):$Log: MoreAEObjects.c,v $Revision 1.14  2002/11/08 22:47:57  eskimo1Convert nil to NULL. Accomodate MoreAEDataModel change. We no longer include "MoreSetup.h", but instead rely oun our header to do it for us.Revision 1.13  2002/10/16 20:29:45  geowarMoreAEOCreateObjSpecifierFromCFURLRef now returns a cFile for a file and a generic cObject for everything else.Revision 1.12  2002/10/03 00:05:04  geowarMoreAEOCreateObjSpecifierFromCFURLRef now returns generic cObject instead of specific cFile or cFolder.Revision 1.11  2002/03/13 19:13:58  geowaraliasHandle => tAliasHandleRevision 1.10  2002/03/07 20:27:29  geowarGeneral clean up and added API's:MoreAEOCreateAliasObjectFromFSRefMoreAEOCreateObjectSpecifierFromFSSpecMoreAEOCreateObjectSpecifierFromFSRefRevision 1.9  2002/02/19 18:53:24  geowarAEDisposeDesc => MoreAEDisposeDescAdded MoreAEOAddAliasParameterFromFSRefAdded MoreAEOCreateAliasDescFromFSRefRevision 1.8  2002/01/16 19:05:09  geowarerr => anErr and (anErr ?= noErr) => (noErr ?= anErr)Revision 1.7  2001/11/07 15:50:37  eskimo1Tidy up headers, add CVS logs, update copyright.         <6>     21/9/01    Quinn   Get rid of wacky Finder label.         <5>     21/9/01    Quinn   Changes for CWPro7 Mach-O build.         <4>     9/13/01    gaw     Always compile your code before checking in chages!         <3>     9/13/01    gaw     AEPutPtr(typeInteger) should use pointers to longs, not shorts                                    (&(pPosition.v ,.h)).         <2>     15/2/01    Quinn   Eliminate redundant code and includes. Prompted by gcc warning.         <1>      3/9/00    GW      Intergrating AppleEvent Helper code. First Check In*///*******************************************************************************//**********	Our Prototypes 	    	****************************************#include "MoreAEObjects.h"//**********	Universal Headers		****************************************#if ! MORE_FRAMEWORK_INCLUDES	#include <AERegistry.h>	#include <AEObjects.h>	#include <AEPackObject.h>	#include <Aliases.h>	#include <FinderRegistry.h>	#include <Processes.h>#endif//**********	Project Headers			****************************************#include "MoreAEDataModel.h"/********************************************************************************	Add a parameter of type typeAlias to an AERecord (or AppleEvent) using the provided FSSpec.	pFSSpec			input:	Pointer to the FSSpec to use.	pKeyword		input:	The key for the data to be added to the record.	pAERecord		input:	Pointer to the record (or event) to add the data to.		RESULT CODES	____________	noErr			   0	No error		paramErr		 -50	The value of target or alias parameter, or of							both, is NIL, or the alias record is corrupt	memFullErr		-108	Not enough room in heap zone	*/pascal	OSErr	MoreAEOAddAliasParameterFromFSSpec(const FSSpecPtr pFSSpec,												const DescType pKeyword,												AERecord *pAERecord ){	OSErr			anErr = noErr;	AliasHandle		tAliasHandle;		anErr = NewAlias( NULL, pFSSpec, &tAliasHandle);	if ( noErr == anErr  &&  tAliasHandle == NULL )	{		anErr = paramErr;	}		if ( noErr == anErr )	{		char	handleState;				handleState = HGetState((Handle) tAliasHandle );		HLock((Handle) tAliasHandle );				anErr = AEPutParamPtr( pAERecord, pKeyword, typeAlias,							   *tAliasHandle, (*tAliasHandle)->aliasSize);				HSetState( (Handle)tAliasHandle, handleState );		DisposeHandle( (Handle)tAliasHandle );	}			return anErr;}//end AddAliasParameterFromFSS/********************************************************************************	Add a parameter of type typeAlias to an AERecord (or AppleEvent) using the provided FSRef.	pFSRef			input:	Pointer to the FSRef to use.	pKeyword		input:	The key for the data to be added to the record.	pAERecord		input:	Pointer to the record (or event) to add the data to.		RESULT CODES	____________	noErr			   0	No error		paramErr		 -50	The value of target or alias parameter, or of							both, is NIL, or the alias record is corrupt	memFullErr		-108	Not enough room in heap zone	*/pascal	OSErr	MoreAEOAddAliasParameterFromFSRef(const FSRefPtr pFSRef,												const DescType pKeyword,												AERecord *pAERecord ){	OSErr			anErr = noErr;	AliasHandle		tAliasHandle;	anErr = FSNewAlias( NULL, pFSRef, &tAliasHandle);	if ( noErr == anErr  &&  tAliasHandle == NULL )	{		anErr = paramErr;	}		if ( noErr == anErr )	{		char	handleState;				handleState = HGetState((Handle) tAliasHandle );		HLock((Handle) tAliasHandle );				anErr = AEPutParamPtr( pAERecord, pKeyword, typeAlias,							   *tAliasHandle, (*tAliasHandle)->aliasSize);				HSetState( (Handle)tAliasHandle, handleState );		DisposeHandle( (Handle)tAliasHandle );	}			return anErr;}//end MoreAEOAddAliasParameterFromFSRef/********************************************************************************	Create and return an AEDesc of type typeAlias using the provided FSSpec.	pFSSpec			input:	Pointer to the FSSpec to use.	pAliasAEDesc	input:	Pointer to null AEDesc.					output:	an AEDesc of type typeAlias.		RESULT CODES	____________	noErr			   0	No error		paramErr		 -50	The value of target or alias parameter, or of							both, is NIL, or the alias record is corrupt	memFullErr		-108	Not enough room in heap zone	*/pascal	OSErr	MoreAEOCreateAliasDescFromFSSpec( const FSSpecPtr pFSSpec,												  AEDesc *pAliasAEDesc ){	OSErr			anErr = noErr;	AliasHandle		tAliasHandle;		anErr = NewAlias( NULL, pFSSpec, &tAliasHandle);	if ( noErr == anErr  &&  tAliasHandle == NULL )	{		anErr = paramErr;	}	if ( noErr == anErr )	{		anErr = MoreAEOCreateAliasDesc( tAliasHandle, pAliasAEDesc );		DisposeHandle( (Handle)tAliasHandle );	}			return anErr;}//end MoreAEOCreateAliasDescFromFSSpec/********************************************************************************	Create and return an AEDesc of type typeAlias using the provided FSRef.	pFSRef			input:	Pointer to the FSRef to use.	pAliasAEDesc	input:	Pointer to null AEDesc.					output:	an AEDesc of type typeAlias.		RESULT CODES	____________	noErr			   0	No error		paramErr		 -50	The value of target or alias parameter, or of							both, is NIL, or the alias record is corrupt	memFullErr		-108	Not enough room in heap zone	*/pascal	OSErr	MoreAEOCreateAliasDescFromFSRef( const FSRefPtr pFSRef,												  AEDesc *pAliasAEDesc ){	OSErr			anErr = noErr;	AliasHandle		tAliasHandle;		anErr = FSNewAlias( NULL, pFSRef, &tAliasHandle);	if ( noErr == anErr  &&  tAliasHandle == NULL )	{		anErr = paramErr;	}	if ( noErr == anErr )	{		anErr = MoreAEOCreateAliasDesc( tAliasHandle, pAliasAEDesc );		DisposeHandle( (Handle)tAliasHandle );	}			return anErr;}//end MoreAEOCreateAliasDescFromFSRef/********************************************************************************	Create and return an AEDesc of type typeAlias using the provided 	alias record.	pAliasHdl		input:	Handle to an alias record.	pAliasAEDesc	input:	Pointer to null AEDesc.					output:	an AEDesc of type typeAlias.		RESULT CODES	____________	noErr			   0	No error		memFullErr		-108	Not enough room in heap zone	*/pascal	OSErr	MoreAEOCreateAliasDesc( const AliasHandle pAliasHdl,										AEDesc *pAliasAEDesc ){	OSErr	anErr = noErr;		char	handleState = HGetState( (Handle)pAliasHdl );	HLock( (Handle)pAliasHdl );		anErr = AECreateDesc( typeAlias, *pAliasHdl, GetHandleSize( (Handle)pAliasHdl ), pAliasAEDesc );		HSetState( (Handle)pAliasHdl, handleState );		return anErr;}//end MoreAEOCreateAliasDesc/********************************************************************************	Given an FSSpec, return an object descriptor containing an alias,	contained by containerObj.		pFSSpec				input:	Pointer to the FSSpec to use.	pContainerAEDesc	input:	Pointer to container object for object being created.	pAliasObjectAEDesc		input:	Pointer to null AEDesc.						output:	an alias object.		RESULT CODES	____________	noErr				    0	No error		paramErr			  -50	The value of target or alias parameter, or of								both, is NIL, or the alias record is corrupt	memFullErr			 -108	Not enough room in heap zone		errAECoercionFail 	-1700	Data could not be coerced to the requested 								Apple event data type		errAEWrongDataType	-1703	Wrong Apple event data type		errAENotAEDesc		-1704	Not a valid descriptor record		errAEBadListItem	-1705	Operation involving a list item failed	*/pascal	OSErr	MoreAEOCreateAliasObjectFromFSSpec( const FSSpecPtr pFSSpec,													AEDesc *pContainerAEDesc,													AEDesc *pAliasObjectAEDesc ){	OSErr		anErr = noErr;	AEDesc		aliasDesc = {typeNull,NULL};	anErr = MoreAEOCreateAliasDescFromFSSpec( pFSSpec, &aliasDesc );	if ( noErr == anErr )	{		anErr = CreateObjSpecifier( typeAlias, pContainerAEDesc, formAbsolutePosition,									&aliasDesc, false, pAliasObjectAEDesc );		MoreAEDisposeDesc( &aliasDesc );	}		return anErr;}//end MoreAEOCreateAliasObjectFromFSSpec/********************************************************************************	Given an FSRef, return an object descriptor containing an alias,	contained by containerObj.		pFSRef				input:	Pointer to the FSRef to use.	pContainerAEDesc	input:	Pointer to container object for object being created.	pAliasObjectAEDesc		input:	Pointer to null AEDesc.						output:	an alias object.		RESULT CODES	____________	noErr				    0	No error		paramErr			  -50	The value of target or alias parameter, or of								both, is NIL, or the alias record is corrupt	memFullErr			 -108	Not enough room in heap zone		errAECoercionFail 	-1700	Data could not be coerced to the requested 								Apple event data type		errAEWrongDataType	-1703	Wrong Apple event data type		errAENotAEDesc		-1704	Not a valid descriptor record		errAEBadListItem	-1705	Operation involving a list item failed	*/pascal	OSErr	MoreAEOCreateAliasObjectFromFSRef( const FSRefPtr pFSRef,													AEDesc *pContainerAEDesc,													AEDesc *pAliasObjectAEDesc ){	OSErr		anErr = noErr;	AEDesc		aliasDesc = {typeNull,NULL};	anErr = MoreAEOCreateAliasDescFromFSRef( pFSRef, &aliasDesc );	if ( noErr == anErr )	{		anErr = CreateObjSpecifier( typeAlias, pContainerAEDesc, formAbsolutePosition,									&aliasDesc, false, pAliasObjectAEDesc );		MoreAEDisposeDesc( &aliasDesc );	}		return anErr;}//end MoreAEOCreateAliasObjectFromFSRef/********************************************************************************	Given an AliasHandle, return an object descriptor containing an alias,	contained by containerObj.		pAliasHdl		input:	Handle to an alias record.	pContainerAEDesc	input:	Pointer to container object for object being created.	pAliasObjectAEDesc	input:	Pointer to null AEDesc.					output:	an alias object.		RESULT CODES	____________	noErr				    0	No error		paramErr			  -50	Error in parameter list	memFullErr			 -108	Not enough room in heap zone		errAECoercionFail 	-1700	Data could not be coerced to the requested 								Apple event data type		errAEWrongDataType	-1703	Wrong Apple event data type		errAENotAEDesc		-1704	Not a valid descriptor record		errAEBadListItem	-1705	Operation involving a list item failed	*/pascal	OSErr	MoreAEOCreateAliasObject( const AliasHandle pAliasHdl,										  AEDesc *pContainerAEDesc,										  AEDesc *pAliasObjectAEDesc ){	OSErr	anErr = noErr;	AEDesc	aliasDesc;	anErr = MoreAEOCreateAliasDesc( pAliasHdl, &aliasDesc );	if ( noErr == anErr )	{		anErr = CreateObjSpecifier( typeAlias, pContainerAEDesc, formAbsolutePosition,									&aliasDesc, false, pAliasObjectAEDesc );		MoreAEDisposeDesc( &aliasDesc );	}		return anErr;}//end MoreAEOCreateAliasObject/********************************************************************************	Given a property type, create an new object descriptor for that property,	contained by containerObj.		pPropertyType		input:	Property type to use for object.	pContainerAEDesc	input:	Pointer to container object for object being created.	propertyObjPtr	input:	Pointer to null AEDesc.					output:	A property object.		RESULT CODES	____________	noErr				    0	No error		paramErr			  -50	Error in parameter list	memFullErr			 -108	Not enough room in heap zone		errAECoercionFail 	-1700	Data could not be coerced to the requested 								Apple event data type		errAEWrongDataType	-1703	Wrong Apple event data type		errAENotAEDesc		-1704	Not a valid descriptor record		errAEBadListItem	-1705	Operation involving a list item failed	*/pascal	OSErr	MoreAEOCreatePropertyObject( const DescType pPropertyType,											 AEDesc *pContainerAEDesc,											 AEDesc *propertyObjPtr ){	OSErr	anErr = noErr;	AEDesc	propDesc;		anErr = AECreateDesc( typeType, &pPropertyType, sizeof( pPropertyType ), &propDesc );	if ( noErr == anErr )	{		anErr = CreateObjSpecifier( cProperty, pContainerAEDesc, formPropertyID,									&propDesc, false, propertyObjPtr );		MoreAEDisposeDesc( &propDesc );	}		return anErr;}//end MoreAEOCreatePropertyObject/********************************************************************************	Given a ProcessSerialNumber, create an new object descriptor for the PSN,	contained by containerObj.		pPSN			input:	ProcessSerialNumber to use for object.	pContainerAEDesc	input:	Pointer to container object for object being created.	pPSNObjDesc		input:	Pointer to null AEDesc.					output:	A ProcessSerialNumber object.		RESULT CODES	____________	noErr				    0	No error		paramErr			  -50	Error in parameter list	memFullErr			 -108	Not enough room in heap zone		errAECoercionFail 	-1700	Data could not be coerced to the requested 								Apple event data type		errAEWrongDataType	-1703	Wrong Apple event data type		errAENotAEDesc		-1704	Not a valid descriptor record		errAEBadListItem	-1705	Operation involving a list item failed	*/pascal	OSErr	MoreAEOCreateProcessObject( const ProcessSerialNumber *pPSN,											AEDesc *pContainerAEDesc,											AEDesc *pPSNObjDesc ){	OSErr	anErr = noErr;	AEDesc	psnDesc;		anErr = AECreateDesc( typeProcessSerialNumber, pPSN, sizeof( ProcessSerialNumber ), &psnDesc );	if ( noErr == anErr )	{		anErr = CreateObjSpecifier( cProperty, pContainerAEDesc, formPropertyID,									&psnDesc, false, pPSNObjDesc );		MoreAEDisposeDesc( &psnDesc );	}		return anErr;}//end MoreAEOCreateProcessObject/********************************************************************************	Given selection type, create an new object descriptor for a selection,	contained by containerObj.		pSelectionAEDesc	input:	Selection type to use for object.	pContainerAEDesc	input:	Pointer to container object for object being created.	pSelectionObject		input:	Pointer to null AEDesc.						output:	A property object.		RESULT CODES	____________	noErr				    0	No error		paramErr			  -50	Error in parameter list	memFullErr			 -108	Not enough room in heap zone		errAECoercionFail 	-1700	Data could not be coerced to the requested 								Apple event data type		errAEWrongDataType	-1703	Wrong Apple event data type		errAENotAEDesc		-1704	Not a valid descriptor record		errAEBadListItem	-1705	Operation involving a list item failed	*/pascal	OSErr	MoreAEOCreateSelectionObject( const DescType pSelectionAEDesc,											  AEDesc *pContainerAEDesc,											  AEDesc *pSelectionObject ){	OSErr	anErr = noErr;		AEDesc	selectionDesc = {typeNull,NULL};		anErr = AECreateDesc( typeAbsoluteOrdinal, &pSelectionAEDesc, sizeof( pSelectionAEDesc ), &selectionDesc );	if ( noErr == anErr )	{		anErr = CreateObjSpecifier( cObject, pContainerAEDesc, formAbsolutePosition,									&selectionDesc, false, pSelectionObject );		MoreAEDisposeDesc( &selectionDesc );	}				return anErr;}//end MoreAEOCreateSelectionObject/********************************************************************************	Make position list (a list containing two longs representin the x and y values	for the position of a Finder item).		pPosition			input:	A point specifying the position.	pPositionAEList		input:	Pointer to an AEList (contents will be lost, but not disposed).						output:	A new AEList containing the x & y values for the position.		Result Codes	____________	noErr				    0	No error		memFullErr			 -108	Not enough room in heap zone	*/pascal	OSErr	MoreAEOCreatePositionList( const Point pPosition,									 AEDescList * pPositionAEList ){	OSErr	anErr = noErr;		anErr = AECreateList( NULL, 0, false, pPositionAEList );	if ( noErr == anErr )	{		long h = pPosition.h;		long v = pPosition.v;		anErr = AEPutPtr( pPositionAEList, 0, typeInteger, &h, sizeof(h) );		if ( noErr == anErr ) 			anErr = AEPutPtr( pPositionAEList, 0, typeInteger, &v, sizeof(v) );	}	return anErr;}//end MoreAEOCreatePositionList//********************************************************************************// A simple wrapper around CreateObjSpecifier which creates// an object specifier using formUniqueID and the unique ID// in pKeyData.pascal OSStatus MoreAEOCreateObjSpecifierFormUniqueID(DescType pDesiredClass, const AEDesc *pContainer, 												SInt32 pKeyData, Boolean pDisposeInputs, 												AEDesc *pObjSpecifier){	OSStatus anErr;	AEDesc   keyDesc;	assert(pContainer != NULL);	assert(pObjSpecifier != NULL);	MoreAENullDesc(&keyDesc);		anErr = AECreateDesc(typeLongInteger, &pKeyData, sizeof(SInt32), &keyDesc);	if (noErr == anErr) {		anErr = CreateObjSpecifier(pDesiredClass, (AEDesc *) pContainer, formUniqueID, &keyDesc, pDisposeInputs, pObjSpecifier);	}		MoreAEDisposeDesc(&keyDesc);		return anErr;}//end MoreAEOCreateObjSpecifierFormUniqueID//********************************************************************************// A simple wrapper around CreateObjSpecifier which creates// an object specifier using formAbsolutePosition, a key of// typeLongInteger (rather than typeAbsoluteOrdinal) and the// position index in pKeyData.pascal OSStatus MoreAEOCreateObjSpecifierFormAbsPos(DescType pDesiredClass, const AEDesc *pContainer, 											SInt32 pKeyData, SInt32 pDisposeInputs, 											AEDesc *pObjSpecifier){	OSStatus anErr;	AEDesc   keyDesc;	assert(pContainer != NULL);	assert(pObjSpecifier != NULL);	MoreAENullDesc(&keyDesc);		anErr = AECreateDesc(typeLongInteger, &pKeyData, sizeof(SInt32), &keyDesc);	if (noErr == anErr) {		anErr = CreateObjSpecifier(pDesiredClass, (AEDesc *) pContainer, formAbsolutePosition, &keyDesc, pDisposeInputs, pObjSpecifier);	}		MoreAEDisposeDesc(&keyDesc);		return anErr;}//end MoreAEOCreateObjSpecifierFormAbsPos//********************************************************************************// A simple wrapper around CreateObjSpecifier which creates// an object specifier using formName and the name in pKeyData.pascal OSStatus MoreAEOCreateObjSpecifierFormName(DescType pDesiredClass, const AEDesc *pContainer, 											ConstStr255Param pKeyData, Boolean pDisposeInputs, 											AEDesc *pObjSpecifier){	OSStatus anErr;	AEDesc   keyDesc;	assert(pContainer != NULL);	assert(pKeyData != NULL);	assert(pObjSpecifier != NULL);		MoreAENullDesc(&keyDesc);		anErr = AECreateDesc(typeText, &pKeyData[1], pKeyData[0], &keyDesc);	if (noErr == anErr) {		anErr = CreateObjSpecifier(pDesiredClass, (AEDesc *) pContainer, formName, &keyDesc, pDisposeInputs, pObjSpecifier);	}	MoreAEDisposeDesc(&keyDesc);	return anErr;}//end MoreAEOCreateObjSpecifierFormName//********************************************************************************// A simple wrapper around CreateObjSpecifier which creates// an object specifier from a FSSpec using formName.pascal OSStatus MoreAEOCreateObjSpecifierFromFSSpec(const FSSpecPtr pFSSpecPtr,AEDesc *pObjSpecifier){	OSErr 		anErr = paramErr;	if (NULL != pFSSpecPtr)	{#if TARGET_API_MAC_CARBON		FSRef tFSRef;		anErr = FSpMakeFSRef(pFSSpecPtr,&tFSRef);		if (noErr == anErr)		{			anErr = MoreAEOCreateObjSpecifierFromFSRef(&tFSRef,pObjSpecifier);		}#else		AEDesc containerAEDesc = {typeNull,NULL};		anErr = MoreAEOCreateAliasObjectFromFSSpec(pFSSpecPtr,&containerAEDesc,pObjSpecifier);#endif TARGET_API_MAC_CARBON	}	return anErr;}//end MoreAEOCreateObjSpecifierFromFSSpec//********************************************************************************// A simple wrapper around CreateObjSpecifier which creates// an object specifier from a FSRef and using formName.pascal OSStatus MoreAEOCreateObjSpecifierFromFSRef(const FSRefPtr pFSRefPtr,AEDesc *pObjSpecifier){	OSErr 		anErr = paramErr;	if (NULL != pFSRefPtr)	{#if TARGET_API_MAC_CARBON		CFURLRef tCFURLRef = CFURLCreateFromFSRef(kCFAllocatorDefault,pFSRefPtr);		if (NULL != tCFURLRef)		{			anErr = MoreAEOCreateObjSpecifierFromCFURLRef(tCFURLRef,pObjSpecifier);			CFRelease(tCFURLRef);		}		else			anErr = coreFoundationUnknownErr;#else		AEDesc containerAEDesc = {typeNull,NULL};		anErr = MoreAEOCreateAliasObjectFromFSRef(pFSRefPtr,&containerAEDesc,pObjSpecifier);#endif TARGET_API_MAC_CARBON	}	return anErr;}// end MoreAEOCreateObjSpecifierFromFSRef//********************************************************************************// A simple wrapper around CreateObjSpecifier which creates// an object specifier from a CFURLRef and using formName.#if TARGET_API_MAC_CARBONpascal OSStatus MoreAEOCreateObjSpecifierFromCFURLRef(const CFURLRef pCFURLRef,AEDesc *pObjSpecifier){	OSErr 		anErr = paramErr;	if (NULL != pCFURLRef)	{		CFStringRef tCFStringRef = CFURLCopyFileSystemPath(pCFURLRef,kCFURLHFSPathStyle);		anErr = coreFoundationUnknownErr;		if (NULL != tCFStringRef)		{			Boolean 		isDirectory = CFURLHasDirectoryPath(pCFURLRef);			AEDesc 			containerDesc = {typeNull, NULL};			AEDesc 			nameDesc = {typeNull, NULL};				Size			bufSize = (CFStringGetLength(tCFStringRef) + (isDirectory ? 1 : 0)) * sizeof(UniChar);				UniCharPtr		buf = (UniCharPtr) NewPtr(bufSize);				if ((anErr = MemError()) == noErr)				{					CFStringGetCharacters(tCFStringRef, CFRangeMake(0,bufSize/2), buf);					if (isDirectory) (buf)[(bufSize-1)/2] = (UniChar) 0x003A;								}						if (anErr == noErr)				anErr = AECreateDesc(typeUnicodeText, buf, GetPtrSize((Ptr) buf), &nameDesc);			if (anErr == noErr)				{					if (isDirectory)	// we use cObject here since this might be a package (and we have no way to tell)						anErr = CreateObjSpecifier(cObject, &containerDesc, formName, &nameDesc, false, pObjSpecifier);					else						anErr = CreateObjSpecifier(cFile, &containerDesc, formName, &nameDesc, false, pObjSpecifier);				}			MoreAEDisposeDesc(&nameDesc);			if (buf)				DisposePtr((Ptr)buf);		}	}	return anErr;}//end MoreAEOCreateObjSpecifierFromCFURLRef#endif TARGET_API_MAC_CARBON//********************************************************************************// *******************************************// *****       AddTwoIntListToEvent      *****// *******************************************//	Creates an AEList containing 2 integers, and then adds the list to an Apple event./*OSErr AddTwoIntListToEvent( AppleEvent *pAppleEvent, long param1, long param2 ){	OSErr	anErr;	AEDescList	paramList;	anErr = AECreateList( NULL, 0, false, &paramList );	if ( noErr == anErr )	{		anErr = AEPutPtr( &paramList, 0, typeInteger, &param1, sizeof( param1 ) );		if ( noErr == anErr )		{			anErr = AEPutPtr( &paramList, 0, typeInteger, &param2, sizeof( param2 ) );			if ( noErr == anErr )			{				anErr = AEPutParamDesc( pAppleEvent, keyDirectObject, &paramList );			}		}	}		MoreAEDisposeDesc( &paramList );	return ( anErr );}// end AddTwoIntListToEvent*/