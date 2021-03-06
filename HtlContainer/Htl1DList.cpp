/*
The High Performance Template Library is a Licensed as LGPL.
It is intended to provide a base object Testitecture, with
Serialization, thread control, etc.  
It is based on a merger of QT, BOOST, STL, and SigSlots.
License information is in the HtlLicense.h file
October 2012
*/
#pragma once
#pragma warning(push)
#pragma warning (disable : 4005 ) /* macro redefinition */
#define HTL_DLLEXPORT
#pragma warning(pop)

#include ".\Htl1DList.h"

namespace HTL
{
//compiler test vectors
	Htl1DList<double, CPP_DOUBLE> arrDblTest;
	
};