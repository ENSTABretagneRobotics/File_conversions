#ifndef WIDGETVaimos_H
#define WIDGETVaimos_H

#include <cmath>
using namespace std;
#include <QWidget>
#include <list>
#include <vector>
#include <QPainter>

struct coloredRect {
	QRect rect;
	QColor color;
};

struct coloredPolygon {
	QPolygon poly;
	QColor color;
};

class widgetVaimos : public QWidget
{
	Q_OBJECT
public:
	vector<int> Mode,K;
	vector<double> T,Theta,Lat,Long,X,Y,Deltav_R0;
	vector<double> Winddir,Windspeed,Filteredwinddir,Filteredwindspeed,Deltag,Deltavmax,Ax,Ay,Bx,By;
	vector<double> Ecart,Norm_am,Norm_bm,Roll,Pitch,Yaw,Dir0,Vit,Longueur;
	vector<double> SwitchAvailable0;
	vector<double> Theta_gps,Speed_gps,Distance_gps,Filteredtheta_gps,Filteredspeed_gps,Dt;
	vector<double> Lat0, Long0;

	double xmin,xmax,ymin,ymax;

	QList<coloredRect> listRect;
	QList<coloredPolygon> listPoly;

	double xToPix(double );
	double yToPix(double );
	void paintEvent(QPaintEvent *);
	void addPolygon(vector<double>& X,vector<double>& Y,double a,double b,double theta,QColor);
	void clear();

	explicit widgetVaimos(QWidget *parent = 0);

signals:

	public slots:
		void LoadFile();
		void Draw();
};

#endif // WIDGETVaimos_H
