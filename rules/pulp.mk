platform ?= rtl
ifdef PULPRUN_PLATFORM
platform=$(PULPRUN_PLATFORM)
endif
$(info Setting platform to $(platform))
-include $(PULPRT_HOME)/rules/pulpos/targets/$(PULPRT_TARGET).mk
