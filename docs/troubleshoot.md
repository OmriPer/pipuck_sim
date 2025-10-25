# Troubleshooting Guide
## Common Issues and Solutions
| Issue | Description | Solution |
|-------|-------------|----------|
|argos3: symbol lookup error: /snap/core20/current/lib/x86_64-linux-gnu/libpthread.so.0: undefined symbol: __libc_pthread_init, version GLIBC_PRIVATE| This error usually occurs when vscode attempts to load the snap version glibc instead of the system version.| disable env inheritance for vscode by adding `"terminal.integrated.inheritEnv": false` to your user settings. or run from an external terminal. |