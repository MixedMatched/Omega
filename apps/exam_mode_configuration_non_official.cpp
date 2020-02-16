#include "exam_mode_configuration.h"

using namespace Poincare;

constexpr Settings::SettingsMessageTree s_ledColorChildren[] = {Settings::SettingsMessageTree(I18n::Message::ColorRed), Settings::SettingsMessageTree(I18n::Message::ColorWhite), Settings::SettingsMessageTree(I18n::Message::ColorGreen), Settings::SettingsMessageTree(I18n::Message::ColorBlue), Settings::SettingsMessageTree(I18n::Message::ColorYellow), Settings::SettingsMessageTree(I18n::Message::ColorPurple), Settings::SettingsMessageTree(I18n::Message::ColorOrange)};
constexpr Settings::SettingsMessageTree ExamModeConfiguration::s_modelExamChildren[3] = {Settings::SettingsMessageTree(I18n::Message::LEDColor, s_ledColorChildren), Settings::SettingsMessageTree(I18n::Message::ExamModeModeStandard), Settings::SettingsMessageTree(I18n::Message::ExamModeModeNoSym)};

int ExamModeConfiguration::numberOfAvailableExamMode() { //This actually depicts number of cells in examModeController
  return 3;
}

GlobalPreferences::ExamMode ExamModeConfiguration::examModeAtIndex(int index) {
  return (s_modelExamChildren[index].label() == I18n::Message::ExamModeModeStandard) ? GlobalPreferences::ExamMode::Standard : GlobalPreferences::ExamMode::NoSym;
}

I18n::Message ExamModeConfiguration::examModeActivationMessage(int index) {
  return I18n::Message::ActivateExamMode;
}

I18n::Message ExamModeConfiguration::examModeActivationWarningMessage(GlobalPreferences::ExamMode mode, int line) {
  if (mode == GlobalPreferences::ExamMode::Off) {
    I18n::Message warnings[] = {I18n::Message::ExitExamMode1, I18n::Message::ExitExamMode2, I18n::Message::Default};
    return warnings[line];
  }
  assert(mode == GlobalPreferences::ExamMode::Standard || mode == GlobalPreferences::ExamMode::NoSym);
  I18n::Message warnings[] = {I18n::Message::ActiveExamModeMessage1, I18n::Message::ActiveExamModeMessage2, I18n::Message::ActiveExamModeMessage3};
  return warnings[line];
}

KDColor ExamModeConfiguration::examModeColor(GlobalPreferences::ExamMode mode) {
  assert(mode == GlobalPreferences::ExamMode::Standard || mode == GlobalPreferences::ExamMode::NoSym);
  Preferences * preferences = Preferences::sharedPreferences();

  switch((int) preferences->colorOfLED()) {
    case 1:
      return KDColorWhite;
    case 2:
      return KDColorGreen;
    case 3:
      return KDColorBlue;
    case 4:
      return KDColorYellow;
    case 5:
      return KDColorPurple;
    case 6:
      return KDColorOrange;
    default:
      return KDColorRed;
  }
}

bool ExamModeConfiguration::appIsForbiddenInExamMode(I18n::Message appName, GlobalPreferences::ExamMode mode) {
  return false;
}

bool ExamModeConfiguration::exactExpressionsAreForbidden(GlobalPreferences::ExamMode mode) {
  return false;
}
