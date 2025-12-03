#include <stdio.h>

int main(void) {
	int safe_value = 50;
	int zero_count = 0;

	FILE *file = fopen("./day_one_full.txt", "r");
	// FILE *file = fopen("./day_one_test.txt", "r");
	char buffer[16];

	while (fgets(buffer, sizeof(buffer), file)) {
		char direction;
		int value;

		if (sscanf(buffer, " %c%d", &direction, &value) == 2) {
			for (int i = 0; i < value; i++) {
				if (direction == 'L') {
					safe_value = (safe_value - 1 + 100) % 100;
				}
				else {
					safe_value = (safe_value + 1) % 100;
				}
				// printf("New safe value: %d\n", safe_value);
				if (safe_value == 0) {
					zero_count++;
				}
			}
		}
	}
	fclose(file);
	printf("zero_count = %d\n", zero_count);
	return 0;
}
