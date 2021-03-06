/*
 *  Copyright (C) 2012 Felix Geyer <debfx@fobos.de>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 or (at your option)
 *  version 3 of the License.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "EditWidget.h"
#include "ui_EditWidget.h"

#include "core/FilePath.h"

EditWidget::EditWidget(QWidget* parent)
    : DialogyWidget(parent)
    , m_ui(new Ui::EditWidget())
{
    m_ui->setupUi(this);
    setReadOnly(false);

    m_ui->messageWidget->setHidden(true);

    QFont headerLabelFont = m_ui->headerLabel->font();
    headerLabelFont.setBold(true);
    headerLabelFont.setPointSize(headerLabelFont.pointSize() + 2);
    headlineLabel()->setFont(headerLabelFont);

    connect(m_ui->categoryList, SIGNAL(categoryChanged(int)),
            m_ui->stackedWidget, SLOT(setCurrentIndex(int)));

    connect(m_ui->buttonBox, SIGNAL(accepted()), SIGNAL(accepted()));
    connect(m_ui->buttonBox, SIGNAL(rejected()), SIGNAL(rejected()));
}

EditWidget::~EditWidget()
{
}

void EditWidget::addPage(const QString& labelText, const QIcon& icon, QWidget* widget)
{
    m_ui->stackedWidget->addWidget(widget);
    m_ui->categoryList->addCategory(labelText, icon);
}

void EditWidget::setPageHidden(QWidget* widget, bool hidden)
{
    int index = m_ui->stackedWidget->indexOf(widget);
    if (index != -1) {
        m_ui->categoryList->setCategoryHidden(index, hidden);
    }

    if (index == m_ui->stackedWidget->currentIndex()) {
        int newIndex = m_ui->stackedWidget->currentIndex() - 1;
        if (newIndex < 0) {
            newIndex = m_ui->stackedWidget->count() - 1;
        }
        m_ui->stackedWidget->setCurrentIndex(newIndex);
    }
}

void EditWidget::setCurrentPage(int index)
{
    m_ui->categoryList->setCurrentCategory(index);
    m_ui->stackedWidget->setCurrentIndex(index);
}

void EditWidget::setHeadline(const QString& text)
{
    m_ui->headerLabel->setText(text);
}

QLabel* EditWidget::headlineLabel()
{
    return m_ui->headerLabel;
}

void EditWidget::setReadOnly(bool readOnly)
{
    m_readOnly = readOnly;

    if (readOnly) {
        m_ui->buttonBox->setStandardButtons(QDialogButtonBox::Close);
    }
    else {
        m_ui->buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    }
}

bool EditWidget::readOnly() const
{
    return m_readOnly;
}

void EditWidget::showMessage(const QString& text, MessageWidget::MessageType type)
{
    m_ui->messageWidget->showMessage(text, type);
}

void EditWidget::hideMessage()
{
    if (m_ui->messageWidget->isVisible()) {
        m_ui->messageWidget->animatedHide();
    }
}
