# mod\_auth\_sanitize\_remote\_user
Apache module that replaces problematic characters in (remote) user names.

This module can be used to replace all occurrences of a set of characters in
the user name of a remote user (`REMOTE_USER`) with a single defined character,
e.g. all spaces with underscores.
This can bu useful, for example, if you have user names that contain spaces
(e.g. if the common name of a client certificate is used as user name), but the
underlying web application does not allow user names that contain spaces.

## Security Considerations

This module was written for a very specific use-case and can cause security
issues when used in a general set-up.
The main issue is that multiple remote users can potentially map to the same
user as seen by the web application and therefore either cannot be mapped
reliably or even gain access to something that they should not have access to.
