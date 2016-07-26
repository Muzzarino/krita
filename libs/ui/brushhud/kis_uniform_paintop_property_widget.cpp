/*
 *  Copyright (c) 2016 Dmitry Kazakov <dimula73@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "kis_uniform_paintop_property_widget.h"

#include <QVBoxLayout>
#include <QCheckBox>

#include "kis_slider_spin_box.h"
#include "kis_acyclic_signal_connector.h"
#include "kis_slider_based_paintop_property.h"
#include "kis_debug.h"

/****************************************************************/
/*      KisUniformPaintOpPropertyWidget                         */
/****************************************************************/

struct KisUniformPaintOpPropertyWidget::Private
{
    Private(KisUniformPaintOpPropertySP _property)
        : property(_property) {}

    typedef KisUniformPaintOpProperty::Type Type;
    KisUniformPaintOpPropertySP property;
};

KisUniformPaintOpPropertyWidget::KisUniformPaintOpPropertyWidget(KisUniformPaintOpPropertySP property, QWidget *parent)
    : QWidget(parent),
      m_d(new Private(property))
{
    KisAcyclicSignalConnector *conn = new KisAcyclicSignalConnector(this);
    conn->connectForwardVariant(property.data(), SIGNAL(valueChanged(const QVariant&)),
                                this, SLOT(setValue(const QVariant&)));

    conn->connectBackwardVariant(this, SIGNAL(valueChanged(const QVariant&)),
                                 property.data(), SLOT(setValue(const QVariant&)));
}

KisUniformPaintOpPropertyWidget::~KisUniformPaintOpPropertyWidget()
{
}

KisUniformPaintOpPropertySP KisUniformPaintOpPropertyWidget::property() const
{
    return m_d->property;
}

/****************************************************************/
/*      KisUniformPaintOpPropertyIntSlider                      */
/****************************************************************/

KisUniformPaintOpPropertyIntSlider::KisUniformPaintOpPropertyIntSlider(KisUniformPaintOpPropertySP property, QWidget *parent)
    : KisUniformPaintOpPropertyWidget(property, parent)
{
    const QString prefix = QString("%1: ").arg(property->name());
    QVBoxLayout *layout = new QVBoxLayout(this);

    KisIntSliderBasedPaintOpProperty *sliderProperty =
        dynamic_cast<KisIntSliderBasedPaintOpProperty*>(property.data());
    KIS_ASSERT_RECOVER_RETURN(sliderProperty);

    m_slider = new KisSliderSpinBox(this);
    m_slider->setBlockUpdateSignalOnDrag(true);
    m_slider->setRange(sliderProperty->min(), sliderProperty->max());
    m_slider->setSingleStep(sliderProperty->singleStep());
    m_slider->setPageStep(sliderProperty->pageStep());
    m_slider->setPrefix(prefix);
    m_slider->setSuffix(sliderProperty->suffix());

    m_slider->setValue(sliderProperty->value().toInt());
    connect(m_slider, SIGNAL(valueChanged(int)), SLOT(slotSliderChanged(int)));

    layout->addWidget(m_slider);
    setLayout(layout);
}

void KisUniformPaintOpPropertyIntSlider::setValue(const QVariant &value)
{
    m_slider->setValue(value.toInt());
}

void KisUniformPaintOpPropertyIntSlider::slotSliderChanged(int value)
{
    emit valueChanged(value);
}

/****************************************************************/
/*      KisUniformPaintOpPropertyDoubleSlider                   */
/****************************************************************/

KisUniformPaintOpPropertyDoubleSlider::KisUniformPaintOpPropertyDoubleSlider(KisUniformPaintOpPropertySP property, QWidget *parent)
    : KisUniformPaintOpPropertyWidget(property, parent)
{
    const QString prefix = QString("%1: ").arg(property->name());
    QVBoxLayout *layout = new QVBoxLayout(this);

    KisDoubleSliderBasedPaintOpProperty *sliderProperty =
        dynamic_cast<KisDoubleSliderBasedPaintOpProperty*>(property.data());
    KIS_ASSERT_RECOVER_RETURN(sliderProperty);

    m_slider = new KisDoubleSliderSpinBox(this);
    m_slider->setBlockUpdateSignalOnDrag(true);
    m_slider->setRange(sliderProperty->min(), sliderProperty->max(), sliderProperty->decimals());
    m_slider->setSingleStep(sliderProperty->singleStep());
    m_slider->setPrefix(prefix);
    m_slider->setSuffix(sliderProperty->suffix());

    m_slider->setValue(sliderProperty->value().toReal());
    connect(m_slider, SIGNAL(valueChanged(qreal)), SLOT(slotSliderChanged(qreal)));

    layout->addWidget(m_slider);
    setLayout(layout);
}

void KisUniformPaintOpPropertyDoubleSlider::setValue(const QVariant &value)
{
    m_slider->setValue(value.toReal());
}

void KisUniformPaintOpPropertyDoubleSlider::slotSliderChanged(qreal value)
{
    emit valueChanged(value);
}

/****************************************************************/
/*      KisUniformPaintOpPropertyCheckBox                   */
/****************************************************************/

KisUniformPaintOpPropertyCheckBox::KisUniformPaintOpPropertyCheckBox(KisUniformPaintOpPropertySP property, QWidget *parent)
    : KisUniformPaintOpPropertyWidget(property, parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    m_checkBox = new QCheckBox(property->name(), this);
    m_checkBox->setChecked(property->value().toBool());
    connect(m_checkBox, SIGNAL(toggled(bool)), SLOT(slotCheckBoxChanged(bool)));

    layout->addWidget(m_checkBox);
    setLayout(layout);
}

void KisUniformPaintOpPropertyCheckBox::setValue(const QVariant &value)
{
    m_checkBox->setChecked(value.toBool());
}

void KisUniformPaintOpPropertyCheckBox::slotCheckBoxChanged(bool value)
{
    emit valueChanged(value);
}
