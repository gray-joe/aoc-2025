#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 256

typedef struct {
	char **items;
	size_t count;
	size_t capacity;
} StringArray;

void initStringArray(StringArray *arr) {
	arr->count = 0;
	arr->capacity = 8;
	arr->items = malloc(arr->capacity * sizeof(char*));
}

void pushString(StringArray *arr, const char *line) {
	if (arr->count >= arr->capacity) {
		arr->capacity *= 2;
		arr->items = realloc(arr->items, arr->capacity * sizeof(char*));
	}

	arr->items[arr->count] = malloc(strlen(line) + 1);
	strcpy(arr->items[arr->count], line);
	arr->count++;
}

void freeStringArray(StringArray *arr) {
	for (size_t i = 0; i < arr->count; i++) {
		free(arr->items[i]);
	}
	free(arr->items);
}

int checkAdjacentPapers(int x, int y, StringArray lines) {
	int count = 0;

	int maxX = lines.count;
	int maxY = strlen(lines.items[0]);

	for (int dx = -1; dx <= 1; dx++) {
		for (int dy = -1; dy <= 1; dy++) {
			if (dx == 0 && dy == 0) {
				continue;
			}

			int nx = x + dx;
			int ny = y + dy;

			if (nx < 0 || nx >= maxX) continue;
			if (ny < 0 || ny >= maxY) continue;

			if (lines.items[nx][ny] == '@') {
				count++;
			}
		}
	}
	return count;
}

int main() {
	// FILE *fp = fopen("./day_four_test.txt", "r");
	FILE *fp = fopen("./day_four_full.txt", "r");
	if (!fp) {
		perror("Failed to open file");
		return 1;
	}

	StringArray lines;
	initStringArray(&lines);
	char buffer[MAX_LINE_LEN];

	while (fgets(buffer, sizeof(buffer), fp)) {
		buffer[strcspn(buffer, "\n")] = '\0';
		pushString(&lines, buffer);
	}

	fclose(fp);

	int free_papers = 0;

	for (size_t i = 0; i < lines.count; i++) {
		int l = strlen(lines.items[i]);
		// printf("Line %zu: %s\n", i, lines.items[i]);

		// if (i > 0) {
		//	printf("  Previous: %s\n", lines.items[i - 1]);
		// }

		// if (i + 1 < lines.count) {
		//	printf("  Next: %s\n", lines.items[i + 1]);
		// }

		for (int j = 0; j < l; j++) {
			if (lines.items[i][j] == '@') {
				// printf("Paper at position %zu-%d\n", i, j);
				if(checkAdjacentPapers(i, j, lines) < 4) {
					free_papers++;
					lines.items[i][j] = '.';
					// printf("Remove paper from position %zu-%d\n", i, j);
					i = 0;
					j = 0;
				}
			}
		}
	}

	printf("Total papers free: %d\n", free_papers);
	freeStringArray(&lines);
	return 0;
}

