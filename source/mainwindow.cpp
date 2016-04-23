#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QShortcut>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    addActions();
    addShortcuts();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addActions() {
  connect( ui->bRun, SIGNAL(clicked()), this, SLOT(bRunClick()) );
  connect( ui->bBrowseIn, SIGNAL(clicked()), this, SLOT(bBrowseInClick()) );
  connect( ui->bBrowseOut, SIGNAL(clicked()), this, SLOT(bBrowseOutClick()) );
}

void MainWindow::addShortcuts() {
  QShortcut *bRun=new QShortcut(QKeySequence("F5"), this);
  connect( bRun, SIGNAL(activated()), ui->bRun, SLOT(click()) );

  QShortcut *bBrowseIn=new QShortcut(QKeySequence("F3"), this);
  connect( bBrowseIn, SIGNAL(activated()), ui->bBrowseIn, SLOT(click()) );

  QShortcut *bBrowseOut=new QShortcut(QKeySequence("F2"), this);
  connect( bBrowseOut, SIGNAL(activated()), ui->bBrowseOut, SLOT(click()) );

  QShortcut *quit=new QShortcut(QKeySequence("Esc"), this);
  connect( quit, SIGNAL(activated()), this, SLOT(close()) );
}


// ---------------------------------------------- button handlers ----------------------------------------------

void MainWindow::bRunClick(){
  status("");
  
  // string filesRoot = "files/";
  // int linesCount = ui->leLinesCount->text().toInt();
  // string fileName = filesRoot + ui->lePath->text().toStdString();
  // string outFilename = filesRoot + ui->leOutFilename->text().toStdString();

  int linesCount = ui->leLinesCount->text().toInt();
  string fileName = ui->lePath->text().toStdString();
  string outFilename = ui->leOutFilename->text().toStdString();
  
  QString outDir = ui->leOutDir->text();
  if(outDir.trimmed().length() == 0){
    QString inDir = ui->lePath->text();
    
    QFileInfo info(inDir);
    outDir = info.absolutePath();
  }
  
  outDir = formatPath(outDir);
  outDir.replace(QRegExp("/$"), "");
  outDir += "/";
  outFilename = outDir.toStdString() + outFilename;
  

  fstream textFile(fileName.c_str(), ios::in);
  if(!textFile.is_open()){
    cout << "cannot open file\n";
    return;
  }


  bool finish = false;
  string str;
  int count = 1;
  int filesLimit = 10;

  while(textFile){
    // if(count > filesLimit) break;

    string outFileC = outFilename + "_" + to_string(count++) + ".txt";
    fstream outFile(outFileC.c_str(), ios::out);
    if(!outFile.is_open()){
      cout << "cannot open file\n";
      return;
    }

    for(int i=0; i<linesCount; ++i){
      getline(textFile, str);
      
      if(!textFile){
        if(i == 0){
          outFile.close();
          remove(outFileC.c_str());
        }
        
        finish = true;
        break;
      }
      
      if(i == linesCount-1){
        outFile << str;
      }
      else{
        outFile << str + "\n";
      }
      // outFile.write((char*) &refval, size);
    }
    
    if(finish) break;
    
    outFile.close();
  }

  textFile.close();

  qDebug() << "Finish";
  status("Splitting Finished");
}


void MainWindow::bBrowseInClick(){
  QString dir=path();
  if(dir.length()==0) dir=QDir::currentPath();

  QString filename = QFileDialog::getOpenFileName(this, tr("Select a text file"), dir);
  if( !filename.isNull() )
  {
    setPath(filename);
  }
}

void MainWindow::bBrowseOutClick(){
  QString dir=path();
  if(dir.length()==0) dir=QDir::currentPath();

  QString dirname = QFileDialog::getExistingDirectory(this, tr("Select a Directory"), dir);
  if( !dirname.isNull() )
  {
    setOutPath(dirname);
  }
}


// --------------------------------------------- set/get ---------------------------------------------

QString MainWindow::path(){
  return ui->lePath->text();
}

void MainWindow::setPath(QString path){
  ui->lePath->setText(path);
}

void MainWindow::setOutPath(QString path){
  ui->leOutDir->setText(path);
}


// --------------------------------------------- service ---------------------------------------------

void MainWindow::status(QString msg) {
  if(msg == ""){
    ui->statusBar->clearMessage();
    return;
  }
  ui->statusBar->showMessage("[" + msg + "]");
}

QString MainWindow::formatPath(QString path){
    QString res = "";
    QFileInfo info(path);
    res = info.absoluteFilePath();
    res.replace(QRegExp("/$"), "");
    return res;
}
