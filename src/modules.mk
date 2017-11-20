mod_auth_sanitize_remote_user.la: mod_auth_sanitize_remote_user.slo
	$(SH_LINK) -rpath $(libexecdir) -module -avoid-version  mod_auth_sanitize_remote_user.lo
DISTCLEAN_TARGETS = modules.mk
shared =  mod_auth_sanitize_remote_user.la
