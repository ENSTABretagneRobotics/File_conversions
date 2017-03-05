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

	filein = fopen("Waypoints.csv", "r");
	fileout = fopen("waypoints_deg_min.txt", "w");

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
		if (sscanf(line, "%lf;%lf", &latitude, &longitude) != 2) 
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
		GPSLatitudeDecDeg2DegDecMin(latitude, &latdeg, &latdecmin, &NorthOrSouth);
		GPSLongitudeDecDeg2DegDecMin(longitude, &longdeg, &longdecmin, &EastOrWest);
		fprintf(fileout, "%d %.8f %c  %d %.8f %c\n", latdeg, latdecmin, NorthOrSouth, longdeg, longdecmin, EastOrWest);
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
