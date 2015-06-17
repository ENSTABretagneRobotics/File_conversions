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
	FILE* alldatafile = NULL;
	FILE* kmlfile = NULL;
	char line[4096];
	unsigned int i = 0;
	double t = 0, latitude = 0, longitude = 0, altitude = 0, v = 0, bearing = 0, psi = 0, theta = 0, phi = 0;
	time_t tt;
	struct tm *timeptr = NULL;
	char timebuf[32];

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	alldatafile = fopen("alldata.csv", "r");
	if (alldatafile == NULL)
	{
		printf("Unable to open csv file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	kmlfile = fopen("alldata.kml", "w");
	if (kmlfile == NULL)
	{
		printf("Unable to create kml file.\n");
		fclose(alldatafile);
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
	fprintf(kmlfile, "\t<name>alldata</name>\n\t<open>1</open>\t<visibility>1</visibility>\n\n");
	//fprintf(kmlfile, "\t<Style id=\"check-hide-children\">\n");
	//fprintf(kmlfile, "\t\t<ListStyle>\n");
	//fprintf(kmlfile, "\t\t\t<listItemType>checkHideChildren</listItemType>\n");
	//fprintf(kmlfile, "\t\t</ListStyle>\n");
	//fprintf(kmlfile, "\t</Style>\n");
	//fprintf(kmlfile, "\t<styleUrl>#check-hide-children</styleUrl>\n");

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(alldatafile, line, sizeof(line)) != NULL) 
	{
		if (sscanf(line, "%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf", &t, &latitude, &longitude, &altitude, &v, &bearing, &psi, &theta, &phi) == 9) 
		{
			if (i%10 == 0)
			{
				tt = (time_t)(t/1000);
				timeptr = gmtime(&tt);
				if (timeptr != NULL) strftime(timebuf, 32, "%Y-%m-%dT%H:%M:%SZ", timeptr);
				fprintf(kmlfile, "\t<Placemark>\n");
				fprintf(kmlfile, "\t\t<visibility>0</visibility>\n");
				fprintf(kmlfile, "\t\t<TimeStamp>\n\t\t\t<when>%.32s</when>\n\t\t</TimeStamp>\n", timebuf);
				//fprintf(kmlfile, "\t\t<Model>\n\t\t\t<altitudeMode>absolute</altitudeMode>\n\t\t\t<Location>\n");
				fprintf(kmlfile, "\t\t<Model>\n\t\t\t<altitudeMode>clampToGround</altitudeMode>\n\t\t\t<Location>\n");
				fprintf(kmlfile, "\t\t\t\t<longitude>%f</longitude>\n", longitude);
				fprintf(kmlfile, "\t\t\t\t<latitude>%f</latitude>\n", latitude);
				fprintf(kmlfile, "\t\t\t\t<altitude>%d</altitude>\n", (int)altitude);
				fprintf(kmlfile, "\t\t\t</Location>\n\t\t\t<Orientation>\n");
				fprintf(kmlfile, "\t\t\t\t<heading>%d</heading>\n", (int)((fmod_2PI(3*M_PI/2.0-psi)+M_PI)*180.0/M_PI));
				fprintf(kmlfile, "\t\t\t\t<tilt>%d</tilt>\n", (int)(theta*180.0/M_PI));
				fprintf(kmlfile, "\t\t\t\t<roll>%d</roll>\n", (int)(-phi*180.0/M_PI));
				//fprintf(kmlfile, "\t\t\t</Orientation>\n\t\t\t<Scale>\n\t\t\t\t<x>50</x>\n\t\t\t\t<y>50</y>\n\t\t\t\t<z>50</z>\n\t\t\t</Scale>\n");
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
		fclose(alldatafile);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(alldatafile) != EXIT_SUCCESS) 
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
