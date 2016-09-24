Waypoints.csv : Liste des lignes que doit suivre le robot. Il peut �tre ouvert et modifi� dans Excel. La premi�re colonne est la latitude des waypoints (maximum de 256 waypoints) formant les lignes que doit suivre le robot et la deuxi�me la longitude, en degr�s d�cimaux (attention � bien utiliser le point '.' dans les nombres, et non la virgule ','). On peut aussi utiliser Google Earth en pla�ant des Placemarks et en enregistrant My Places sous le fichier Waypoints.kml dans le dossier waypoints_conversions pour pr�parer un trajet. Il faut ensuite ex�cuter KML2Waypoints pour le convertir en Waypoints.csv. Waypoints2KML peut aussi �tre utilis� pour faire la conversion inverse. 

lognav2KML g�n�re un fichier lognav.kml permettant d'afficher un trajet dans Google Earth � partir d'un fichier lognav.csv contenant une liste des positions en degr�s d�cimaux, la premi�re colonne �tant le temps, la seconde la latitude la troisi�me la longitude et les 2 derni�res 2 types d'altitude GPS, le s�parateur de colonne �tant le caract�re ';' et le s�parateur de nombre d�cimaux le caract�re '.'.

NMEA2CSV g�n�re un fichier compatible avec lognav2KML � partir d'un fichier lognav.txt de donn�es brutes NMEA.

Mind the line endings in the configuration files depending on the OS (use e.g. the command dos2unix *.txt to convert line endings for Linux)!
