// Prevent Visual Studio Intellisense from defining _WIN32 and _MSC_VER when we use 
// Visual Studio to edit Linux or Borland C++ code.
#ifdef __linux__
#	undef _WIN32
#endif // __linux__
#if defined(__GNUC__) || defined(__BORLANDC__)
#	undef _MSC_VER
#endif // defined(__GNUC__) || defined(__BORLANDC__)

#include "OSMisc.h"

//#define VAIMOS_MODE
//#define LOGSTATE_MODE
//#define ASLOGGER_MODE

//#define ALTITUDE_MODE "absolute"
#define ALTITUDE_MODE "clampToGround"

int main(int argc, char* argv[])
{
	char szFileInPath[256];
	char szFileOutPath[256];
	FILE* filein = NULL;
	FILE* fileout = NULL;
	char szTemp[256];
	char szName[256];
	char line[4096];
	unsigned int i = 0;
	double latitude = 0, longitude = 0, altitude = 0;
#ifdef VAIMOS_MODE
	int i0 = 0;
	double f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12; 
#else
#ifdef LOGSTATE_MODE
	double f0, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12; 
	double f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25;
#else
#ifdef ASLOGGER_MODE
	double f0, f1;
#else
	double utc = 0;
#endif // ASLOGGER_MODE
#endif // LOGSTATE_MODE
#endif // VAIMOS_MODE

	if (argc != 2)
	{
		strcpy(szFileInPath, "lognav.csv");
		printf("Warning : No parameter specified.\n");
		printf("Usage : lognav2KML file.csv.\n");
		printf("Default : lognav2KML %.255s.\n", szFileInPath);
	}
	else
	{
		sprintf(szFileInPath, "%.249s", argv[1]);
	}

	strcpy(szTemp, szFileInPath);
	RemoveExtensionInFilePath(szTemp);
	sprintf(szFileOutPath, "%.249s.kml", szTemp);
	strcpy(szName, szTemp);
	RemovePathInFilePath(szName);

	printf("Check and change if needed\n\n");
	printf("Control Panel\\Regional and Language Options\\Customize\\Numbers\n\n");
	printf("if you do not know if you should use a '.' or a ',' in floating points numbers\n\n");
	printf("or a ';' or ',' in list separators.\n\n");

	filein = fopen(szFileInPath, "r");
	if (filein == NULL)
	{
		printf("Unable to open CSV file.\n");
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fileout = fopen(szFileOutPath, "w");
	if (fileout == NULL)
	{
		printf("Unable to create KML file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	fprintf(fileout, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(fileout, "<kml xmlns=\"http://www.opengis.net/kml/2.2\" xmlns:gx=\"http://www.google.com/kml/ext/2.2\" xmlns:kml=\"http://www.opengis.net/kml/2.2\" xmlns:atom=\"http://www.w3.org/2005/Atom\">\n");
	//fprintf(fileout, "<Folder>\n");
	//fprintf(fileout, "<name>%.249s</name>\n", szName);
	//fprintf(fileout, "<open>1</open>\n");

	fprintf(fileout, "<Document>\n");
	fprintf(fileout, "<name>%.249s</name>\n", szName);
	fprintf(fileout, "\t<Placemark>\n\t\t<name>%.249s</name>\n", szName);
	fprintf(fileout, "\t\t<Style>\n\t\t\t<LineStyle>\n\t\t\t\t<color>ff00ff00</color>\n\t\t\t\t<width>2</width>\n\t\t\t</LineStyle>\n\t\t</Style>\n");
	fprintf(fileout, "\t\t<LineString>\n\t\t\t<extrude>0</extrude>\n\t\t\t<tessellate>0</tessellate>\n\t\t\t<altitudeMode>"ALTITUDE_MODE"</altitudeMode>\n\t\t\t<coordinates>\n");

	printf("Converting...\n");

	i = 0;
	memset(line, 0, sizeof(line));
	while (fgets3(filein, line, sizeof(line)) != NULL) 
	{
#ifdef VAIMOS_MODE
		if ((sscanf(line, "%d;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf", 
			&i0, &f0, &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12,
			&latitude, &longitude) == 16)&&(latitude != 0)&&(longitude != 0))
#else
#ifdef LOGSTATE_MODE
		if ((sscanf(line, "%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf;%lf", 
			&f0, &f1, &f2, &f3, &f4, &f5, &f6, &f7, &f8, &f9, &f10, &f11, &f12, &f13, &f14, &f15, &f16, &f17, &f18, &f19, &f20, &f21, &f22, &f23, &f24, &f25,
			&latitude, &longitude) == 28)&&(latitude != 0)&&(longitude != 0))
#else
#ifdef ASLOGGER_MODE
		if ((sscanf(line, "%lf;%lf;%lf;%lf;%lf", &f0, &f1, &latitude, &longitude, &altitude) == 5)&&
			(latitude != 0)&&(longitude != 0))
#else
		if ((sscanf(line, "%lf;%lf;%lf;%lf", &utc, &latitude, &longitude, &altitude) == 4)&&
			(latitude != 0)&&(longitude != 0)) 
#endif // ASLOGGER_MODE
#endif // LOGSTATE_MODE
#endif // VAIMOS_MODE
		{
			if ((i%65504) == 65503)
			{
				// Divide in several placemarks if too many coordinates.
				fprintf(fileout, "\t\t\t</coordinates>\n\t\t</LineString>\n\t</Placemark>\n");
				fprintf(fileout, "\t<Placemark>\n\t\t<name>%.249s</name>\n", szName);
				fprintf(fileout, "\t\t<Style>\n\t\t\t<LineStyle>\n\t\t\t\t<color>ff00ff00</color>\n\t\t\t\t<width>2</width>\n\t\t\t</LineStyle>\n\t\t</Style>\n");
				fprintf(fileout, "\t\t<LineString>\n\t\t\t<extrude>0</extrude>\n\t\t\t<tessellate>0</tessellate>\n\t\t\t<altitudeMode>"ALTITUDE_MODE"</altitudeMode>\n\t\t\t<coordinates>\n");
			}
			fprintf(fileout, "%f,%f,%f ", longitude, latitude, altitude);
			i++;
		}
#ifdef VAIMOS_MODE
#else
#ifdef LOGSTATE_MODE
#else
#ifdef ASLOGGER_MODE
		else if ((sscanf(line, "%lf;%lf;%lf;%lf", &f0, &f1, &latitude, &longitude) == 4)&&
			(latitude != 0)&&(longitude != 0))
#else
		else if ((sscanf(line, "%lf;%lf;%lf", &utc, &latitude, &longitude) == 3)&&
			(latitude != 0)&&(longitude != 0))
#endif // ASLOGGER_MODE
#endif // LOGSTATE_MODE
#endif // VAIMOS_MODE
#ifdef VAIMOS_MODE
#else
#ifdef LOGSTATE_MODE
#else
		{
			if ((i%65504) == 65503)
			{
				// Divide in several placemarks if too many coordinates.
				fprintf(fileout, "\t\t\t</coordinates>\n\t\t</LineString>\n\t</Placemark>\n");
				fprintf(fileout, "\t<Placemark>\n\t\t<name>%.249s</name>\n", szName);
				fprintf(fileout, "\t\t<Style>\n\t\t\t<LineStyle>\n\t\t\t\t<color>ff00ff00</color>\n\t\t\t\t<width>2</width>\n\t\t\t</LineStyle>\n\t\t</Style>\n");
				fprintf(fileout, "\t\t<LineString>\n\t\t\t<extrude>0</extrude>\n\t\t\t<tessellate>0</tessellate>\n\t\t\t<altitudeMode>clampToGround</altitudeMode>\n\t\t\t<coordinates>\n");
			}
			// Optimization for the altitude : integer.
			fprintf(fileout, "%f,%f,%d ", longitude, latitude, 0);
			i++;
		}
#endif // LOGSTATE_MODE
#endif // VAIMOS_MODE
		else
		{
			printf("Skipping an invalid line in the CSV file.\n");
		}
		memset(line, 0, sizeof(line));
	}

	fprintf(fileout, "\t\t\t</coordinates>\n\t\t</LineString>\n\t</Placemark>\n");
	fprintf(fileout, "</Document>\n");
	//fprintf(fileout, "</Folder>\n");
	fprintf(fileout, "</kml>\n");

	printf("Found %u positions.\n", i);

	if (fclose(fileout) != EXIT_SUCCESS) 
	{
		printf("Error closing KML file.\n");
		fclose(filein);
#ifdef _DEBUG
		fprintf(stdout, "Press ENTER to continue . . . ");
		(void)getchar();
#endif // _DEBUG
		return EXIT_FAILURE;
	}

	if (fclose(filein) != EXIT_SUCCESS) 
	{
		printf("Error closing CSV file.\n");
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
