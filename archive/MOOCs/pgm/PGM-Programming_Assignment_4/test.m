load("PA4Sample");
dbclear("CliqueTreeCalibrate");
%dbstop("CliqueTreeCalibrate", 72);
%dbstop("FactorMaxMarginalization", 54);
%debug_on_error(1);
%debug_on_warning(1);
CliqueTreeCalibrate(MaxSumCalibrate.INPUT, 1);
