subdirs :=
subdirs += util
subdirs += mio
subdirs += sx
subdirs += imProtocal-protobuf
subdirs += db
subdirs += sm
subdirs += c2s

include ./targets.mk

$(possible_targets_list):
	@$(MAKE) $(subdirs) target=$@

.PHONY: $(subdirs)
$(subdirs):
	@$(MAKE) -C $@ $(target)



