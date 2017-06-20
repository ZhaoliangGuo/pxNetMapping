/////////////////////////////////////////////////////////////////////////
// Object.h
// Class of basic object, obarray, oblist and obtree derived from MFC classes
// 2003.11.11
/////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef NX_UTILS_EXP
#define NX_UTILS_EXP __declspec(dllimport)
#endif


#include "XMLDOMDocObj.h"
#include "QSort.h"
#include "afxtempl.h"
#include "NxEditAPI.h"

template<class TYPE>
INT_PTR FindItemInArray(CArray<TYPE, TYPE> & aItemArray, TYPE & Item)
{
	INT_PTR iCount = aItemArray.GetCount();
	for(INT_PTR i = 0; i < iCount; i++) 
	{
		if(aItemArray[i] == Item)
			return i;
	}

	return -1;
};

template <class T>
BOOL ElementInArray(T element, CArray<T, T>& array)
{
	for(int i = 0; i < (int)array.GetSize(); i++)
	{
		T tempElement = array.GetAt(i);
		if(tempElement == element)
			return TRUE;
	}

	return FALSE;
}

template <class T>
int ElementIndexInArray(T element, CArray<T, T>& array)
{
	for (int i = 0; i < (int)array.GetSize(); i++)
	{
		T tempElement = array.GetAt(i);
		if (tempElement == element)
			return i;
	}

	return -1;
}
/************************************************************************/
/* CNxObject                                                            */
/************************************************************************/
class NX_UTILS_EXP CNxObject : public CObject
{
	DECLARE_SERIAL(CNxObject)
public:
	CNxObject();
	virtual ~CNxObject();
	
	// Serialize this object, throw exception if failed.
	// Needs to be overriden, call base version first.	 
	virtual void Serialize(CArchive& ar);
	
	// Serialize this object in XML format, throw exception if failed.
	// Needs to be overriden, call base version first.	 
	virtual void SerializeXML(CNxXMLDOMDocObj* pDocObj, IXMLDOMNode* pParentNode);

	virtual void UpdateSerializeData() {};

public:
	static CString FindClassName(LPCTSTR lpszClassName);
	static CString FindClassPartnerStr(LPCTSTR lpszClassName);
	
public:
	static SNxClassID ms_aClass[];
public:

	//new project
	virtual HRESULT SaveKLV(INxKLVDocumentObject * in_pDocObj, INxKLVNode * in_pParentNode);
	virtual HRESULT LoadKLV(INxKLVDocumentObject * in_pDocObj, LPBYTE		in_pParentNode);

	static CString FindClassName(unsigned long in_ulClassKey);

	static SNxClassID2 ms_aKLVClass[];


};

/************************************************************************/
/* Array, element must be CNxObject                                     */
/************************************************************************/

class NX_UTILS_EXP CNxObArray : public CObArray, public CNxQSort
{
	DECLARE_SERIAL(CNxObArray)

public:
	CNxObArray();
	virtual ~CNxObArray();
	
	// Similar as those of CNxObject		 
	virtual void Serialize(CArchive& ar);
	virtual void SerializeXML(CNxXMLDOMDocObj* pDocObj, IXMLDOMNode* pParentNode);
	
	// Delete elements in array
	// nIndex --- the start index
	// nCount --- the count to be deleted	
	void DeleteAt(int nIndex, int nCount = 1);
    
	// Delete all elements in array
	void DeleteAll();

	// 查找指定的对象, 成功则返回其在数组中以零为底的序号, 
	// 否则返回 - 1;
	int Find(CObject * pFind);

	// 使用直接插入的方法进行排序
	void Sort();

public:
	virtual HRESULT SaveKLV(INxKLVDocumentObject * in_pDocObj, INxKLVNode * in_pParentNode);
	virtual HRESULT LoadKLV(INxKLVDocumentObject * in_pDocObj, LPBYTE   in_pParentNode);

};

/************************************************************************/
/*  List, element must be CNxObject                                     */
/************************************************************************/
class NX_UTILS_EXP CNxObList : public CObList
{
	DECLARE_SERIAL(CNxObList)

public:
	CNxObList();
	virtual ~CNxObList();
	
	// Similar as those of CNxObject		
	virtual void Serialize(CArchive& ar);
	virtual void SerializeXML(CNxXMLDOMDocObj* pDocObj, IXMLDOMNode* pParentNode);
	
	// Delete objects	 
	void DeleteHead();
	void DeleteTail();
	void DeleteAt(POSITION pos);
	void DeleteAll();

	// Get index of a child object
	int GetIndex(CNxObject* pObject);

	POSITION MoveBefore(POSITION posOb, POSITION posRef);
	POSITION MoveAfter(POSITION posOb, POSITION posRef);

public:

	virtual HRESULT SaveKLV(INxKLVDocumentObject * in_pDocObj, INxKLVNode * in_pParentNode);
	virtual HRESULT LoadKLV(INxKLVDocumentObject * in_pDocObj, LPBYTE	  in_pParentNode);

};

/************************************************************************/
/*  Tree, element must be CNxObject                                     */
/************************************************************************/
#define HNXTNITEM POSITION

class NX_UTILS_EXP CNxObTree : public CNxObject, public CNxQSort
// 由于该对象需要实现排序的功能，所以也继承CNxQSort，
// 该类暂不实现Serialize
{
public:
	CNxObTree();
	virtual ~CNxObTree();

protected:
	CPtrList	m_lstPool;

public:
	BOOL RemoveAll();
	BOOL DeleteAll();

	HNXTNITEM AddItem(IN HNXTNITEM hParent, IN void * pItem, IN DWORD dwNodeState = 0);
	BOOL RemoveItem(HNXTNITEM hItem);
	BOOL DeleteItem(HNXTNITEM hItem);
	HNXTNITEM InsertItem(IN HNXTNITEM hParent, IN UINT nIndex, IN void * pItem, IN DWORD dwNodeState = 0);

	BOOL CanMove(HNXTNITEM hItem, HNXTNITEM hTo);
	BOOL MoveItem(HNXTNITEM hItem, HNXTNITEM hTo);

	HNXTNITEM GetRoot();
	HNXTNITEM GetParent(HNXTNITEM hItem);
	int		  GetChildrenCount(HNXTNITEM hItem);
	HNXTNITEM GetChildren(HNXTNITEM hParentItem, int iIndex);
	HNXTNITEM GetTailChildren(HNXTNITEM hParentItem);
	HNXTNITEM GetHeadChildren(HNXTNITEM hParentItem);
	HNXTNITEM GetNextBrother(HNXTNITEM hItem);
	HNXTNITEM GetPrevBrother(HNXTNITEM hItem);

	HNXTNITEM FindItem(void * pItem);
	void * GetItem(HNXTNITEM hItem);

	// 遍历Tree中指定节点下所有的Item，并加到oList中，
	// PS:不包括指定节点hItem
	void TreeToList(IN OUT CList<HNXTNITEM, HNXTNITEM> & oList, HNXTNITEM hItem);

	BOOL Sort(HNXTNITEM hItem);

	BOOL SetNodeState(HNXTNITEM hItem, DWORD dwMask);
	BOOL ResetNodeState(HNXTNITEM hItem, DWORD dwMask);
	DWORD GetNodeState(HNXTNITEM hItem, DWORD dwMask);

protected: 
	//重载 CNxQSort::_Compare
	virtual int _Compare(LPVOID lpA, LPVOID lpB);
};

