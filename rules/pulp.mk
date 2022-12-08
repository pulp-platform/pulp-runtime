ifdef PULPRUN_PLATFORM
platform=$(PULPRUN_PLATFORM)
$(info Setting platform to $(platform))
endif
-include $(PULPRT_HOME)/rules/pulpos/targets/$(PULPRT_TARGET).mk
