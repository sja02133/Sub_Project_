#include "pch.h"
#include "figure_class.h"

//CFigure
void CFigure::get_figureInfo(figure_info* info, CPoint ptStart, CPoint ptEnd)
{
	if (ptStart.y < ptEnd.y) {
		//위 -> 아래
		if (ptStart.x < ptEnd.x) {
			//왼쪽 -> 오른쪽
			info->ptRect.m_ptLeftUp = ptStart;
			info->ptRect.m_ptRightDown = ptEnd;
			
			info->ptRect.m_ptLeftDown.x = ptStart.x;
			info->ptRect.m_ptLeftDown.y = ptEnd.y;
			info->ptRect.m_ptRightUp.x = ptEnd.x;
			info->ptRect.m_ptRightUp.y = ptStart.y;
		}
		else {
			//오른쪽 -> 왼쪽
			info->ptRect.m_ptRightUp = ptStart;
			info->ptRect.m_ptLeftDown = ptEnd;

			info->ptRect.m_ptLeftUp.x = ptEnd.x;
			info->ptRect.m_ptLeftUp.y = ptStart.y;
			info->ptRect.m_ptRightDown.x = ptStart.x;
			info->ptRect.m_ptRightDown.y = ptEnd.y;
		}
	}
	else {
		//아래 -> 위
		if (ptStart.x < ptEnd.x) {
			//왼쪽 -> 오른쪽
			info->ptRect.m_ptLeftDown = ptStart;
			info->ptRect.m_ptRightUp = ptEnd;

			info->ptRect.m_ptLeftUp.x = ptStart.x;
			info->ptRect.m_ptLeftUp.y = ptEnd.y;
			info->ptRect.m_ptRightDown.x = ptEnd.x;
			info->ptRect.m_ptRightDown.y = ptStart.y;
		}
		else {
			//오른쪽 -> 왼쪽
			info->ptRect.m_ptRightDown = ptStart;
			info->ptRect.m_ptLeftUp = ptEnd;

			info->ptRect.m_ptLeftDown.x = ptEnd.x;
			info->ptRect.m_ptLeftDown.y = ptStart.y;
			info->ptRect.m_ptRightUp.x = ptStart.x;
			info->ptRect.m_ptRightUp.y = ptEnd.y;
		}
	}

}

//FIGURE MANAGER
template<typename T>
BYTE Figure_manager::get_figureType(T* figure)
{
	return figure->m_type;
}

template<typename T>
BYTE Figure_manager::set_figureTh(T* figure,UINT th)
{
	figure->th = th;
	return TRUE;
}

Figure_manager::Figure_manager()
{

}

BOOL Figure_manager::append_figure(CFigure* figure)
{
	if (this->m_figure_list.size() == 0) {
		this->set_figureTh(figure, 1);
	}
	else {
		this->set_figureTh(figure, this->m_figure_list.size() + 1);
	}
	this->m_figure_list.push_back(figure);
	figure->m_type = this->get_figureType(figure);
	return TRUE;
}

BOOL Figure_manager::delete_figure()
{
	auto iter = this->m_figure_list.end();
	this->m_figure_list.pop_back();
	delete (*iter);
	return TRUE;
}

BOOL Figure_manager::delete_figure(UINT th)
{
	for (auto iter = this->m_figure_list.begin(); iter != m_figure_list.end(); ++iter) {
		CFigure* figure = *iter;
		if ((this->get_figureType(figure)) == th) {
			figure->check_erase = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}


//CArrow_line
CArrow_line::CArrow_line(CPoint& ptStart, CPoint& ptEnd)
{
	this->m_type = 1;

	this->m_ptStart = ptStart;
	this->m_ptEnd = ptEnd;

	double angle = atan2(ptEnd.y - ptStart.y, ptEnd.x - ptStart.x);
	double arrowSize = 25.0;  // 화살표 크기 조절 가능

	if (angle == 0.0) {
		angle = M_PI;
	}

	// 화살표 끝점 좌표 계산
	CPoint ptArrowEnd = ptEnd;

	double angle1 = angle + M_PI / 4;
	double angle2 = angle - M_PI / 4;
	int arrowSizeHalf = (arrowSize / 2);

	this->m_ptLeft = CPoint((ptArrowEnd.x - (arrowSizeHalf * cos(angle1))), (ptArrowEnd.y - (arrowSizeHalf * sin(angle1))));
	this->m_ptRight = CPoint((ptArrowEnd.x - (arrowSizeHalf * cos(angle2))), (ptArrowEnd.y - (arrowSizeHalf * sin(angle2))));
}

void CArrow_line::Draw(CDC* pDC) {
	pDC->SetROP2(R2_COPYPEN);
	pDC->MoveTo(this->m_ptStart);
	pDC->LineTo(this->m_ptEnd);
	pDC->MoveTo(this->m_ptEnd);
	pDC->LineTo(this->m_ptLeft);
	pDC->MoveTo(this->m_ptEnd);
	pDC->LineTo(this->m_ptRight);
}

void CArrow_line::Erase(CDC* pDC) {
	pDC->SetROP2(R2_NOT);
	pDC->MoveTo(this->m_ptStart);
	pDC->LineTo(this->m_ptEnd);
	pDC->MoveTo(this->m_ptEnd);
	pDC->LineTo(this->m_ptLeft);
	pDC->MoveTo(this->m_ptEnd);
	pDC->LineTo(this->m_ptRight);
}

void CArrow_line::arrow_lineResize(CPoint& ptStart, CPoint& ptEnd)
{
	this->m_ptStart = ptStart;
	this->m_ptEnd = ptEnd;

	double angle = atan2(ptEnd.y - ptStart.y, ptEnd.x - ptStart.x);
	double arrowSize = 25.0;  // 화살표 크기 조절 가능

	if (angle == 0.0) {
		angle = M_PI;
	}

	// 화살표 끝점 좌표 계산
	CPoint ptArrowEnd = ptEnd;

	double angle1 = angle + M_PI / 4;
	double angle2 = angle - M_PI / 4;
	int arrowSizeHalf = (arrowSize / 2);

	this->m_ptLeft = CPoint((ptArrowEnd.x - (arrowSizeHalf * cos(angle1))), (ptArrowEnd.y - (arrowSizeHalf * sin(angle1))));
	this->m_ptRight = CPoint((ptArrowEnd.x - (arrowSizeHalf * cos(angle2))), (ptArrowEnd.y - (arrowSizeHalf * sin(angle2))));
}

//CArrow_line//

//CRectangle 직사각형
CRectangle::CRectangle()
{
	this->m_ptStart.x = 0;
	this->m_ptStart.y = 0;
	this->m_ptEnd.x = 0;
	this->m_ptEnd.y = 0;
	this->m_type = 2;
}

CRectangle::CRectangle(CPoint& ptStart, CPoint& ptEnd) {
	this->m_type = 2;
	
	this->m_ptStart = ptStart;
	this->m_ptEnd = ptEnd;

	figure_info info;
	this->get_figureInfo(&info, this->m_ptStart, this->m_ptEnd);
	this->rectPoint = info.ptRect;
}

//CRectangle::~CRectangle() {
//	//this->initialize_CPoint(this->m_ptStart);
//	//this->initialize_CPoint(this->m_ptEnd);
//}

void CRectangle::Draw(CDC* pDC) {
	pDC->SetROP2(R2_COPYPEN);
	pDC->MoveTo(this->rectPoint.m_ptLeftUp);
	pDC->LineTo(this->rectPoint.m_ptLeftDown);
	pDC->LineTo(this->rectPoint.m_ptRightDown);
	pDC->LineTo(this->rectPoint.m_ptRightUp);
	pDC->LineTo(this->rectPoint.m_ptLeftUp);
}

void CRectangle::Erase(CDC* pDC)
{
	pDC->SetROP2(R2_NOT);
	//this->Draw(pDC);
	pDC->MoveTo(this->rectPoint.m_ptLeftUp);
	pDC->LineTo(this->rectPoint.m_ptLeftDown);
	pDC->LineTo(this->rectPoint.m_ptRightDown);
	pDC->LineTo(this->rectPoint.m_ptRightUp);
	pDC->LineTo(this->rectPoint.m_ptLeftUp);
}

void CRectangle::rectangleResize(CPoint& ptStart, CPoint& ptEnd)
{
	this->m_ptStart = ptStart;
	this->m_ptEnd = ptEnd;

	figure_info info;
	this->get_figureInfo(&info, this->m_ptStart, this->m_ptEnd);
	this->rectPoint = info.ptRect;

	//if (ptStart.y < ptEnd.y) {
	//	//위 -> 아래
	//	if (ptStart.x < ptEnd.x) {
	//		//왼쪽 -> 오른쪽
	//		this->rectPoint.m_ptLeftUp = ptStart;
	//		this->rectPoint.m_ptRightDown = ptEnd;

	//		this->rectPoint.m_ptLeftDown.x = ptStart.x;
	//		this->rectPoint.m_ptLeftDown.y = ptEnd.y;
	//		this->rectPoint.m_ptRightUp.x = ptEnd.x;
	//		this->rectPoint.m_ptRightUp.y = ptStart.y;
	//	}
	//	else {
	//		//오른쪽 -> 왼쪽
	//		this->rectPoint.m_ptRightUp = ptStart;
	//		this->rectPoint.m_ptLeftDown = ptEnd;

	//		this->rectPoint.m_ptLeftUp.x = ptEnd.x;
	//		this->rectPoint.m_ptLeftUp.y = ptStart.y;
	//		this->rectPoint.m_ptRightDown.x = ptStart.x;
	//		this->rectPoint.m_ptRightDown.y = ptEnd.y;
	//	}
	//}
	//else {
	//	//아래 -> 위
	//	if (ptStart.x < ptEnd.x) {
	//		//왼쪽 -> 오른쪽
	//		this->rectPoint.m_ptLeftDown = ptStart;
	//		this->rectPoint.m_ptRightUp = ptEnd;

	//		this->rectPoint.m_ptLeftUp.x = ptStart.x;
	//		this->rectPoint.m_ptLeftUp.y = ptEnd.y;
	//		this->rectPoint.m_ptRightDown.x = ptEnd.x;
	//		this->rectPoint.m_ptRightDown.y = ptStart.y;
	//	}
	//	else {
	//		//오른쪽 -> 왼쪽
	//		this->rectPoint.m_ptRightDown = ptStart;
	//		this->rectPoint.m_ptLeftUp = ptEnd;

	//		this->rectPoint.m_ptLeftDown.x = ptEnd.x;
	//		this->rectPoint.m_ptLeftDown.y = ptStart.y;
	//		this->rectPoint.m_ptRightUp.x = ptStart.x;
	//		this->rectPoint.m_ptRightUp.y = ptEnd.y;
	//	}
	//}
}
//CRectangle 직사각형//

//CSquare 정사각형
CSquare::CSquare(CPoint& ptStart, CPoint& ptEnd) {
	this->m_type = 3;

	this->m_ptStart = ptStart;
	this->m_ptEnd = ptEnd;

	int dx = abs(this->m_ptEnd.x - this->m_ptStart.x);
	int dy = abs(this->m_ptEnd.y - this->m_ptStart.y);
	int diff = abs(dx - dy);

	// x축과 y축 거리 중 큰 값을 기준으로 유지
	if (diff > 0)
	{
		if (dx > dy)
			this->m_ptEnd.y = ptStart.y + (this->m_ptEnd.y > ptStart.y ? dx : -dx);
		else
			this->m_ptEnd.x = ptStart.x + (this->m_ptEnd.x > ptStart.x ? dy : -dy);
	}
	figure_info info;
	this->get_figureInfo(&info, this->m_ptStart, this->m_ptEnd);
	this->rectPoint = info.ptRect;

	//if (ptStart.y < ptEnd.y) {
	//	//위 -> 아래
	//	if (ptStart.x < ptEnd.x) {
	//		//왼쪽 -> 오른쪽
	//		this->rectPoint.m_ptLeftUp = ptStart;
	//		this->rectPoint.m_ptRightDown = ptEnd;

	//		this->rectPoint.m_ptLeftDown.x = ptStart.x;
	//		this->rectPoint.m_ptLeftDown.y = ptEnd.y;
	//		this->rectPoint.m_ptRightUp.x = ptEnd.x;
	//		this->rectPoint.m_ptRightUp.y = ptStart.y;
	//	}
	//	else {
	//		//오른쪽 -> 왼쪽
	//		this->rectPoint.m_ptRightUp = ptStart;
	//		this->rectPoint.m_ptLeftDown = ptEnd;

	//		this->rectPoint.m_ptLeftUp.x = ptEnd.x;
	//		this->rectPoint.m_ptLeftUp.y = ptStart.y;
	//		this->rectPoint.m_ptRightDown.x = ptStart.x;
	//		this->rectPoint.m_ptRightDown.y = ptEnd.y;
	//	}
	//}
	//else {
	//	//아래 -> 위
	//	if (ptStart.x < ptEnd.x) {
	//		//왼쪽 -> 오른쪽
	//		this->rectPoint.m_ptLeftDown = ptStart;
	//		this->rectPoint.m_ptRightUp = ptEnd;

	//		this->rectPoint.m_ptLeftUp.x = ptStart.x;
	//		this->rectPoint.m_ptLeftUp.y = ptEnd.y;
	//		this->rectPoint.m_ptRightDown.x = ptEnd.x;
	//		this->rectPoint.m_ptRightDown.y = ptStart.y;
	//	}
	//	else {
	//		//오른쪽 -> 왼쪽
	//		this->rectPoint.m_ptRightDown = ptStart;
	//		this->rectPoint.m_ptLeftUp = ptEnd;

	//		this->rectPoint.m_ptLeftDown.x = ptEnd.x;
	//		this->rectPoint.m_ptLeftDown.y = ptStart.y;
	//		this->rectPoint.m_ptRightUp.x = ptStart.x;
	//		this->rectPoint.m_ptRightUp.y = ptEnd.y;
	//	}
	//}
}

void CSquare::Draw(CDC* pDC) {
	//
	//int size = min(abs(this->m_ptEnd.x - this->m_ptStart.x), abs(this->m_ptEnd.y - this->m_ptStart.y));
	//int signX = (this->m_ptEnd.x >= this->m_ptStart.x) ? 1 : -1;
	//int signY = (this->m_ptEnd.y >= this->m_ptStart.y) ? 1 : -1;
	//CPoint ptTopRight(this->m_ptStart.x + (size * signX), this->m_ptStart.y);
	//CPoint ptBottomLeft(this->m_ptStart.x, this->m_ptStart.y + (size * signY));
	//CPoint ptBottomRight(this->m_ptStart.x + (size * signX), this->m_ptStart.y + (size * signY));

	//pDC->MoveTo(this->m_ptStart);
	//pDC->LineTo(ptTopRight);
	//pDC->LineTo(ptBottomRight);
	//pDC->LineTo(ptBottomLeft);
	//pDC->LineTo(this->m_ptStart);
	pDC->SetROP2(R2_COPYPEN);
	pDC->MoveTo(this->rectPoint.m_ptLeftUp);
	pDC->LineTo(this->rectPoint.m_ptLeftDown);
	pDC->LineTo(this->rectPoint.m_ptRightDown);
	pDC->LineTo(this->rectPoint.m_ptRightUp);
	pDC->LineTo(this->rectPoint.m_ptLeftUp);
}

void CSquare::Erase(CDC* pDC)
{
	pDC->SetROP2(R2_NOT);
	pDC->MoveTo(this->rectPoint.m_ptLeftUp);
	pDC->LineTo(this->rectPoint.m_ptLeftDown);
	pDC->LineTo(this->rectPoint.m_ptRightDown);
	pDC->LineTo(this->rectPoint.m_ptRightUp);
	pDC->LineTo(this->rectPoint.m_ptLeftUp);
}

void CSquare::squareResize(CPoint& ptStart, CPoint& ptEnd)
{
	this->m_ptStart = ptStart;
	this->m_ptEnd = ptEnd;

	// 기준이 되는 최소값의 변 길이 구하기
	int minLength = min(this->m_ptEnd.x - this->m_ptStart.x, this->m_ptEnd.y - this->m_ptStart.y);
	if (minLength < 0) {
		minLength *= -1;
	}

	if (ptStart.y < ptEnd.y) {
		//위 -> 아래
		this->m_ptEnd.x += minLength;
		this->m_ptEnd.y += minLength;
	}
	else {
		//아래 -> 위
		this->m_ptEnd.x -= minLength;
		this->m_ptEnd.y -= minLength;
	}
	figure_info info;
	this->get_figureInfo(&info, this->m_ptStart, this->m_ptEnd);
	this->rectPoint = info.ptRect;
	//if (ptStart.y < ptEnd.y) {
	//	//위 -> 아래
	//	if (ptStart.x < ptEnd.x) {
	//		//왼쪽 -> 오른쪽
	//		this->rectPoint.m_ptLeftUp = this->m_ptStart;
	//		this->rectPoint.m_ptRightDown = this->m_ptEnd;

	//		this->rectPoint.m_ptLeftDown.x = this->m_ptStart.x;
	//		this->rectPoint.m_ptLeftDown.y = this->m_ptEnd.y;
	//		this->rectPoint.m_ptRightUp.x = this->m_ptEnd.x;
	//		this->rectPoint.m_ptRightUp.y = this->m_ptStart.y;
	//	}
	//	else {
	//		//오른쪽 -> 왼쪽
	//		this->rectPoint.m_ptRightUp = this->m_ptStart;
	//		this->rectPoint.m_ptLeftDown = this->m_ptEnd;

	//		this->rectPoint.m_ptLeftUp.x = this->m_ptEnd.x;
	//		this->rectPoint.m_ptLeftUp.y = this->m_ptStart.y;
	//		this->rectPoint.m_ptRightDown.x = this->m_ptStart.x;
	//		this->rectPoint.m_ptRightDown.y = this->m_ptEnd.y;
	//	}
	//}
	//else {
	//	//아래 -> 위
	//	if (ptStart.x < ptEnd.x) {
	//		//왼쪽 -> 오른쪽
	//		this->rectPoint.m_ptLeftDown = this->m_ptStart;
	//		this->rectPoint.m_ptRightUp = this->m_ptEnd;

	//		this->rectPoint.m_ptLeftUp.x = this->m_ptStart.x;
	//		this->rectPoint.m_ptLeftUp.y = this->m_ptEnd.y;
	//		this->rectPoint.m_ptRightDown.x = this->m_ptEnd.x;
	//		this->rectPoint.m_ptRightDown.y = this->m_ptStart.y;
	//	}
	//	else {
	//		//오른쪽 -> 왼쪽
	//		this->rectPoint.m_ptRightDown = this->m_ptStart;
	//		this->rectPoint.m_ptLeftUp = this->m_ptEnd;

	//		this->rectPoint.m_ptLeftDown.x = this->m_ptEnd.x;
	//		this->rectPoint.m_ptLeftDown.y = this->m_ptStart.y;
	//		this->rectPoint.m_ptRightUp.x = this->m_ptStart.x;
	//		this->rectPoint.m_ptRightUp.y = this->m_ptEnd.y;
	//	}
	//}
}
//CSquare 정사각형//

//CCircle 원
CCircle::CCircle(CPoint& ptStart, CPoint& ptEnd) {

	this->m_type = 4;

	this->m_ptStart = ptStart;
	this->m_ptEnd = ptEnd;

	this->m_ptRadius.x = (this->m_ptStart.x + this->m_ptEnd.x) / 2;
	this->m_ptRadius.y = (this->m_ptStart.y + this->m_ptEnd.y) / 2;
}

void CCircle::Draw(CDC* pDC)
{
	Gdiplus::Graphics graphics(pDC->GetSafeHdc());
	CPen* pPen = pDC->GetCurrentPen();  // 현재 펜 객체 가져오기
	LOGPEN logPen;
	pPen->GetLogPen(&logPen);
	Gdiplus::Pen pen(Gdiplus::Color(GetRValue(pDC->GetDCPenColor())
		, GetGValue(pDC->GetDCPenColor()),
		GetBValue(pDC->GetDCPenColor())));

	//Pen pen(Color(255,0,0,255));  // 원의 색상을 파란색으로 설정
	CRectangle rectangle(this->m_ptStart, this->m_ptEnd);
	rectangle.Draw(pDC);

	figure_info fInfo;
	get_figureInfo(&fInfo, this->m_ptStart, this->m_ptEnd);

	RectF ellipseRect(fInfo.ptRect.m_ptLeftUp.x, fInfo.ptRect.m_ptLeftUp.y,
		abs(fInfo.ptRect.m_ptRightUp.x - fInfo.ptRect.m_ptLeftUp.x),
		abs(fInfo.ptRect.m_ptRightUp.y- fInfo.ptRect.m_ptLeftDown.y));
	graphics.DrawEllipse(&pen, ellipseRect);
}

void CCircle::Erase(CDC* pDC)
{
	Graphics graphics(pDC->GetSafeHdc());
	Pen pen(Color(GetRValue(pDC->GetBkColor())
		, GetGValue(pDC->GetBkColor()),
		GetBValue(pDC->GetBkColor())));

	CRectangle rectangle(this->m_ptStart, this->m_ptEnd);
	rectangle.Erase(pDC);

	figure_info fInfo;
	get_figureInfo(&fInfo, this->m_ptStart, this->m_ptEnd);

	RectF ellipseRect(fInfo.ptRect.m_ptLeftUp.x, fInfo.ptRect.m_ptLeftUp.y,
		abs(fInfo.ptRect.m_ptRightUp.x - fInfo.ptRect.m_ptLeftUp.x),
		abs(fInfo.ptRect.m_ptRightUp.y - fInfo.ptRect.m_ptLeftDown.y));
	graphics.DrawEllipse(&pen, ellipseRect);
}

void CCircle::circleResize(CPoint& ptStart, CPoint& ptEnd)
{
	this->m_ptStart = ptStart;
	this->m_ptEnd = ptEnd;
	this->m_ptRadius.x = (this->m_ptStart.x + this->m_ptEnd.x) / 2;
	this->m_ptRadius.y = (this->m_ptStart.y + this->m_ptEnd.y) / 2;
}
