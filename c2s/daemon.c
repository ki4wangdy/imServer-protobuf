
#include <c2s.h>

#define C2S_OK		0
#define C2S_ERROR	-1

int c2s_daemon(){
	int  fd;
	switch (fork()) {
		case -1:
			return C2S_ERROR;
		case 0:
			break;
		default:
			exit(0);
	}

	if (setsid() == -1) {
		return C2S_ERROR;
	}

	umask(0);
	fd = open("/dev/null", O_RDWR);
	if (fd == -1) {
		return C2S_ERROR;
	}

	if (dup2(fd, STDIN_FILENO) == -1) {
		return C2S_ERROR;
	}
	if (dup2(fd, STDOUT_FILENO) == -1) {
		return C2S_ERROR;
	}
// 	if (fd > STDERR_FILENO) {
// 		if (close(fd) == -1) {
// 			return C2S_ERROR;
// 		}
// 	}
	return C2S_OK;
}

