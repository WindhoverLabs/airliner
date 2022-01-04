str(CFETime.getTimeSinceEpoch(pvs[1].getValue().getValue(), pvs[2].getValue().getValue(),
                              registry.get(display.getMacroValue("REGISTRY_PATH") + "/config/CFE_TIME_EPOCH_YEAR/value"),
                              registry.get(display.getMacroValue("REGISTRY_PATH") + "/config/CFE_TIME_EPOCH_DAY/value"),
                              registry.get(display.getMacroValue("REGISTRY_PATH") + "/config/CFE_TIME_EPOCH_HOUR/value"),
                              registry.get(display.getMacroValue("REGISTRY_PATH") + "/config/CFE_TIME_EPOCH_MINUTE/value"),
                              registry.get(display.getMacroValue("REGISTRY_PATH") + "/config/CFE_TIME_EPOCH_SECOND/value")))