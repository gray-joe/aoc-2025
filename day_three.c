#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int max;
	int index;
} MaxValue;

MaxValue get_largest_value(const char *line, int start, int end) {
	MaxValue result = { .max = -1, .index = start };

	if (start > end) {
		return result;
	}

	int current_max = -1;
	int current_index = start;
	for (int i = start; i <= end; i++) {
		int n = line[i] - '0';
		if (n > current_max) {
			current_max = n;
			current_index = i;
			if (current_max == 9) break;
		}
	}
	result.max = current_max;
	result.index = current_index + 1;
	return result;
}

int get_n_largest_values(const char *line, int n, int solution[]) {
	int l = strlen(line);
	if (n <= 0 || l < n) return -1;

	int start = 0;
	int end = l - n;
	MaxValue n0 = get_largest_value(line, start, end);
	solution[0] = n0.max;
	start = n0.index;

	for (int i = 1; i < n; i++) {
		int remaining = n - i;
		int end_i = l - remaining;
		if (start > end_i) {
			start = end_i;
		}
		MaxValue m = get_largest_value(line, start, end_i);
		solution[i] = m.max;
		start = m.index;
	}
	return 0;
}

int main() {
	// FILE *fp = fopen("day_three_test.txt", "r");
	FILE *fp = fopen("day_three_full.txt", "r");
	if (!fp) {
		perror("Failed to open file");
		return 1;
	}

	long long total = 0;
	char *line = NULL;
	size_t len = 0;

	while (getline(&line, &len, fp) != -1) {
		line[strcspn(line, "\n")] = '\0';
		
		int l = strlen(line);
		if (l < 12) {
			fprintf(stderr, "line too short: %s\n", line);
			continue;
		}

		int values[12];
		if (get_n_largest_values(line, 12, values) != 0) {
			fprintf(stderr, "failed to get values for line: %s\n", line);
			continue;
		}

		long long line_total = 0;
		for (int i = 0; i < 12; i++) {
			line_total = line_total * 10 + values[i];
		}
		total += line_total;

		printf("%s\n", line);
		printf("adding on: %lld\n", line_total);
		printf("sanity check: ");
		for (int i = 0; i < 12; i++) putchar('0' + values[i]);
		putchar('\n');
	}
	printf("total: %lld\n", total);

	free(line);
	fclose(fp);
	return 0;
}

