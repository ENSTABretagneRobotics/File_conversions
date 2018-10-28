#include "mainwindow.h"
#include "ui_mainwindow.h"

int k0=0;
int kmax=0;
int ks0=0;
int ks1=0;
int Echelle=0;
int step=0;
bool wind=false,sail1=false,sail2=false,go=false,thickline=false,text=false,filteredwinddirmode=false,motorboatmode=false,recalce=false,resetorigin=false,switchavailable=false;

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
	connect(ui->ThickLine_checkBox,SIGNAL(stateChanged(int)),this,SLOT(Init()));
	connect(ui->FilteredWindDirMode_checkBox,SIGNAL(stateChanged(int)),this,SLOT(LoadFile()));
	connect(ui->MotorboatMode_checkBox,SIGNAL(stateChanged(int)),this,SLOT(Init()));
	connect(ui->Recalce_checkBox,SIGNAL(stateChanged(int)),this,SLOT(Init()));
	connect(ui->ResetOrigin_checkBox,SIGNAL(stateChanged(int)),this,SLOT(LoadFile()));
	connect(ui->SwitchAvailable_checkBox,SIGNAL(stateChanged(int)),this,SLOT(LoadFile()));
	connect(ui->ks0_verticalScrollBar,SIGNAL(valueChanged(int)),this,SLOT(Init()));
	connect(ui->ks1_verticalScrollBar,SIGNAL(valueChanged(int)),this,SLOT(Init()));

	LoadFile();
}

void MainWindow::LoadFile()
{  
	wind=ui->Wind_checkBox->isChecked();
	sail1=ui->Sail1_checkBox->isChecked();
	sail2=ui->Sail2_checkBox->isChecked();
	go=ui->Go_checkBox->isChecked();
	thickline=ui->ThickLine_checkBox->isChecked();
	text=ui->Text_checkBox->isChecked();
	filteredwinddirmode=ui->FilteredWindDirMode_checkBox->isChecked();
	motorboatmode=ui->MotorboatMode_checkBox->isChecked();
	recalce=ui->Recalce_checkBox->isChecked();
	resetorigin=ui->ResetOrigin_checkBox->isChecked();
	switchavailable=ui->SwitchAvailable_checkBox->isChecked();
	ui->widgetVaimos1->LoadFile();
	ui->k0_horizontalScrollBar->setMaximum(kmax-1);
	ui->k0_horizontalScrollBar->setValue(kmax-10);
	ui->ks0_verticalScrollBar->setMaximum(kmax-1);
	ui->ks0_verticalScrollBar->setValue(0);
	ui->ks1_verticalScrollBar->setMaximum(kmax-1);
	ui->ks1_verticalScrollBar->setValue(kmax-1);
	Init();
}

void MainWindow::Init()
{ 
	Echelle=2+exp(0.1*ui->Echelle_verticalScrollBar->value());
	k0=ui->k0_horizontalScrollBar->value();
	ks0=ui->ks0_verticalScrollBar->value();
	ks1=ui->ks1_verticalScrollBar->value();
	step=ui->Step_verticalScrollBar->value();
	wind=ui->Wind_checkBox->isChecked();
	sail1=ui->Sail1_checkBox->isChecked();
	sail2=ui->Sail2_checkBox->isChecked();
	go=ui->Go_checkBox->isChecked();
	thickline=ui->ThickLine_checkBox->isChecked();
	text=ui->Text_checkBox->isChecked();
	filteredwinddirmode=ui->FilteredWindDirMode_checkBox->isChecked();
	motorboatmode=ui->MotorboatMode_checkBox->isChecked();
	recalce=ui->Recalce_checkBox->isChecked();
	resetorigin=ui->ResetOrigin_checkBox->isChecked();
	switchavailable=ui->SwitchAvailable_checkBox->isChecked();
	ui->widgetVaimos1->Draw();
	ui->widgetVaimos1->repaint();
}

MainWindow::~MainWindow()
{
	delete ui;
}
