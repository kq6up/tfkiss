SUBDIRS = src doc examples

EXTRA_DIST = \
    autogen.sh \
    CHANGES \
    INSTALL \
    README

distclean-local:
	rm -f config.cache

# The leading - tells make to ignore mkdir errors. Needed because
# checkinstall/installwatch intercepts mkdir() inconsistently with stat(),
# so neither plain "mkdir -p" nor "test -d || mkdir -p" survives a
# directory that another install rule already created.
install-data-hook:
	-$(MKDIR_P) $(DESTDIR)$(sysconfdir)/tfkiss
	-$(MKDIR_P) $(DESTDIR)$(localstatedir)/run
	-$(MKDIR_P) $(DESTDIR)$(localstatedir)/log
