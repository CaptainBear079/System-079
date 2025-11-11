// Execute a executable Return codes: 0 = success, 1 = error from execve, -1 = fatal error restart should be performed
int __SYS_EXECUTE__As_child(const char* path, int argc, char** argv);