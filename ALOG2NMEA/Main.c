#include "Utils.h"

int main(int argc, char* argv[])
{
	double time = 0;
	char data[255+1];
	char szFileOutPath[256];
	char szTemp[256];
	FILE* filein = NULL;
	FILE* fileout = NULL;
	char line[255+1];
	int i = 0;

	if (argc != 2)
	{
		printf("Usage : ALOG2NMEA file.alog\n");
		return EXIT_FAILURE;
	}

	sprintf(szTemp, "%.249s", argv[1]);
	// Remove the extension.
	for (i = strlen(szTemp)-1; i >= 0; i--) { if (szTemp[i] == '.') break;}
	if ((i > 0)&&(i < (int)strlen(szTemp))) memset(szTemp+i, 0, strlen(szTemp)-i);
	sprintf(szFileOutPath, "%.249s.txt", szTemp);

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	filein = fopen(argv[1], "r");
	fileout = fopen(szFileOutPath, "w");

	if ((filein == NULL)||(fileout == NULL))
	{
		printf("File error.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL)
	{
		memset(data, 0, sizeof(data));
		data[sizeof(data)-1] = 0; // The last character must be a 0 to be a valid string for sscanf.
		if (sscanf(line, "%lf GPS_RAW iGPS %255s", &time, data) == 2) 
		{
			fprintf(fileout, "%.255s\n", data);
			fflush(fileout);
			i++;
		}
		else
		{
			printf("Skipping an invalid line in the alog file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	printf("Found %d positions.\n", i);

	fclose(fileout);
	fclose(filein);

#ifdef _DEBUG
	fprintf(stdout, "Press ENTER to continue . . . ");
	(void)getchar();
#endif // _DEBUG

	return EXIT_SUCCESS;
}
