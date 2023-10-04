
// GDI_PLUS_KDYDoc.h : interface of the CGDIPLUSKDYDoc class
//


#pragma once


class CGDIPLUSKDYDoc : public CDocument
{
protected: // create from serialization only
	CGDIPLUSKDYDoc() noexcept;
	DECLARE_DYNCREATE(CGDIPLUSKDYDoc)

	//custom
public:

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CGDIPLUSKDYDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
