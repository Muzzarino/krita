/* This file is part of the KDE project
 * Copyright (C) 2007-2008 Thomas Zander <zander@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
#ifndef STYLESWIDGET_H
#define STYLESWIDGET_H

#include <QFrame>
#include <QList>
#include <QTextBlockFormat>
#include <QTextCharFormat>

#include <ui_StylesWidget.h>

class KoStyleManager;
class KoParagraphStyle;
class KoCharacterStyle;
class StylesModel;
class StylesDelegate;

class StylesWidget : public QFrame
{
    Q_OBJECT
public:
    explicit StylesWidget(QWidget *parent = 0,  bool paragraphMode=true, Qt::WindowFlags f = 0);
    virtual ~StylesWidget();

public slots:
    void setStyleManager(KoStyleManager *sm);
    void setCurrentFormat(const QTextBlockFormat &format);
    void setCurrentFormat(const QTextCharFormat &format);

signals:
    void doneWithFocus();
    void paragraphStyleSelected(KoParagraphStyle *paragraphStyle, bool canDelete);
    void characterStyleSelected(KoCharacterStyle *characterStyle, bool canDelete);

private slots:
    void applyStyle();

signals:
    void paragraphStyleSelected(KoParagraphStyle *style);
    void characterStyleSelected(KoCharacterStyle *style);

private:
    Ui::StylesWidget widget;
    KoStyleManager *m_styleManager;

    QTextBlockFormat m_currentBlockFormat;
    QTextCharFormat m_currentCharFormat;
    StylesModel *m_stylesModel;
    StylesDelegate *m_stylesDelegate;
    bool m_blockSignals;
    bool m_isEmbedded;
    bool m_isHovered;
};

#endif
