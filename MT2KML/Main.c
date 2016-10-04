// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "Globals.h"

int main()
{
	FILE* filein = NULL;
	FILE* kmlfile = NULL;
	char line[4096];
	unsigned int i = 0;
	unsigned int step = 50; // To change if too many data...
	double d0 = 0, d1 = 0, d2 = 0, d3 = 0, d4 = 0, d5 = 0, d6 = 0, d7 = 0, d8 = 0, 
		d9 = 0, d10 = 0, d11 = 0, d12 = 0, d13 = 0, 
		roll = 0, pitch = 0, yaw = 0, latitude = 0, longitude = 0, altitude = 0;
	int UTC_Year = 0, UTC_Month = 0, UTC_Day = 0, UTC_Hour = 0, UTC_Minute = 0; 
	double UTC_Seconds = 0;
	struct timeval tv;
	time_t tt;
	struct tm* timeptr = NULL;
	char timebuf[32];

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	filein = fopen("MT.csv", "r");
	if (filein == NULL)
	{
		printf("Unable to open csv file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	kmlfile = fopen("MT.kml", "w");
	if (kmlfile == NULL)
	{
		printf("Unable to create kml file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	strcpy(timebuf, "0");

	fprintf(kmlfile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(kmlfile, "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n");
	//fprintf(kmlfile, "<Folder>\n");
	//fprintf(kmlfile, "<name>alldata</name>\n");
	//fprintf(kmlfile, "<open>1</open>\n");
	fprintf(kmlfile, "<Document>\n");
	fprintf(kmlfile, "\t<name>alldata</name>\n\t<open>1</open>\n\t<visibility>1</visibility>\n");
	//fprintf(kmlfile, "\t<Style id=\"check-hide-children\">\n");
	//fprintf(kmlfile, "\t\t<ListStyle>\n");
	//fprintf(kmlfile, "\t\t\t<listItemType>checkHideChildren</listItemType>\n");
	//fprintf(kmlfile, "\t\t</ListStyle>\n");
	//fprintf(kmlfile, "\t</Style>\n");
	//fprintf(kmlfile, "\t<styleUrl>#check-hide-children</styleUrl>\n");

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL) 
	{
		if (sscanf(line, 
			"%lf;%lf;%lf;"
			"%d;%d;%d;%d;%d;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%d;%d",
			&d0, &d1, &d2, 
			&UTC_Year, &UTC_Month, &UTC_Day, &UTC_Hour, &UTC_Minute, &UTC_Seconds, 
			&d3, &d4, &d5, &d6, &d7, &d8, &d9, &d10, 
			&roll, &pitch, &yaw, 
			&latitude, &longitude, &altitude,
			&d11, &d12, &d13, 
			&tv.tv_sec, &tv.tv_usec) == 28) 
		{
			if (i%step == 0)
			{
				tt = (time_t)tv.tv_sec;
				timeptr = gmtime(&tt);
				if (timeptr != NULL) strftime(timebuf, 32, "%Y-%m-%dT%H:%M:%SZ", timeptr);
				fprintf(kmlfile, "\t<Placemark>\n");
				fprintf(kmlfile, "\t\t<visibility>0</visibility>\n");
				fprintf(kmlfile, "\t\t<TimeStamp>\n\t\t\t<when>%.32s</when>\n\t\t</TimeStamp>\n", timebuf);
				fprintf(kmlfile, "\t\t<Model>\n\t\t\t<altitudeMode>clampToGround</altitudeMode>\n\t\t\t<Location>\n");
				fprintf(kmlfile, "\t\t\t\t<longitude>%f</longitude>\n", longitude);
				fprintf(kmlfile, "\t\t\t\t<latitude>%f</latitude>\n", latitude);
				fprintf(kmlfile, "\t\t\t\t<altitude>%d</altitude>\n", (int)altitude*0);
				fprintf(kmlfile, "\t\t\t</Location>\n\t\t\t<Orientation>\n");
				fprintf(kmlfile, "\t\t\t\t<heading>%d</heading>\n", ((int)-yaw+720)%360);
				fprintf(kmlfile, "\t\t\t\t<tilt>%d</tilt>\n", (int)pitch);
				fprintf(kmlfile, "\t\t\t\t<roll>%d</roll>\n", (int)-roll);
				fprintf(kmlfile, "\t\t\t</Orientation>\n\t\t\t<Scale>\n\t\t\t\t<x>1</x>\n\t\t\t\t<y>1</y>\n\t\t\t\t<z>1</z>\n\t\t\t</Scale>\n");
				fprintf(kmlfile, "\t\t\t<Link>\n\t\t\t\t<href>untitled.dae</href>\n\t\t\t</Link>\n");
				fprintf(kmlfile, "\t\t\t<ResourceMap>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_0.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_0.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_1.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_1.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_2.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_2.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_3.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_3.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_4.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_4.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_5.JPG</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_5.JPG</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_6.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_6.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t</ResourceMap>\n");
				fprintf(kmlfile, "\t\t</Model>\n");
				fprintf(kmlfile, "\t</Placemark>\n");
			}
			i++;
		}
		else if (sscanf(line, 
			"%lf;%lf;%lf;"
			"%d;%d;%d;%d;%d;%lf;"
			"%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf;"
			"%lf;%lf;%lf",
			&d0, &d1, &d2, 
			&UTC_Year, &UTC_Month, &UTC_Day, &UTC_Hour, &UTC_Minute, &UTC_Seconds, 
			&d3, &d4, &d5, &d6, &d7, &d8, &d9, &d10, 
			&roll, &pitch, &yaw, 
			&latitude, &longitude, &altitude,
			&d11, &d12, &d13) == 26) 
		{
			if (i%step == 0)
			{
				sprintf(timebuf, "%04d-%02d-%02dT%02d:%02d:%02dZ", UTC_Year, UTC_Month, UTC_Day, UTC_Hour, UTC_Minute, (int)UTC_Seconds);
				fprintf(kmlfile, "\t<Placemark>\n");
				fprintf(kmlfile, "\t\t<visibility>0</visibility>\n");
				fprintf(kmlfile, "\t\t<TimeStamp>\n\t\t\t<when>%.32s</when>\n\t\t</TimeStamp>\n", timebuf);
				fprintf(kmlfile, "\t\t<Model>\n\t\t\t<altitudeMode>absolute</altitudeMode>\n\t\t\t<Location>\n");
				fprintf(kmlfile, "\t\t\t\t<longitude>%f</longitude>\n", longitude);
				fprintf(kmlfile, "\t\t\t\t<latitude>%f</latitude>\n", latitude);
				fprintf(kmlfile, "\t\t\t\t<altitude>%d</altitude>\n", (int)altitude);
				fprintf(kmlfile, "\t\t\t</Location>\n\t\t\t<Orientation>\n");
				fprintf(kmlfile, "\t\t\t\t<heading>%d</heading>\n", ((int)-yaw+720)%360);
				fprintf(kmlfile, "\t\t\t\t<tilt>%d</tilt>\n", (int)pitch);
				fprintf(kmlfile, "\t\t\t\t<roll>%d</roll>\n", (int)-roll);
				fprintf(kmlfile, "\t\t\t</Orientation>\n\t\t\t<Scale>\n\t\t\t\t<x>1</x>\n\t\t\t\t<y>1</y>\n\t\t\t\t<z>1</z>\n\t\t\t</Scale>\n");
				fprintf(kmlfile, "\t\t\t<Link>\n\t\t\t\t<href>untitled.dae</href>\n\t\t\t</Link>\n");
				fprintf(kmlfile, "\t\t\t<ResourceMap>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_0.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_0.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_1.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_1.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_2.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_2.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_3.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_3.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_4.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_4.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_5.JPG</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_5.JPG</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t\t<Alias>\n\t\t\t\t\t<targetHref>texture_6.jpg</targetHref>\n\t\t\t\t\t<sourceHref>untitled/texture_6.jpg</sourceHref>\n\t\t\t\t</Alias>\n");
				fprintf(kmlfile, "\t\t\t</ResourceMap>\n");
				fprintf(kmlfile, "\t\t</Model>\n");
				fprintf(kmlfile, "\t</Placemark>\n");
			}
			i++;
		}
		else
		{
			printf("Skipping an invalid line in the csv file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	fprintf(kmlfile, "</Document>\n");
	//fprintf(kmlfile, "</Folder>\n");
	fprintf(kmlfile, "</kml>\n");

	printf("Found %u positions.\n", i);

	if (fclose(kmlfile) != EXIT_SUCCESS) 
	{
		printf("Error closing kml file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein) != EXIT_SUCCESS) 
	{
		printf("Error closing csv file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

#ifdef _DEBUG
	fprintf(stdout, "Press ENTER to continue . . . ");
	(void)getchar();
#endif // _DEBUG

	return EXIT_SUCCESS;
}
