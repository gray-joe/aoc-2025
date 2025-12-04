#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int digit_count(unsigned long long n) {
	if (n == 0) return 1;
	int count = 0;
	while (n > 0) {
		n /= 10;
		count++;
	}
	return count;
}

int pow10(unsigned long long n) {
	int r = 1;
	while (n--) r *= 10;
	return r;
}

int is_repeated_pattern(unsigned long long n) {
	char s[32];
	sprintf(s, "%llu", n);

	int l = strlen(s);
	for (int i = 1; i <= l / 2; i++) {
		if (l % i != 0)
			continue;

		int check = 1;
		for (int j = i; j < l; j++) {
			if (s[j] != s[j % i]) {
				check = 0;
				break;
			}
		}
		if (check) {
			return 1;
		}
	}
	return 0;
}

int main() {
	unsigned long long *arr = NULL;
	int count = 0;
	unsigned long long sum = 0;

	// FILE *fp = fopen("./day_two_test.txt", "r");
	FILE *fp = fopen("./day_two_full.txt", "r");
	if (!fp) {
		perror("File open error");
		return 1;
	}

	char line[4096];
	if (fgets(line, sizeof(line), fp) != NULL) {

		char *token = strtok(line, ",");
		while (token != NULL) {
			char *dash = strchr(token, '-');
			if (dash) {
				*dash = '\0';
				unsigned long long left  = strtoull(token, NULL, 10);
				unsigned long long right = strtoull(dash + 1, NULL, 10);
				printf("Range: %llu - %llu\n", left, right);
				for (unsigned long long i = left; i <= right; i++) {
					int l = digit_count(i);
	 				int divisor = pow10(l / 2);

					if (((l % 2 == 0) && ((i / divisor) == (i % divisor))) || is_repeated_pattern(i)) {
						unsigned long long *tmp = realloc(arr, (count + 1) * sizeof(unsigned long long));
						if (tmp == NULL) {
							printf("Allocation failed!\n");
							free(arr);
							return 1;
						}
						arr = tmp;
						arr[count] = i;
						count++;
					}
				}
			}
			token = strtok(NULL, ",");
		}
	}
	printf("Stored values:\n");
	for (int i = 0; i < count; i++) {
		printf("%llu ", arr[i]);
		sum += arr[i];
	}
	printf("\n");
	printf("Sum: %llu\n", sum);
	free(arr);
	fclose(fp);
	return 0;
}

