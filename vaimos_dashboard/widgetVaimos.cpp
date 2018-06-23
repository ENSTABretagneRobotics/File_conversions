#include <QtGui>
#include <list>
#include "widgetVaimos.h"
#include "ui_mainwindow.h"

extern int k0;
extern int kmax;
extern int ks0;
extern int ks1;
extern int Echelle;
extern int step;
extern bool wind;
extern bool sail1;
extern bool sail2;
extern bool go;
extern bool thickline;
extern bool text;
extern bool filteredwinddirmode;
extern bool motorboatmode;
extern bool recalce;
extern bool resetorigin;

//++++++++++++++++++++++++++++++++//
#include <float.h>

#define EARTH_RADIUS 6371000.0

void GPS2RefCoordSystem(double lat0, double long0, double latitude, double longitude, double* pX, double* pY)
{
	*pX = (M_PI/180.0)*EARTH_RADIUS*(longitude-long0)*cos((M_PI/180.0)*latitude);
	*pY = (M_PI/180.0)*EARTH_RADIUS*(latitude-lat0);
}

void RefCoordSystem2GPS(double lat0, double long0, double x, double y, double* pLatitude, double* pLongitude)
{
    *pLatitude = (y/(double)EARTH_RADIUS)*(180.0/M_PI)+lat0;
    if ((fabs(*pLatitude-90.0) < DBL_EPSILON)||(fabs(*pLatitude+90.0) < DBL_EPSILON))
    {
        *pLongitude = 0;
    }
    else
    {
        *pLongitude = (x/(double)EARTH_RADIUS)*(180.0/M_PI)/cos((M_PI/180.0)*(*pLatitude))+long0;
    }
}

double fmod_2PI(double theta)
{
	return fmod(fmod(theta, 2*M_PI)+3*M_PI, 2*M_PI)-M_PI;
}

double fmod_360_pos_rad2deg(double theta)
{
	return fmod(fmod(theta*180.0/M_PI, 2*180.0)+2*180.0, 2*180.0);
}

double fmod_360_rad2deg(double theta)
{
	return fmod(fmod(theta*180.0/M_PI, 2*180.0)+3*180.0, 2*180.0)-180.0;
}

double calculateSignedDistanceToLine2(const double nextLon, const double nextLat, const double prevLon, const double prevLat,
    const double gpsLon, const double gpsLat)
{
    double sinalpha = 0, wxa = 0, wya = 0, wxb = 0, wyb = 0, x = 0, y = 0;

    GPS2RefCoordSystem(prevLat, prevLon, prevLat, prevLon, &wxa, &wya); // GPS coordinates of a.
    GPS2RefCoordSystem(prevLat, prevLon, nextLat, nextLon, &wxb, &wyb); // GPS coordinates of b.
    GPS2RefCoordSystem(prevLat, prevLon, gpsLat, gpsLon, &x, &y); // GPS coordinates of m.

    double norm_ba = sqrt(pow(wxb-wxa, 2)+pow(wyb-wya, 2)); // Length of the line (norm of b-a).
    //double phi = atan2(wyb-wya, wxb-wxa); // Angle of the line.

    double norm_ma = sqrt(pow(x-wxa, 2)+pow(y-wya, 2)); // Distance from the beginning of the line (norm of m-a).
    //double norm_bm = sqrt(pow(wxb-x, 2)+pow(wyb-y, 2)); // Distance to the destination waypoint of the line (norm of b-m).

    if ((norm_ma != 0)&&(norm_ba != 0))
        sinalpha = ((wxb-wxa)*(y-wya)-(wyb-wya)*(x-wxa))/(norm_ma*norm_ba);
    else
        sinalpha = 0;

    double e = norm_ma*sinalpha; // Distance to the line (signed).

    return e;
}

double calculateSignedDistanceToLine2ref(double wxa, double wya, double wxb, double wyb, double x, double y)
{
    double sinalpha = 0;

    double norm_ba = sqrt(pow(wxb-wxa, 2)+pow(wyb-wya, 2)); // Length of the line (norm of b-a).
    //double phi = atan2(wyb-wya, wxb-wxa); // Angle of the line.

    double norm_ma = sqrt(pow(x-wxa, 2)+pow(y-wya, 2)); // Distance from the beginning of the line (norm of m-a).
    //double norm_bm = sqrt(pow(wxb-x, 2)+pow(wyb-y, 2)); // Distance to the destination waypoint of the line (norm of b-m).

    if ((norm_ma != 0)&&(norm_ba != 0))
            sinalpha = ((wxb-wxa)*(y-wya)-(wyb-wya)*(x-wxa))/(norm_ma*norm_ba);
    else
            sinalpha = 0;

    double e = norm_ma*sinalpha; // Distance to the line (signed).

    return e;
}

widgetVaimos::widgetVaimos(QWidget *parent) :
    QWidget(parent)
{
}

void widgetVaimos::clear()
{   
	listRect.clear();//???
    listPoly.clear();
    update();
}

double widgetVaimos::xToPix(double x)
{   
	double echx = width()/(xmax-xmin);
    return (x-xmin)*echx;
}

double widgetVaimos::yToPix(double y)
{
    double echy = height()/(ymax-ymin);
    return height()-(y-ymin)*echy;
}

void widgetVaimos::addPolygon(vector<double>& X,vector<double>& Y,double a,double b,double theta,QColor col)
{   
	if (X.size()==0) return;
    coloredPolygon P;
    P.color=col;
    for (unsigned int i=0;i<X.size();i++)
    {   double x,y;
        x=a+X[i]*cos(theta)-Y[i]*sin(theta);
        y=b+X[i]*sin(theta)+Y[i]*cos(theta);
        P.poly << QPoint(xToPix(x), yToPix(y));
    }
    listPoly.push_front(P);
    update();
}

void widgetVaimos::LoadFile()
{
    QFile file("data.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {   qDebug() << "File data.txt not found ";
        return;
    }
    qDebug() << "File open ";
    QTextStream in(&file);
    QString line1 = in.readLine();
    QStringList mots;

    clear();

    kmax=0; ks0=0; ks1=0;

    Mode.clear(); K.clear(); 
	T.clear(); Theta.clear(); Lat.clear(); Long.clear(); X.clear(); Y.clear(); Deltav_R0.clear();
    Winddir.clear(); Windspeed.clear(); Deltag.clear(); Deltavmax.clear(); Ax.clear(); Ay.clear(); Bx.clear(); By.clear();    
    Ecart.clear(); Norm_am.clear(); Norm_bm.clear(); Roll.clear(); Pitch.clear(); Yaw.clear(); Dir0.clear(); Vit.clear(); Longueur.clear();
    Theta_gps.clear(); Speed_gps.clear(); Distance_gps.clear(); Filteredtheta_gps.clear(); Filteredspeed_gps.clear(); Dt.clear();
	Lat0.clear(); Long0.clear();

    while (line1 != NULL)
    {
		mots = line1.split(";"); kmax++;
        int k=mots[0].toInt();                          // [0] numero de ligne
        K.push_back(k);
        double t=mots[1].toDouble();                    // [1] temps en seconde
        T.push_back(t);
		double lat0=mots[2].toDouble();                 // [2] lat0 (en degres): latitude de début de mission (convention axe x vers l'est, axe y vers le nord et axe z vers le haut) dans lequel sont exprimés x, y, theta, psi (qui correspondent à ceux utilisés dans l'article)
		Lat0.push_back(lat0);
		double long0=mots[3].toDouble();                // [3] long0 (en degres) : longitude de début de mission
		Long0.push_back(long0);
		double roll=mots[4].toDouble();                 // [4] roll (in rad) : angle de roulis du bateau calculé par la MTi-G
        Roll.push_back(roll);
        double pitch=mots[5].toDouble();                // [5] pitch (in rad) : angle de tangage du bateau calculé par la MTi-G
        Pitch.push_back(pitch);
        double yaw=mots[6].toDouble();                  // [6] yaw (in rad) : angle de cap du bateau calculé par la MTi-G exprimé dans le repère NWU (North-West-Up, 0: Nord, -1.57: Est, 3.14: Sud, 1.57: Ouest)
        Yaw.push_back(yaw);
		double winddir=0;
		double windspeed=0.0;
		if (filteredwinddirmode)
		{
			winddir=3*M_PI/2-mots[9].toDouble();       // [7] winddir=%pi/2-(m(:,8)-%pi); winddir (in rad) : angle d'où vient le vent réel par rapport au Nord calculé par la station météo (0: Nord, 1.57: Est, 3.14: Sud, 4.71: Ouest)
			Winddir.push_back(winddir);
			windspeed=10.0;//mots[10].toDouble();           // [8] windspeed (in m/s) : vitesse du vent réel donnée par la station météo
			Windspeed.push_back(windspeed);
			// [9] filteredwinddir (in rad) : winddir filtré
			// [10] filteredwindspeed (in m/s) : windspeed filtré
		}
		else
		{
			winddir=3*M_PI/2-mots[7].toDouble();       // [7] winddir=%pi/2-(m(:,8)-%pi); winddir (in rad) : angle d'où vient le vent réel par rapport au Nord calculé par la station météo (0: Nord, 1.57: Est, 3.14: Sud, 4.71: Ouest)
			Winddir.push_back(winddir);
			windspeed=mots[8].toDouble();           // [8] windspeed (in m/s) : vitesse du vent réel donnée par la station météo
			Windspeed.push_back(windspeed);
			// [9] filteredwinddir (in rad) : winddir filtré
			// [10] filteredwindspeed (in m/s) : windspeed filtré
		}
        double deltav_R0=3*M_PI/2-mots[11].toDouble();    // [11] heading (in rad) : angle de la voile par rapport au Nord calculé par la station météo (0: Nord, 1.57: Est, 3.14: Sud, 4.71: Ouest)
        Deltav_R0.push_back(deltav_R0);
        double theta=mots[12].toDouble();               // [12] theta (in rad) : angle de cap du bateau exprimé dans le repère de référence (voir article)
        Theta.push_back(theta);
                                                            // [13] psi (in rad) : angle vers où va le vent exprimé dans le repère de référence (voir article)
        double lat=mots[14].toDouble();                  // [14] latitude (in decimal degrees) : latitude actuelle du robot donnée par la station météo
        Lat.push_back(lat);
        double longi=mots[15].toDouble();                 // [15] longitude (in decimal degrees) : longitude actuelle du robot donnée par la station météo
        Long.push_back(longi);
        //++++++++++++++++++++++++++++++++//
        double x=mots[16].toDouble();                   // [16] x (in m) : position x du robot exprimée dans le repère de référence (voir article) et calculée à partir de latitude et longitude
        double y=mots[17].toDouble();                   // [17] y (in m) : position y du robot exprimée dans le repère de référence (voir article) et calculée à partir de latitude et longitude

        double x0=0;double y0=0;
        if (resetorigin) GPS2RefCoordSystem(48.39, -4.425, lat0, long0, &x0, &y0);
        X.push_back(x+x0);
        Y.push_back(y+y0);

        double ax=mots[18].toDouble();                  // [18] ax (in m) : position x du point A (waypoint définissant le début de la ligne à suivre) exprimée dans le repère de référence
        Ax.push_back(ax+x0);
        double ay=mots[19].toDouble();                  //[19] ay (in m) : position y du  point A (waypoint définissant le début de la ligne à suivre) exprimée dans le repère de référence
        Ay.push_back(ay+y0);
        double bx=mots[20].toDouble();                  //[20] bx (in m) : position x du point B (waypoint définissant la fin de la ligne à suivre) exprimée dans le repère de référence
        Bx.push_back(bx+x0);
        double by=mots[21].toDouble();                  //[21] by (in m) : position y du point B (waypoint définissant la fin de la ligne à suivre) exprimée dans le repère de référence
        By.push_back(by+y0);
        //--------------------------------//
                                                            // [22] CurWP : numéro du waypoint B (waypoint définissant la fin de la ligne à suivre)
                                                            // [23] wpslat[CurWP] (in decimal degrees) : latitude de ce waypoint
                                                            // [24] wpslong[CurWP] (in decimal degrees) : longitude de ce waypoint
        double ecart=mots[25].toDouble(); //[25] e (in m) : écart entre le robot et la ligne à suivre (voir article)
        Ecart.push_back(ecart);
        double norm_am=mots[26].toDouble(); //[26] norm_ma (in m) : distance entre la position actuelle du robot et le point A
        Norm_am.push_back(norm_am);
        double norm_bm=mots[27].toDouble(); //[27] norm_bm (in m) : distance entre le point B et la position actuelle du robot
        Norm_bm.push_back(norm_bm);
        int mode=mots[28].toInt(); //[28] state : 0: suivi direct de ligne, 1: remontée au vent avec vent venant de tribord, 2: remontée au vent avec vent venant de babord
        Mode.push_back(mode);
        double deltag=mots[29].toDouble(); //[29] deltag (in rad) : angle du gouvernail (voir article)
        Deltag.push_back(deltag);
        double deltavmax=mots[30].toDouble();  //[30] deltavmax (in rad) : angle maximal de la voile (voir article)
        Deltavmax.push_back(deltavmax);
        double dir0=mots[31].toDouble(); //[31] direction voulue
        Dir0.push_back(dir0);
        line1 = in.readLine();
   }
	ks0=0;
	ks1=kmax;
	double dt=0;
    double vit=0;
    double longueur=0;
	Dt.push_back(dt);
	Vit.push_back(vit);
	Longueur.push_back(longueur);
	double theta_gps=Theta[0];
	double speed_gps=0;
	double distance_gps=0;
	double filteredtheta_gps=Theta[0];
	double filteredspeed_gps=0;
	Theta_gps.push_back(theta_gps);
	Speed_gps.push_back(speed_gps);
	Distance_gps.push_back(distance_gps);
	Filteredtheta_gps.push_back(filteredtheta_gps);
	Filteredspeed_gps.push_back(filteredspeed_gps);

    for (int k=1;k<kmax;k++)
    {
        dt=T[k]-T[k-1];
		Dt.push_back(dt);
        vit=0.95*vit+(0.05/dt)*sqrt(pow(X[k]-X[k-1],2)+pow(Y[k]-Y[k-1],2));
        Vit.push_back(vit);
        longueur=longueur+vit*dt;
        //longueur=longueur+sqrt(pow(X[k]-X[k-1],2)+pow(Y[k]-Y[k-1],2));
        Longueur.push_back(longueur);

		theta_gps=atan2(Y[k]-Y[k-1],X[k]-X[k-1]);
        Theta_gps.push_back(theta_gps);
		speed_gps=sqrt(pow(X[k]-X[k-1],2)+pow(Y[k]-Y[k-1],2))/Dt[k];
        Speed_gps.push_back(speed_gps);
		distance_gps=Distance_gps[k-1]+sqrt(pow(X[k]-X[k-1],2)+pow(Y[k]-Y[k-1],2));
        Distance_gps.push_back(distance_gps);
		// Filter with cos and sin...
		if (Theta_gps[k]==0) filteredtheta_gps=Filteredtheta_gps[k-1];
		else filteredtheta_gps=Theta_gps[k];
        Filteredtheta_gps.push_back(filteredtheta_gps);
		//if (Speed_gps[k]==0) filteredspeed_gps=Filteredspeed_gps[k-1];
		//else filteredspeed_gps=0.9*Filteredspeed_gps[k-1]+0.1*Speed_gps[k];
		filteredspeed_gps=0.9*Filteredspeed_gps[k-1]+0.1*Speed_gps[k];
		//else filteredspeed_gps=Speed_gps[k];
        Filteredspeed_gps.push_back(filteredspeed_gps);
    }
}

void widgetVaimos::Draw()
{   clear();
    xmin=X[k0]-Echelle;
    xmax=X[k0]+Echelle;
    ymin=Y[k0]-Echelle;
    ymax=Y[k0]+Echelle;

    vector<double> Xl,Yl;  // ligne courante
	if (thickline)
	{ 
		double phi = atan2(By[k0]-Ay[k0], Bx[k0]-Ax[k0]); // Angle of the line.
		Xl.push_back(Ax[k0]+0.5*sin(phi));  Yl.push_back(Ay[k0]-0.5*cos(phi));
		Xl.push_back(Bx[k0]);  Yl.push_back(By[k0]);
		Xl.push_back(Ax[k0]-0.5*sin(phi));  Yl.push_back(Ay[k0]+0.5*cos(phi)); 
	}
	else
	{ 
		Xl.push_back(Ax[k0]);  Yl.push_back(Ay[k0]);
		Xl.push_back(Bx[k0]);  Yl.push_back(By[k0]);
	}
    addPolygon(Xl,Yl,0,0,0,QColor("Green"));


    for (int k=ks0; k<ks1; k=k+step)
        if ((X[k]<xmax)&&(X[k]>xmin)&&(Y[k]<ymax)&&(Y[k]>ymin))
            {   vector<double> Xw,Yw;                           // vent
                double h=0.1*Windspeed[k];
                Xw.push_back(0);        Yw.push_back(0);
                Xw.push_back(h);        Yw.push_back(0);
                Xw.push_back(h-0.2);    Yw.push_back(0.2);
                Xw.push_back(h);        Yw.push_back(0);
                Xw.push_back(h-0.2);    Yw.push_back(-0.2);
                Xw.push_back(h);        Yw.push_back(0);
                if (wind) addPolygon(Xw,Yw,X[k]+2,Y[k],Winddir[k],QColor("Black"));

                if (go)
                {
                        vector<double> Xg,Yg;                            // Direction suivie en mode direct.
                    if (Mode[k]==0)
                    {
                        double h=1;
                        Xg.push_back(0);        Yg.push_back(0);
                        Xg.push_back(h);        Yg.push_back(0);
                        Xg.push_back(h-0.2);    Yg.push_back(0.2);
                        Xg.push_back(h);        Yg.push_back(0);
                        Xg.push_back(h-0.2);    Yg.push_back(-0.2);
                        Xg.push_back(h);        Yg.push_back(0);
                        addPolygon(Xg,Yg,X[k]+2,Y[k],Dir0[k],QColor("Green"));
                     }
                }
                vector<double> Xl;                                 // autres lignes
                vector<double> Yl;
				if (k != k0)
				{
					Xl.push_back(Ax[k]);  Yl.push_back(Ay[k]);
					Xl.push_back(Bx[k]);  Yl.push_back(By[k]);
					addPolygon(Xl,Yl,0,0,0,QColor("Red"));
				}

                double a=0.5;
                vector<double> Xr;  //gouvernail
                vector<double> Yr;
                Xr.push_back(-a);  Yr.push_back(0);
                Xr.push_back(0);   Yr.push_back(0);
                addPolygon(Xr,Yr,X[k]-a*cos(Theta[k]),Y[k]-a*sin(Theta[k]),Theta[k]+Deltag[k],QColor("Purple"));

                vector<double> Xv;  //voile
                vector<double> Yv;
                Xv.push_back(-3*a);  Yv.push_back(0);
                Xv.push_back(0);     Yv.push_back(0);
                double thetamoinspsi=Theta[k]-Winddir[k];
                double deltav;
                if (sin(thetamoinspsi)>0)  deltav=Deltavmax[k];
                    else deltav=-Deltavmax[k];

                //qDebug()<<"deltav="<<deltav;
                if (sail1)  addPolygon(Xv,Yv,X[k]+3*a*cos(Theta[k]),Y[k]+3*a*sin(Theta[k]),Deltav_R0[k]+M_PI,QColor("Purple"));
                if (sail2)  addPolygon(Xv,Yv,X[k]+3*a*cos(Theta[k]),Y[k]+3*a*sin(Theta[k]),Theta[k]+deltav,QColor("Magenta"));


                vector<double> Xc,Yc;                //Robot
                Xc.push_back(-a);  Yc.push_back(-a);
                Xc.push_back(4*a); Yc.push_back(0);
                Xc.push_back(-a);  Yc.push_back(a);
                Xc.push_back(-a);  Yc.push_back(-a);
                QColor col("Green");
                if (fabs(Roll[k]*180/M_PI)>35) col=QColor("Red");
                else  if (Mode[k]!=0) col=QColor("Yellow");
                addPolygon(Xc,Yc,X[k],Y[k],Theta[k],col);

            }
    //qDebug() << "k0="<<k0<<"x="<<X[k0]<<"Dir0="<<Dir0[k0]<<'\n';
    repaint();
}

void widgetVaimos::paintEvent(QPaintEvent *)
{  
	QPainter painter(this);
    painter.setPen(QPen(QColor("Red")));
    painter.drawRect(QRectF(0, 0, width()-1, height()-1));
    QList<coloredRect>::iterator i;
    for (i=this->listRect.begin(); i != this->listRect.end(); i=i+1)
    {   painter.fillRect(i->rect,i->color );
        painter.drawRect(i->rect);
    }
    QList<coloredPolygon>::iterator j;
    for(j=this->listPoly.begin(); j != this->listPoly.end(); j=j+1)
    {   painter.setPen(QPen(j->color));
        painter.setBrush(j->color );
        painter.drawPolygon(j->poly);
    }
    painter.setPen(QPen(QColor("Black")));
    painter.setBrush(Qt::Dense7Pattern);
    painter.drawEllipse(QPoint(0.5*width(),0.5*height()),20,20);
    painter.setPen(QPen(QColor("Black")));
    int a=0;
    if (text)
    {
        painter.drawText(QRectF(3, a*20, 400,20),
                         QString("k=%1, ks0=%2, ks1=%3 ").
						 arg(K[k0]).arg(ks0).arg(ks1)); a++;
        painter.drawText(QRectF(3, a*20, 400,20),
                         QString("t=%1 s, x=%2, y=%3, theta=%4 deg ").
						 arg(T[k0]).arg(X[k0]).arg(Y[k0]).arg(fmod_360_pos_rad2deg(M_PI/2.0-Theta[k0]))); a++;
        painter.drawText(QRectF(3, a*20, 400,20),
                         QString("SOG=%1 m.sec^-1 = %2 knots, COG=%3 deg ").
                         arg(Filteredspeed_gps[k0]).arg(Filteredspeed_gps[k0]*1.94).arg(fmod_360_pos_rad2deg(M_PI/2.0-Filteredtheta_gps[k0]))); a++;
		if (!motorboatmode)
		{
			painter.drawText(QRectF(3, a*20, 400,20),
				QString("length=%1 m, sail angle =%2 deg, rudder angle=%3 deg ").
				arg(Distance_gps[k0]).arg(fmod_360_pos_rad2deg(Deltav_R0[k0]+M_PI-Theta[k0])).arg(Deltag[k0])); a++;
			painter.drawText(QRectF(3, a*20, 400,20),
				QString("wind : dir=%1 deg =%2 deg, speed=%3 m.s^-1 =%4 knots ").
				arg(Winddir[k0]*180/M_PI).arg(fmod_360_pos_rad2deg(-(Winddir[k0]-3.0*M_PI/2.0))).arg(Windspeed[k0]).arg(1.94*Windspeed[k0])); a++;
		}
		else
		{
			painter.drawText(QRectF(3, a*20, 400,20),
				QString("length=%1 m, rudder angle=%2 deg").
				arg(Distance_gps[k0]).arg(Deltag[k0])); a++;
		}
        painter.drawText(QRectF(3, a*20, 400,20),
                         QString("dist to the line = %1, ||am||= %2, ||bm||= %3 ").
                         arg(Ecart[k0]).arg(Norm_am[k0]).arg(Norm_bm[k0])); a++;
		if (recalce)
		{
			double Longa = 0, Lata = 0, Longb = 0, Latb = 0, Longi = 0, Lati = 0;
			RefCoordSystem2GPS(Lat0[k0], Long0[k0], Ax[k0], Ay[k0], &Lata, &Longa);
			RefCoordSystem2GPS(Lat0[k0], Long0[k0], Bx[k0], By[k0], &Latb, &Longb);
			RefCoordSystem2GPS(Lat0[k0], Long0[k0], X[k0], Y[k0], &Lati, &Longi);
			painter.drawText(QRectF(3, a*20, 400,20),
							 QString("dist to the line (recalculated) = %1 ").
							 arg(calculateSignedDistanceToLine2ref(Ax[k0], Ay[k0], Bx[k0], By[k0], X[k0], Y[k0]))); a++;
			//painter.drawText(QRectF(3, a*20, 400,20),
			//				 QString("dist to the line (recalculated from LLA) = %1 ").
			//				 arg(calculateSignedDistanceToLine2(Longb, Latb, Longa, Lata, Long[k0], Lat[k0]))); a++;
		}
        painter.drawText(QRectF(3, a*20, 400,20),
                         QString("AHRS (deg) : Roll %1, Pitch = %2, Yaw = %3 ").
                         arg(Roll[k0]*180/M_PI).arg(Pitch[k0]*180/M_PI).arg(Yaw[k0]*180/M_PI)); a++;
    }
    painter.end();
}
