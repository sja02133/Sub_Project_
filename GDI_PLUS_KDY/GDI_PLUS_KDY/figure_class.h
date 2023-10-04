#pragma once

#include "framework.h"
#include <list>
#include <corecrt_math_defines.h>

struct rectangleCPoint {
	CPoint m_ptLeftUp;
	CPoint m_ptLeftDown;
	CPoint m_ptRightUp;
	CPoint m_ptRightDown;
};

struct figure_info {
	rectangleCPoint ptRect;
};

class CFigure {
public:
	BOOL check_erase = FALSE;
	BYTE m_Figure_type = 0;
	BOOL check_first = TRUE;
	
	BYTE m_type = 0;
	UINT th = 0;
	// 1 => arrow line
public:
	virtual void Draw(CDC* pDC) { };
	virtual void Erase(CDC* pDC) { };
//	void initialize_CPoint(CPoint& point);
	void get_figureInfo(figure_info* info,CPoint ptStart,CPoint ptEnd);
};

class CArrow_line : public CFigure {
public:
	UINT th = 0;
	BYTE m_type = 1;
	//LINE
	CPoint m_ptStart;
	CPoint m_ptEnd;

	//ARROW
	CPoint m_ptLeft;
	CPoint m_ptRight;
	double size = 25.0;

	void arrow_lineResize(CPoint& ptStart, CPoint& ptEnd);
public:
	CArrow_line(CPoint& ptStart, CPoint& ptEnd);
	void Draw(CDC* pDC);
	void Erase(CDC* pDC);
};



class CRectangle : public CFigure {
public:
	UINT th = 0;
	BYTE m_type = 2;
	CPoint m_ptStart;
	CPoint m_ptEnd;
	rectangleCPoint rectPoint;
public:
	CRectangle();
	CRectangle(CPoint& ptStart, CPoint& ptEnd);
	void Draw(CDC* pDC);
	void Erase(CDC* pDC);
	void rectangleResize(CPoint& ptStart, CPoint& ptEnd);
protected:
//	~CRectangle();
};

class CSquare : public CFigure {
public:
	UINT th = 0;
	BYTE m_type = 3;
	CPoint m_ptStart;
	CPoint m_ptEnd;

	rectangleCPoint rectPoint;
public:
	CSquare(CPoint& ptStart, CPoint& ptEnd);
	void Draw(CDC* pDC);
	void Erase(CDC* pDC);
	void squareResize(CPoint& ptStart, CPoint& ptEnd);
};

class CCircle : public CFigure {
public:
	UINT th = 0;
	BYTE m_type = 4;
	CPoint m_ptStart;
	CPoint m_ptEnd;

	CPoint m_ptRadius;
public:
	CCircle(CPoint& ptStart, CPoint& ptEnd);
	void Draw(CDC* pDC);
	void Erase(CDC* pDC);

	void circleResize(CPoint& ptStart, CPoint& ptEnd);
};



struct before_point {
	CPoint m_ptStart;
	CPoint m_ptEnd;
};

class Figure_manager{
public:
	Figure_manager();
	//BOOL create_figure();
	BOOL delete_figure(UINT th);
	BOOL delete_figure();
	BOOL append_figure(CFigure* figure);

	template<typename T>
	BYTE get_figureType(T* figure);

	template<typename T>
	BYTE set_figureTh(T* figure, UINT th);
public:
	std::list<CFigure*> m_figure_list;

public:
	BYTE active_type = 0;
	CArrow_line* arrow;
	CRectangle* rect;
	CSquare* square;
	CCircle* circle;
};

