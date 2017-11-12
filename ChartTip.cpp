/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "ChartTip.h"

#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtGui/QMouseEvent>
#include <QtCharts/QChart>

#include <QStyleOptionGraphicsItem>

ChartTip::ChartTip( QtCharts::QChart* a_pParent ):
	QGraphicsItem( a_pParent )
{
	// Nothing
}

QRectF ChartTip::boundingRect() const
{
	return QRect( 0, 0, 50, 50 );
}

void ChartTip::paint( QPainter* a_pPainter, const QStyleOptionGraphicsItem* a_pOption, QWidget* /*a_pWidget*/ )
{
	QFont font = a_pPainter->font();
	font.setPixelSize( 8 );
	a_pPainter->setFont( font );

	a_pPainter->setBrush( QColor( 255, 255, 255 ) );
	a_pPainter->drawText( a_pOption->rect, m_strText );
}

void ChartTip::mousePressEvent( QGraphicsSceneMouseEvent* a_pEvent )
{
	a_pEvent->setAccepted( true );
}

void ChartTip::mouseMoveEvent( QGraphicsSceneMouseEvent* a_pEvent )
{
	if ( a_pEvent->buttons() & Qt::LeftButton )
	{
		setPos( mapToParent( a_pEvent->pos() - a_pEvent->buttonDownPos( Qt::LeftButton ) ) );
		a_pEvent->setAccepted( true );
	}
	else
	{
		a_pEvent->setAccepted( false );
	}
}

void ChartTip::SetText( const QString& a_rText )
{
	m_strText = a_rText;
}

void ChartTip::SetAnchor( QPointF a_point )
{
	m_anchor = a_point;
}

void ChartTip::UpdateGeometry()
{
	prepareGeometryChange();
	setPos( m_anchor );
}
