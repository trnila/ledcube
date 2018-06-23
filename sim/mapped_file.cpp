#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <string>
#include "mapped_file.h"

MappedFile::MappedFile(const char *path) {
	int fd = open(path, O_RDONLY);
	if (fd <= 0) {
		perror("open");
	}

	struct stat statbuf{};
	if (fstat(fd, &statbuf) < 0) {
		perror("fstat error");
	}

	len = (size_t) statbuf.st_size;
	content = (char *) mmap(nullptr, len, PROT_READ, MAP_SHARED, fd, 0);
}

MappedFile::~MappedFile() {
	munmap((void *) content, len);
}