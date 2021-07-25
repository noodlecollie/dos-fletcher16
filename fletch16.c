#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define BUFFER_SIZE 256

static int16_t Sum[2] = { 0, 0 };
static char FileData[BUFFER_SIZE];

static inline void Fl16Part(size_t length)
{
	const char* data = &FileData[0];

	while ( length > 0 )
	{
		Sum[0] = (Sum[0] + *data) & 0xFF;
		Sum[1] = (Sum[0] + Sum[1]) & 0xFF;

		++data;
		--length;
	}
}

static inline int16_t Fl16Finalise(void)
{
	int16_t check[2];
	int16_t checksum;

	check[0] = 0xFF - ((Sum[0] + Sum[1]) & 0xFF);
	check[1] = 0xFF - ((Sum[0] + check[0]) & 0xFF);

	checksum = (int16_t)(check[0] & 0xFF);
	checksum |= (int16_t)((check[1] & 0xFF) << 8);

	return checksum;
}

static inline int CheckFileValid(FILE* inFile)
{
	int errorCode;

	errorCode = ferror(inFile);

	if ( errorCode != 0 )
	{
		fprintf(stderr, "Error %d reading data from file.\n", errorCode);
		return 0;
	}

	return 1;
}

int CalcFl16(FILE* inFile, int16_t* outChecksum)
{
	*outChecksum = 0;

	while ( 1 )
	{
		size_t numRead;

		numRead = fread(FileData, 1, BUFFER_SIZE, inFile);

		if ( !CheckFileValid(inFile) )
		{
			return 0;
		}

		Fl16Part(numRead);

		if ( feof(inFile) )
		{
			break;
		}
	}

	*outChecksum = Fl16Finalise();
	return 1;
}

int main(int argc, char *argv[])
{
	int16_t checksum = 0;
	FILE* inFile = NULL;

	if ( argc != 2 )
	{
		fprintf(stderr, "Usage: FLETCH16 <file>\n");
		return 1;
	}

	inFile = fopen(argv[1], "rb");

	if ( !inFile )
	{
		fprintf(stderr, "Could not open \"%s\"\n", argv[1]);
		return 1;
	}

	if ( CalcFl16(inFile, &checksum) )
	{
		// Ensure checksum is cast/masked appropriately before printing,
		// or sometimes we can get FFFF as a prefix.
		int32_t printChecksum = checksum & 0xFFFF;

		printf("%04X\n", printChecksum);
	}

	fclose(inFile);
	return 0;
}
