include apps/shared/Makefile
include apps/home/Makefile
include apps/on_boarding/Makefile
include apps/hardware_test/Makefile
include apps/usb/Makefile
apps =

# All selected apps are included. Each Makefile below is responsible for setting
# the $apps variable (name of the app class) and the $app_headers
# (path to the apps header).
$(foreach i,${EPSILON_APPS},$(eval include apps/$(i)/Makefile))

app_src += $(addprefix apps/,\
  apps_container.cpp \
  apps_container_storage.cpp \
  apps_window.cpp \
  backlight_dimming_timer.cpp \
  battery_timer.cpp \
  battery_view.cpp \
  empty_battery_window.cpp \
  exam_pop_up_controller.cpp \
  global_preferences.cpp \
  i18n.py \
  lock_view.cpp \
  main.cpp \
  math_toolbox.cpp \
  shift_alpha_lock_view.cpp \
  suspend_timer.cpp \
  title_bar_view.cpp \
  variable_box_controller.cpp \
  variable_box_empty_controller.cpp \
)

apps_launch_on_boarding_src += apps/apps_container_launch_on_boarding.cpp
apps_launch_default_src += apps/apps_container_launch_default.cpp
apps_prompt_none_src += apps/apps_container_prompt_none.cpp
apps_prompt_beta_src += apps/apps_container_prompt_beta.cpp
apps_prompt_update_src += apps/apps_container_prompt_update.cpp

snapshots_declaration = $(foreach i,$(apps),$(i)::Snapshot m_snapshot$(subst :,,$(i))Snapshot;)
apps_declaration = $(foreach i,$(apps),$(i) m_$(subst :,,$(i));)
snapshots_construction = $(foreach i,$(apps),,m_snapshot$(subst :,,$(i))Snapshot())
snapshots_list = $(foreach i,$(apps),,&m_snapshot$(subst :,,$(i))Snapshot)
snapshots_count = $(words $(apps))
snapshot_includes = $(foreach i,$(app_headers),-include $(i) )
epsilon_app_names = '$(foreach i,${EPSILON_APPS},"$(i)", )'

$(call object_for,apps/apps_container_storage.cpp apps/apps_container.cpp apps/main.cpp): CXXFLAGS += $(snapshot_includes) -DAPPS_CONTAINER_APPS_DECLARATION="$(apps_declaration)" -DAPPS_CONTAINER_SNAPSHOT_DECLARATIONS="$(snapshots_declaration)" -DAPPS_CONTAINER_SNAPSHOT_CONSTRUCTORS="$(snapshots_construction)" -DAPPS_CONTAINER_SNAPSHOT_LIST="$(snapshots_list)" -DAPPS_CONTAINER_SNAPSHOT_COUNT=$(snapshots_count) -DEPSILON_APPS_NAMES=$(epsilon_app_names) -DUSERNAME="$(USERNAME)"

# I18n file generation

# The header is refered to as <apps/i18n.h> so make sure it's findable this way
SFLAGS += -I$(BUILD_DIR)

i18n_files += $(addprefix apps/language_,$(addsuffix .universal.i18n, $(EPSILON_I18N)))
i18n_files += $(addprefix apps/,\
  shared.de.i18n\
  shared.en.i18n\
  shared.es.i18n\
  shared.fr.i18n\
  shared.pt.i18n\
  shared.universal.i18n\
  toolbox.de.i18n\
  toolbox.en.i18n\
  toolbox.es.i18n\
  toolbox.fr.i18n\
  toolbox.pt.i18n\
  variables.de.i18n\
  variables.en.i18n\
  variables.es.i18n\
  variables.fr.i18n\
  variables.pt.i18n\
)

$(eval $(call rule_for, \
  I18N, \
  apps/i18n.cpp, \
  $(i18n_files), \
  $$(PYTHON) apps/i18n.py --header $$(subst .cpp,.h,$$@) --implementation $$@ --locales $$(EPSILON_I18N) --files $$^ \
))

$(BUILD_DIR)/apps/i18n.h: $(BUILD_DIR)/apps/i18n.cpp

# Handle PNG files

$(eval $(call depends_on_image,apps/title_bar_view.cpp,apps/exam_icon.png))

all_app_src = $(app_src) $(epsilon_src) $(apps_launch_on_boarding_src) $(apps_launch_default_src) $(apps_prompt_none_src) $(apps_container_prompt_update) $(apps_prompt_beta_src) $(tests_src)

$(call object_for,$(all_app_src)): $(BUILD_DIR)/apps/i18n.h
$(call object_for,$(all_app_src)): $(BUILD_DIR)/python/port/genhdr/qstrdefs.generated.h

apps_tests_src = $(app_calculation_test_src) $(app_probability_test_src) $(app_regression_test_src) $(app_sequence_test_src) $(app_shared_test_src) $(app_statistics_test_src) $(app_solver_test_src)

# Configure variants
apps_all_src = $(app_src)
apps_all_src += $(apps_launch_default_src) $(apps_launch_on_boarding_src
apps_all_src += $(apps_prompt_none_src) $(apps_prompt_update_src) $(apps_prompt_beta_src)

apps_default_src = $(app_src) $(apps_launch_default_src) $(apps_prompt_none_src)
apps_onboarding_src = $(app_src) $(apps_launch_on_boarding_src) $(apps_prompt_none_src)
apps_onboarding_update_src = $(app_src) $(apps_launch_on_boarding_src) $(apps_prompt_update_src)
apps_onboarding_beta_src = $(app_src) $(apps_launch_on_boarding_src) $(apps_prompt_beta_src)
