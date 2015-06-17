#include "Utils.h"

int main()
{
	double latitude = 0; double longitude = 0;
	int latdeg = 0; double latdecmin = 0; char NorthOrSouth = 0;
	int longdeg = 0; double longdecmin = 0; char EastOrWest = 0;
	FILE* filein = NULL;
	FILE* fileout = NULL;
	char line[MAX_BUF_LEN];
	int i = 0;
	int nbWPs = 0;

	filein = fopen("waypoints_deg_min.txt", "r");
	fileout = fopen("Waypoints.csv", "w");

	if ((filein == NULL)||(fileout == NULL))
	{
		printf("File error.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL)
	{
		if (sscanf(line, "%d %lf %c  %d %lf %c", &latdeg, &latdecmin, &NorthOrSouth, &longdeg, &longdecmin, &EastOrWest) != 6) 
		{
			printf("Invalid file.\n");
			nbWPs = i;
			fclose(fileout);
			fclose(filein);
#ifdef _DEBUG
			fprintf(stdout, "Press ENTER to continue . . . ");
			(void)getchar();
#endif // _DEBUG
			return EXIT_FAILURE;
		}
		GPSLatitudeDegDecMin2DecDeg(latdeg, latdecmin, NorthOrSouth, &latitude);
		GPSLongitudeDegDecMin2DecDeg(longdeg, longdecmin, EastOrWest, &longitude);
		fprintf(fileout, "%f;%f;\n", latitude, longitude);
		i++;
		memset(line, 0, sizeof(line));
	}
	nbWPs = i;
	fclose(fileout);
	fclose(filein);

	if (nbWPs <= 0)
	{
		printf("No waypoints.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	printf("Successfully converted %d waypoints.\n", nbWPs);

#ifdef _DEBUG
	fprintf(stdout, "Press ENTER to continue . . . ");
	(void)getchar();
#endif // _DEBUG

	return EXIT_SUCCESS;
}
