#include "Utils.h"

int main(int argc, char* argv[])
{
	double time = 0, bearing = 0, ad_interval = 0;
	int dbytes = 0;
	char data[8192+1];
	int Hour = 0, Min = 0;
	double Seconds = 0;
	int Bearing = 0;
	int Rangescale = 32;
	int Steps = 32;
	double VelocityOfSound = 1500;
	char szFileOutPath[256];
	char szTemp[256];
	FILE* filein = NULL;
	FILE* fileout = NULL;
	char line[8192+1];
	int i = 0;//, j = 0;
	int nb = 0;

	if (argc != 2)
	{
		printf("Usage : ALOG2SeanetCSV file.alog\n");
		return EXIT_FAILURE;
	}

	sprintf(szTemp, "%.249s", argv[1]);
	// Remove the extension.
	for (i = strlen(szTemp)-1; i >= 0; i--) { if (szTemp[i] == '.') break;}
	if ((i > 0)&&(i < (int)strlen(szTemp))) memset(szTemp+i, 0, strlen(szTemp)-i);
	sprintf(szFileOutPath, "%.249s.csv", szTemp);

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

	memset(data, 0, sizeof(data));

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL)
	{
		memset(data, 0, sizeof(data));
		data[sizeof(data)-1] = 0; // The last character must be a 0 to be a valid string for sscanf.
		if (sscanf(line, "%lf SONAR_RAW_DATA iSonar bearing=%lf,ad_interval=%lf,scanline=[%dx1]{%8192s}", &time, &bearing, &ad_interval, &dbytes, data) == 5) 
		{
			Hour = (int)(time/3600);
			Min = (int)(time/60)-60*Hour;
			Seconds = ((int)time)%60+time-(int)time;

			Rangescale = 10*(int)(ad_interval*0.000640*VelocityOfSound*dbytes/2.0); // 0.000640?

			Bearing = ((int)(bearing/0.05625+6400))%6400; // Angle of the transducer (0..6399 in 1/16 Gradian units, 0.05625 = (1/16)*(9/10)).

			fprintf(fileout, "%DIG,%02d:%02d:%06.3f,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%.*s", 
				Hour, Min, Seconds, 2, 16, 8967, Rangescale, 21,100,9,79,0,0, 
				Steps, Bearing, dbytes, strlen(data)-1, data
				);
			fprintf(fileout, "\n");

			fflush(fileout);

			i++;
		}
		else
		{
			printf("Skipping an invalid line in the alog file.\n");
		}
		memset(line, 0, sizeof(line));
	}
	nb = i;
	fclose(fileout);
	fclose(filein);

	if (nb <= 0)
	{
		printf("No Seanet pings.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	printf("Successfully converted %d Seanet pings.\n", nb);

#ifdef _DEBUG
	fprintf(stdout, "Press ENTER to continue . . . ");
	(void)getchar();
#endif // _DEBUG

	return EXIT_SUCCESS;
}
