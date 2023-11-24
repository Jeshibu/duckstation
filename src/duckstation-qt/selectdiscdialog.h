// SPDX-FileCopyrightText: 2019-2023 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: (GPL-3.0 OR CC-BY-NC-ND-4.0)

#pragma once
#include "common/timer.h"
#include "common/types.h"
#include "qtprogresscallback.h"
#include "ui_selectdiscdialog.h"
#include <QtWidgets/QDialog>
#include <array>
#include <memory>
#include <string>

class SelectDiscDialog final : public QDialog
{
  Q_OBJECT

public:
  SelectDiscDialog(const std::string& disc_set_name, QWidget* parent = nullptr);
  ~SelectDiscDialog();

  std::string takeSelectedDiscPath();

private Q_SLOTS:
  void updateStartEnabled();
  void onStartClicked();
  void onCancelClicked();

private:
  void populateList(const std::string& disc_set_name);

  Ui::SelectDiscDialog m_ui;
  std::string m_selected_path;
};
