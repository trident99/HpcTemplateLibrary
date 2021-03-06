/*
**	This file is part of the Hpc Template Library (Htl or Htl).
**  It is based on a merger of QT, STL, and SigSlots along with development of new classes.
**  License information is in the HtlLicense.h file
**	This software was merged and developed by:
**	
**  Anthony Daniels
**	QT, STL, SigSlots
**
**  Htl is free software: you can redistribute it and/or modify
**  it under the terms of the GNU Lesser General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  The United States of America Department of Defense has unlimited 
**	usage, redistribution, and modification rights to HtlStar.
**
**  Htl is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Lesser General Public License for more details.
**
**  You should have received a copy of the GNU Lesser General Public License
**  along with Htl.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#pragma warning(push)
#pragma warning (disable : 4005 ) /* macro redefinition */
#define HTL_DLLEXPORT
#pragma warning(pop)

#include ".\HtlObject.h"
#include ".\HtlObjectMap.h"
#include "..\modHtlBase.h"


namespace HTL
{
	//!Generic Public Constructor 
	HtlObjectMap::HtlObjectMap(void)
	{
		std::string strFile = "HtlObjectMap.txt";
		m_objPrint.Set_blnPrintToFile(true);
		m_objPrint.Set_blnPrintToStream(false);
		m_objPrint.Set_blnPrintToTextEdit(false);
		m_objPrint.Set_strFileName(strFile);
		//comment out next line to enable debug dumping to file
		//m_objPrint.Disable();

		m_objKey.newID();
		return;
	};

	//!Default Destructor Method
	HtlObjectMap::~HtlObjectMap(void)
	{
		ClearRegistry();
		return;	
	};

	//!Get the pointer to the HtlObject
	HtlObject * HtlObjectMap::GetHtlObjectPtr(HtlGUID128 & objGUID)
	{
		m_objPrint.DebugPrint(1,"HtlObjectMap::GetHtlObjectPtr\n");
		HtlObject * ptrObj = NULL;
		HtlTreeNode<HtlGUID128,HtlObject*> * ptrIter;
		std::string strSearchGUID = objGUID.toStdString();
		if(m_objHtlObjectMap.size() <= 0){return NULL;};
		//safe to proceed
		m_objPrint.DebugPrint(1,"Searching GUID: %s\n",strSearchGUID.c_str());
		ptrIter = m_objHtlObjectMap.find(objGUID);
		if(ptrIter != NULL)
		{
			ptrObj = (*ptrIter).value();
			m_objPrint.DebugPrint(1,"Iterator Found: %p\n",ptrObj);
			if(ptrObj)
			{
				std::string strFoundGUID = ptrObj->Get_objGUID().toStdString();
				m_objPrint.DebugPrint(1,"Found GUID: %s\n",strFoundGUID.c_str());
			}
		}
		return ptrObj;	
	};

	//!Get a ISA Dialog by its GUID
	bool HtlObjectMap::DoesHtlObjectExist(HtlGUID128 & objGUID)
	{
		HtlObject * ptrObj = NULL;
		HtlTreeNode<HtlGUID128,HtlObject*> * ptrIter;

		if(m_objHtlObjectMap.size() <= 0){return false;};
		//safe to proceed
		ptrIter = m_objHtlObjectMap.find(objGUID);
		if(ptrIter != NULL)
		{
			ptrObj = (*ptrIter).value();
		}else{
			return false;
		};
		if(ptrObj)
		{
			return true;
		}else{
			return false;
		};

	};

	//!Register an ISA dialog to the server
	int HtlObjectMap::RegisterHtlObject(HtlObject * ptrObj)
	{
		std::string strGUID;
		//safety check
		if(!ptrObj){return 0;};
		//lock the map
		m_objMutex.lock(m_objKey);
		m_objPrint.DebugPrint(1,"\nHtlObjectMap::RegisterHtlObject1\n");
		m_objPrint.DebugPrint(1,"placing %s\n",ptrObj->Get_strType().c_str());
		HtlTreeNode<HtlGUID128,HtlObject*> * ptrIter;

		strGUID = ptrObj->Get_objGUID().toStdString();
		m_objPrint.DebugPrint(1,"Search GUID: %s\n",strGUID.c_str());
		ptrIter = m_objHtlObjectMap.find(ptrObj->Get_objGUID());
		if(ptrIter == NULL)
		{	//then not inserted
			strGUID = ptrObj->Get_objGUID().toStdString();
			m_objPrint.DebugPrint(1,"New GUID: %s\n",strGUID.c_str());
			//then not found, insert it
			m_objHtlObjectMap.insert(ptrObj->Get_objGUID(), ptrObj);
		}else{			
			strGUID = ptrIter->key().toStdString();
			m_objPrint.DebugPrint(1,"Found GUID: %s\n",strGUID.c_str());
			//match found, new pointer, update the pointer
			strGUID = ptrObj->Get_objGUID().toStdString();
			m_objPrint.DebugPrint(1,"New GUID: %s\n",strGUID.c_str());
			ptrIter->setValue(ptrObj);

		};
		//unlock the map
		m_objMutex.unlock(m_objKey);

		return 1;	
	};

	int HtlObjectMap::RegisterHtlObject(HtlObject * ptrObj, HtlGUID128 & objGUID)
	{		
		std::string strGUID;
		//safety check
		if(!ptrObj){return 0;};
		//lock the map
		m_objMutex.lock(m_objKey);
		m_objPrint.DebugPrint(1,"\nHtlObjectMap::RegisterHtlObject2\n");
		m_objPrint.DebugPrint(1,"placing %s\n",ptrObj->Get_strType().c_str());
		HtlTreeNode<HtlGUID128,HtlObject*> * ptrIter;
		strGUID = objGUID.toStdString();
		m_objPrint.DebugPrint(1,"Search GUID: %s\n",strGUID.c_str());
		ptrIter = m_objHtlObjectMap.find(objGUID);
		if(ptrIter == NULL)
		{
			//not found yet
			strGUID = objGUID.toStdString();
			m_objPrint.DebugPrint(1,"New GUID: %s\n",strGUID.c_str());
			strGUID = ptrObj->Get_objGUID().toStdString();
			m_objPrint.DebugPrint(1,"Check GUID: %s\n",strGUID.c_str());
			//then not found, insert it
			m_objHtlObjectMap.insert(objGUID, ptrObj);
		}else{
			strGUID = ptrIter->key().toStdString();
			m_objPrint.DebugPrint(1,"Found GUID: %s\n",strGUID.c_str());
			//match found, new pointer, update the pointer
			strGUID = ptrObj->Get_objGUID().toStdString();
			m_objPrint.DebugPrint(1,"New GUID: %s\n",strGUID.c_str());
			ptrIter->setValue(ptrObj);
		};
		//unlock the map
		m_objMutex.unlock(m_objKey);
		return 1;		
	};

	int HtlObjectMap::UpdateHtlObject(HtlObject * ptrObj, HtlGUID128 & objNewGUID)
	{
		//lock the map
		int i = 0;
		HtlGUID128 objKeyToDel;
		std::string strGUID;
		if(!ptrObj){return 0;};
		m_objMutex.lock(m_objKey);
		m_objPrint.DebugPrint(1,"\nHtlObjectMap::UpdateHtlObject1\n");
		m_objPrint.DebugPrint(1,"placing %s\n",ptrObj->Get_strType().c_str());
		if(m_objHtlObjectMap.size() > 0)
		{
			HtlTreeNode<HtlGUID128,HtlObject*> * ptrIter;
			for(ptrIter = m_objHtlObjectMap.begin(); ptrIter != m_objHtlObjectMap.end();)
			{
				m_objPrint.DebugPrint(1,"checking..%02d\n",i);
				if((*ptrIter).value() == ptrObj)
				{//then match found
					objKeyToDel = ptrIter->key();
					strGUID = objKeyToDel.toStdString();
					m_objPrint.DebugPrint(1,"Current GUID: %s\n",strGUID.c_str());
					m_objHtlObjectMap.erase(objKeyToDel);
					strGUID = objNewGUID.toStdString();
					m_objPrint.DebugPrint(1,"New GUID: %s\n",strGUID.c_str());
					strGUID = ptrObj->Get_objGUID().toStdString();
					m_objPrint.DebugPrint(1,"Checked GUID: %s\n",strGUID.c_str());
					m_objHtlObjectMap.insert(objNewGUID, ptrObj);
					//release the lock
					m_objMutex.unlock(m_objKey);
					return 1;
				};
				i++;
				ptrIter = m_objHtlObjectMap.next();
			};
		}else{
			m_objPrint.DebugPrint(1,"m_objHtlObjectMap size = 0 \n");
		}
		//match not found in registry, add it
		m_objHtlObjectMap.insert(objNewGUID, ptrObj);
		//release the lock
		m_objMutex.unlock(m_objKey);

		return 0;
	};

	int HtlObjectMap::UpdateHtlObject(HtlGUID128 & objGUID, HtlObject * ptrNewObj)
	{
		std::string strGUID;
		//lock the map
		if(!ptrNewObj){return 0;};
		m_objMutex.lock(m_objKey);
		m_objPrint.DebugPrint(1,"\nHtlObjectMap::UpdateHtlObject2\n");
		m_objPrint.DebugPrint(1,"placing %s\n",ptrNewObj->Get_strType().c_str());
		if(m_objHtlObjectMap.size() > 0)
		{
			HtlTreeNode<HtlGUID128,HtlObject*> * ptrIter;
			for(ptrIter = m_objHtlObjectMap.begin(); ptrIter != m_objHtlObjectMap.end();)
			{
				if((*ptrIter).key() == objGUID)
				{//then match found
					strGUID = objGUID.toStdString();
					m_objPrint.DebugPrint(1,"Current GUID: %s\n",strGUID.c_str());
					strGUID = ptrNewObj->Get_objGUID().toStdString();
					m_objPrint.DebugPrint(1,"Check GUID: %s\n",strGUID.c_str());
					(*ptrIter).setValue( ptrNewObj);
					//release the lock
					m_objMutex.unlock(m_objKey);
					return 1;
				};
				ptrIter =  m_objHtlObjectMap.next();
			};
		};
		//match not found in registry, add it
		m_objHtlObjectMap.insert(objGUID, ptrNewObj);
		//release the lock
		m_objMutex.unlock(m_objKey);
		return 0;
	};

	//!Unregister an ISA dialog
	int HtlObjectMap::UnRegisterHtlObject(HtlObject * ptrObj)
	{
		HtlGUID128 objKeyToDel;
		//lock the map
		m_objMutex.lock(m_objKey);
		//safety check
		if(!ptrObj)
		{
			//release the lock
			m_objMutex.unlock(m_objKey);
			return 0;
		};
		//search for the object in the registry
		if(m_objHtlObjectMap.size() > 0)
		{
			objKeyToDel = ptrObj->Get_objGUID();
			m_objHtlObjectMap.erase(objKeyToDel);
		};

		//release the lock
		m_objMutex.unlock(m_objKey);
		return 1;	
	};

	int HtlObjectMap::UnRegisterHtlObject(HtlGUID128 & objGUID)
	{
		//lock the map
		m_objMutex.lock(m_objKey);
		if(m_objHtlObjectMap.size() > 0)
		{
			m_objHtlObjectMap.erase(objGUID);
		};
		//release the lock
		m_objMutex.unlock(m_objKey);

		return 1;		
	};

	//!Clear the registry
	void HtlObjectMap::ClearRegistry(void)
	{
		m_objHtlObjectMap.clear();
		return;	
	};

};//end namespace HTL
