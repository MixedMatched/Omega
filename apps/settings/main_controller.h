#ifndef SETTINGS_MAIN_CONTROLLER_H
#define SETTINGS_MAIN_CONTROLLER_H

#include <escher.h>
#include "settings_message_tree.h"
#include "message_table_cell_with_gauge_with_separator.h"
#include "sub_menu/about_controller.h"
#include "sub_menu/accessibility_controller.h"
#include "sub_menu/exam_mode_controller.h"
#include "sub_menu/language_controller.h"
#include "sub_menu/math_options_controller.h"
#include "sub_menu/preferences_controller.h"

namespace Settings {

extern const SettingsMessageTree s_modelAngleChildren[3];
extern const SettingsMessageTree s_modelEditionModeChildren[2];
extern const SettingsMessageTree s_modelFloatDisplayModeChildren[4];
extern const SettingsMessageTree s_modelComplexFormatChildren[3];
extern const SettingsMessageTree s_symbolChildren[4];
extern const SettingsMessageTree s_modelMathOptionsChildren[5];
extern const SettingsMessageTree s_modelFontChildren[2];
extern const SettingsMessageTree s_accessibilityChildren[6];
extern const SettingsMessageTree s_contributorsChildren[17];
#ifdef USERNAME
extern const SettingsMessageTree s_modelAboutChildren[8];
#else
extern const SettingsMessageTree s_modelAboutChildren[7];
#endif
extern const SettingsMessageTree s_model;

class MainController : public ViewController, public ListViewDataSource, public SelectableTableViewDataSource {
public:
  MainController(Responder * parentResponder, InputEventHandlerDelegate * inputEventHandlerDelegate);
  View * view() override;
  bool handleEvent(Ion::Events::Event event) override;
  void didBecomeFirstResponder() override;
  int numberOfRows() const override;
  KDCoordinate rowHeight(int j) override;
  KDCoordinate cumulatedHeightFromIndex(int j) override;
  int indexFromCumulatedHeight(KDCoordinate offsetY) override;
  HighlightCell * reusableCell(int index, int type) override;
  int reusableCellCount(int type) override;
  int typeAtLocation(int i, int j) override;
  void willDisplayCellForIndex(HighlightCell * cell, int index) override;
  void viewWillAppear() override;
private:
  static const SettingsMessageTree * model();
private:
  StackViewController * stackController() const;
  I18n::Message promptMessage() const;
  bool hasPrompt() const { return promptMessage() != I18n::Message::Default; }
  constexpr static int k_numberOfSimpleChevronCells = 8;
  MessageTableCellWithChevronAndMessage m_cells[k_numberOfSimpleChevronCells];
  MessageTableCellWithGaugeWithSeparator m_brightnessCell;
  MessageTableCellWithSwitch m_popUpCell;
  SelectableTableView m_selectableTableView;
  MathOptionsController m_mathOptionsController;
  LanguageController m_languageController;
  AccessibilityController m_accessibilityController;
  ExamModeController m_examModeController;
  AboutController m_aboutController;
  PreferencesController m_preferencesController;
};

}

#endif
