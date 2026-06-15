SUBDIRS = src doc examples

EXTRA_DIST = \
    CHANGES \
    INSTALL \
    README

distclean-local:
	rm -f config.cache

install-data-hook:
	$(MKDIR_P) $(DESTDIR)$(sysconfdir)/tfkiss
	$(MKDIR_P) $(DESTDIR)$(localstatedir)/run
	$(MKDIR_P) $(DESTDIR)$(localstatedir)/log
