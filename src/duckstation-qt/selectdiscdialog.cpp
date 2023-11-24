// SPDX-FileCopyrightText: 2019-2023 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: (GPL-3.0 OR CC-BY-NC-ND-4.0)

#include "selectdiscdialog.h"
#include "qtutils.h"

#include "core/game_list.h"

#include "common/assert.h"
#include "common/path.h"

#include <QtWidgets/QTreeWidget>

SelectDiscDialog::SelectDiscDialog(const std::string& disc_set_name, QWidget* parent /* = nullptr */) : QDialog(parent)
{
  m_ui.setupUi(this);
  populateList(disc_set_name);
  updateStartEnabled();

  connect(m_ui.start, &QPushButton::clicked, this, &SelectDiscDialog::onStartClicked);
  connect(m_ui.cancel, &QPushButton::clicked, this, &SelectDiscDialog::onCancelClicked);
  connect(m_ui.discList, &QTreeWidget::itemSelectionChanged, this, &SelectDiscDialog::updateStartEnabled);
}

SelectDiscDialog::~SelectDiscDialog() = default;

std::string SelectDiscDialog::takeSelectedDiscPath()
{
  return std::move(m_selected_path);
}

void SelectDiscDialog::updateStartEnabled()
{
  const QList<QTreeWidgetItem*> items = m_ui.discList->selectedItems();
  m_ui.start->setEnabled(!items.isEmpty());
  if (!items.isEmpty())
    m_selected_path = items.first()->data(0, Qt::UserRole).toString().toStdString();
  else
    m_selected_path = {};
}

void SelectDiscDialog::onStartClicked()
{
  done(1);
}

void SelectDiscDialog::onCancelClicked()
{
  done(0);
}

void SelectDiscDialog::populateList(const std::string& disc_set_name)
{
  const auto lock = GameList::GetLock();
  const std::vector<const GameList::Entry*> entries = GameList::GetDiscSetMembers(disc_set_name);

  for (const GameList::Entry* entry : entries)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem();
    item->setText(0, QString::number(entry->disc_set_index + 1));
    item->setText(1, QString::fromStdString(entry->title));
    item->setText(2, QtUtils::StringViewToQString(Path::GetFileName(entry->path)));
    item->setData(0, Qt::UserRole, QString::fromStdString(entry->path));
    m_ui.discList->addTopLevelItem(item);

    if (m_ui.discList->topLevelItemCount() == 1)
    {
      m_ui.discList->setCurrentItem(item);
      m_ui.start->setEnabled(true);
    }
  }

  QtUtils::ResizeColumnsForTreeView(m_ui.discList, {25, 250, -1});

  setWindowTitle(tr("Select disc for %1").arg(QString::fromStdString(disc_set_name)));
}
