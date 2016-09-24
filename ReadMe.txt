Waypoints.csv : Liste des lignes que doit suivre le robot. Il peut être ouvert et modifié dans Excel. La première colonne est la latitude des waypoints (maximum de 256 waypoints) formant les lignes que doit suivre le robot et la deuxième la longitude, en degrés décimaux (attention à bien utiliser le point '.' dans les nombres, et non la virgule ','). On peut aussi utiliser Google Earth en plaçant des Placemarks et en enregistrant My Places sous le fichier Waypoints.kml dans le dossier waypoints_conversions pour préparer un trajet. Il faut ensuite exécuter KML2Waypoints pour le convertir en Waypoints.csv. Waypoints2KML peut aussi être utilisé pour faire la conversion inverse. 

lognav2KML génère un fichier lognav.kml permettant d'afficher un trajet dans Google Earth à partir d'un fichier lognav.csv contenant une liste des positions en degrés décimaux, la première colonne étant le temps, la seconde la latitude la troisième la longitude et les 2 dernières 2 types d'altitude GPS, le séparateur de colonne étant le caractère ';' et le séparateur de nombre décimaux le caractère '.'.

NMEA2CSV génère un fichier compatible avec lognav2KML à partir d'un fichier lognav.txt de données brutes NMEA.

Mind the line endings in the configuration files depending on the OS (use e.g. the command dos2unix *.txt to convert line endings for Linux)!
