#include <stdio.h>

unsigned char target[] = { 0x83, 0xbc, 0x04, 0x30, 0x05, 0x00, 0x00, 0xff, 0x75 };
unsigned char patch[]  = { 0x81, 0x7c, 0x24, 0x2c, 0x00, 0x04, 0x00, 0x00, 0x7c };

int doTheThing(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "No input file given\n");
		return 0;
	}

	FILE *file = fopen(argv[1], "rb+");

	if (!file) {
		fprintf(stderr, "Could not open input file\n");
		return 0;
	}

	fseek(file, 0, SEEK_SET);

	int targetPos = 0;
	int startOfTargetPos;
	int foundTarget = 0;
	unsigned char cur;

	while (!feof(file)) {
		cur = fgetc(file);

		if (cur == target[targetPos]) {
			if (targetPos == 0) startOfTargetPos = ftell(file) - 1;
			targetPos++;
			if (targetPos == sizeof(target)) {
				foundTarget = 1;
				break;
			}
		} else {
			targetPos = 0;
		}
	}

	if (foundTarget) {
		fseek(file, startOfTargetPos, SEEK_SET);
		fwrite(patch, 1, sizeof(patch), file);
	}

	fclose(file);

	return foundTarget;
}

#define let int

int main (int argc, char **argv) {
	let res = doTheThing(argc, argv);

	if (res) {
		fprintf(stderr, "Patch unable to be applied.\n");
		return 1;
	} else {
		printf("Patch successfully applied.\n");
		return 0;
	}
}
