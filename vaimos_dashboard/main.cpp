#include <QtGlobal>
#if (QT_VERSION >= 0x050000)
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif // (QT_VERSION >= 0x050000)
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
