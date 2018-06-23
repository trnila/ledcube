#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <string>
#include <stdexcept>
#include <cstring>
#include "mapped_file.h"

MappedFile::MappedFile(const char *path) {
	int fd = open(path, O_RDONLY);
	if (fd <= 0) {
		throw std::runtime_error(strerror(errno));
	}

	struct stat statbuf{};
	if (fstat(fd, &statbuf) < 0) {
		throw std::runtime_error(strerror(errno));
	}

	len = (size_t) statbuf.st_size;
	content = (char *) mmap(nullptr, len, PROT_READ, MAP_SHARED, fd, 0);
	if (!content) {
		throw std::runtime_error("Could not map file!");
	}
}

MappedFile::~MappedFile() {
	munmap((void *) content, len);
}