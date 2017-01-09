include(PICMake)
pi_add_target(pi_cv ${MAKE_TYPE} . REQUIRED OPENCV DEPENDENCY pi_base)
