#include "Variables.hpp"

using namespace Variables;
using namespace geode::prelude;

void Variables::updateSettings() {
    auto mod = Mod::get();

    enabled = mod->getSettingValue<bool>("enabled");
    separators = mod->getSettingValue<bool>("separators");
    tryHandleEndTrigger = mod->getSettingValue<bool>("try-handle-end-trigger");
    
    separatorThickness = mod->getSettingValue<double>("separator-thickness");
    width = mod->getSettingValue<double>("width") / 100;
    height = mod->getSettingValue<double>("height");
    minimumLabelSize = mod->getSettingValue<double>("minimum-label-size");

    seperatorCol = ccc4FFromccc4B(mod->getSettingValue<ccColor4B>("separator-col"));
}