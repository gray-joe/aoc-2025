#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 256

typedef struct {
	long long *items;
	size_t size;
	size_t capacity;
} LongArray;

typedef struct {
	long long low;
	long long high;
} Range;

typedef struct {
	Range *items;
	size_t size;
	size_t capacity;
} RangeArray;

void initLongArray(LongArray *arr) {
	arr->items = NULL;
	arr->size = 0;
	arr->capacity = 0;
}

void initRangeArray(RangeArray *arr) {
	arr->items = NULL;
	arr->size = 0;
	arr->capacity = 0;
}

void ensureLongCapacity(LongArray *arr) {
	if (arr->size >= arr->capacity) {
		arr->capacity = (arr->capacity == 0) ? 4 : arr->capacity * 2;
		arr->items = realloc(arr->items, arr->capacity * sizeof(long long));
	}
}

void ensureRangeCapacity(RangeArray *arr) {
	if (arr->size >= arr->capacity) {
		arr->capacity = (arr->capacity == 0) ? 4 : arr->capacity * 2;
		arr->items = realloc(arr->items, arr->capacity * sizeof(Range));
	}
}

void pushLong(LongArray *arr, long long value) {
	ensureLongCapacity(arr);
	arr->items[arr->size++] = value;
}

void pushRange(RangeArray *arr, long long low, long long high) {
	ensureRangeCapacity(arr);
	arr->items[arr->size].low = low;
	arr->items[arr->size].high = high;
	arr->size++;
}

void freeLongArray(LongArray *arr) {
	free(arr->items);
}

void freeRangeArray(RangeArray *arr) {
	free(arr->items);
}

const Range* valueInRanges(const RangeArray *ranges, long long x) {
	size_t left = 0;
	size_t right = ranges->size;

	while (left < right) {
		size_t mid = left + (right - left) / 2;
		const Range *r = &ranges->items[mid];

		if (x < r->low) {
			right = mid;
		} else if (x > r->high) {
			left = mid + 1;
		} else {
			return r;
		}
	}
	return NULL;
}

int compareRanges(const void *a, const void *b) {
	const Range *ra = a;
	const Range *rb = b;

	if (ra->low < rb->low) return -1;
	if (ra->low > rb->low) return 1;
	return 0;
}

void mergeRanges(RangeArray *arr) {
	if (arr->size == 0) return;

	size_t out = 0;

	for (size_t i = 1; i < arr->size; i++) {
		if (arr->items[i].low <= arr->items[out].high + 1) {
			if (arr->items[i].high > arr->items[out].high) {
				arr->items[out].high = arr->items[i].high;
			}
		} else {
			out++;
			arr->items[out] = arr->items[i];
		}
	}
	arr->size = out + 1;
}

int main() {
	// FILE *fp = fopen("./day_five_test.txt", "r");
	FILE *fp = fopen("./day_five_full.txt", "r");
	if (!fp) {
		perror("Could not open file");
		return 1;
	}

	LongArray available_ids;
	initLongArray(&available_ids);
	RangeArray fresh_ids;
	initRangeArray(&fresh_ids);

	char buffer[MAX_LINE_LEN];
	int partTwo = 0;

	while (fgets(buffer, sizeof(buffer), fp)) {
		buffer[strcspn(buffer, "\n")] = '\0';

		if(strlen(buffer) == 0) {
			partTwo = 1;
			continue;
		}

		if (!partTwo) {
			long long start, end;
			if (sscanf(buffer, " %lld-%lld", &start, &end) != 2) {
				printf("Invalid range: %s\n", buffer);
			}
		        pushRange(&fresh_ids, start, end);
		} else {
			long long value;
			if (sscanf(buffer, "%lld", &value) == 1) {
				pushLong(&available_ids, value);
			}
		}
	}

	fclose(fp);

	int fresh_count = 0;
	long long total_fresh_id_count = 0;

	qsort(fresh_ids.items, fresh_ids.size, sizeof(Range), compareRanges);
	mergeRanges(&fresh_ids);

	printf("FRESH IDS:\n");
	for (size_t i = 0; i < fresh_ids.size; i++) {
		printf("%lld-%lld\n", fresh_ids.items[i].low, fresh_ids.items[i].high);
		total_fresh_id_count += (fresh_ids.items[i].high - fresh_ids.items[i].low + 1);
	}


	printf("AVAILABLE IDS:\n");
	for (size_t i = 0; i < available_ids.size; i++) {
		long long value = available_ids.items[i];
		printf("%lld", value);
		const Range *r = valueInRanges(&fresh_ids, value);
		if (r) {
			printf(" IS FRESH\n");
			fresh_count++;
		} else {
			printf(" IS NOT FRESH\n");
		}
	}

	printf("Fresh Count: %d\n", fresh_count);
	printf("Total Fresh Ids: %lld\n", total_fresh_id_count);
	freeRangeArray(&fresh_ids);
	freeLongArray(&available_ids);

	return 0;
}
