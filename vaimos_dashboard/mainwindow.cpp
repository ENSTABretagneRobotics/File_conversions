#include "mainwindow.h"
#include "ui_mainwindow.h"

int k0;
int kmax;
int Echelle;
int step;
bool wind,sail1,sail2,go,text;

MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent),
ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->pushButtonLoadFile,SIGNAL(clicked()),this,SLOT(LoadFile()));
	connect(ui->k0_horizontalScrollBar,SIGNAL(valueChanged(int)),this,SLOT(Init()));
	connect(ui->Echelle_verticalScrollBar,SIGNAL(valueChanged(int)),this,SLOT(Init()));
	connect(ui->Step_verticalScrollBar,SIGNAL(valueChanged(int)),this,SLOT(Init()));
	connect(ui->Sail1_checkBox,SIGNAL(stateChanged(int)),this,SLOT(Init()));
	connect(ui->Sail2_checkBox,SIGNAL(stateChanged(int)),this,SLOT(Init()));
	connect(ui->Wind_checkBox,SIGNAL(stateChanged(int)),this,SLOT(Init()));
	connect(ui->Text_checkBox,SIGNAL(stateChanged(int)),this,SLOT(Init()));
	connect(ui->Go_checkBox,SIGNAL(stateChanged(int)),this,SLOT(Init()));

	LoadFile();
}

void MainWindow::LoadFile()
{  
	ui->widgetVaimos1->LoadFile();
	ui->k0_horizontalScrollBar->setMaximum(kmax-1);
	ui->k0_horizontalScrollBar->setValue(kmax-10);
	Init();
}

void MainWindow::Init()
{ 
	Echelle=2+exp(0.1*ui->Echelle_verticalScrollBar->value());
	k0=ui->k0_horizontalScrollBar->value();
	step=ui->Step_verticalScrollBar->value();
	wind=ui->Wind_checkBox->isChecked();
	sail1=ui->Sail1_checkBox->isChecked();
	sail2=ui->Sail2_checkBox->isChecked();
	go=ui->Go_checkBox->isChecked();
	text=ui->Text_checkBox->isChecked();
	ui->widgetVaimos1->Draw();
	ui->widgetVaimos1->repaint();
}

MainWindow::~MainWindow()
{
	delete ui;
}
