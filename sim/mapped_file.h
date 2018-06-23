#pragma once

class MappedFile {
public:
	MappedFile(const char *path);

	~MappedFile();

	const char *get_content() {
		return content;
	}

private:
	const char *content;
	size_t len;
};