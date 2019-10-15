#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    m_procIndex(0)
{
    ui->setupUi(this);
    m_cachedFileList.clear();
    m_faceInfoList.clear();
    m_editRootPath = new QLineEdit(this);
    m_editRootPath->setGeometry(0,0,WINDOW_WIDTH,40);

    m_btnInit = new QPushButton(this);
    m_btnInit->setGeometry(WINDOW_WIDTH/2-200,100,150,40);
    m_btnInit->setText("初始化...");
    connect(m_btnInit,SIGNAL(clicked()),this,SLOT(handleInitButton()));

    m_label = new QLabel(this);
    m_label->setGeometry((WINDOW_WIDTH-PREVIEW_WIDTH)/2,WINDOW_HEIGHT-PREVIEW_HEIGHT,PREVIEW_WIDTH,PREVIEW_HEIGHT);
//    QImage image;
//    image.load("/Users/UxinTest/Desktop/MM_000130_IEU+00_PM+00_EN_A0_D0_T0_BB_M0_R1_S0.tif");
//    m_label->clear();
//    QPixmap pic = QPixmap::fromImage(image);
//    m_label->setPixmap(pic.scaled(QSize(PREVIEW_WIDTH,PREVIEW_HEIGHT),Qt::KeepAspectRatio));

    m_btnPrev = new QPushButton(this);
    m_btnPrev->setGeometry(WINDOW_WIDTH/2-200,0,150,40);
    m_btnPrev->setText("上一张");
    connect(m_btnPrev,SIGNAL(clicked()),this,SLOT(handlePrevButton()));

    m_btnNext = new QPushButton(this);
    m_btnNext->setGeometry(WINDOW_WIDTH/2+50,0,150,40);
    m_btnNext->setText("下一张");
    connect(m_btnNext,SIGNAL(clicked()),this,SLOT(handleNextButton()));

    m_cbFaceType = new QComboBox(this);
    m_cbFaceType->setGeometry(10,50,150,30);
    m_strFaceTypeList<<"椭圆脸"<<"圆形脸"<<"方形脸"<<"锥子脸"<<"梨形脸";
    m_cbFaceType->addItems(m_strFaceTypeList);

    m_cbEyelidType = new QComboBox(this);
    m_cbEyelidType->setGeometry(10,90,150,30);
    m_strEyelidTypeList<<"杏眼"<<"丹凤眼"<<"上斜眼"<<"下斜眼"<<"细长眼"<<"三角眼";
    m_cbEyelidType->addItems(m_strEyelidTypeList);

    m_cbBrowType = new QComboBox(this);
    m_cbBrowType->setGeometry(10,130,150,30);
    m_strBrowTypeList<<"一号眉";
    m_cbBrowType->addItems(m_strBrowTypeList);

    m_cbMouthType = new QComboBox(this);
    m_cbMouthType->setGeometry(10,170,150,30);
    m_strMouthTypeList<<"一号嘴";
    m_cbMouthType->addItems(m_strMouthTypeList);

    m_label_curIndex = new QLabel(this);
    m_label_curIndex->setGeometry(WINDOW_WIDTH/2,50,150,40);
    m_label_curIndex->setText("第1张");

    m_btnConfirm = new QPushButton(this);
    m_btnConfirm->setGeometry(WINDOW_WIDTH/2-150/2,90,150,40);
    m_btnConfirm->setText("修改");
    connect(m_btnConfirm,SIGNAL(clicked()),this,SLOT(handleConfirmButton()));

    m_editJumpPage = new QLineEdit(this);
    m_editJumpPage->setGeometry(WINDOW_WIDTH/2-150/2,135,70,30);

    m_btnGo = new QPushButton(this);
    m_btnGo->setGeometry(WINDOW_WIDTH/2-150/2+80,135,70,30);
    m_btnGo->setText("跳转");
    connect(m_btnGo,SIGNAL(clicked()),this,SLOT(handleJumpPicButton()));

//    m_label_isTaged = new QLabel(this);
//    m_label_isTaged->setGeometry(WINDOW_WIDTH-150,50,200,30);
//    m_label_isTaged->setText("未标记");

    m_label_faecType = new QLabel(this);
    m_label_faecType->setGeometry(WINDOW_WIDTH-150,90,200,30);
    m_label_faecType->setText("未知类型");

    m_label_eyelidType = new QLabel(this);
    m_label_eyelidType->setGeometry(WINDOW_WIDTH-150,130,200,30);
    m_label_eyelidType->setText("未知类型");

    m_label_browType = new QLabel(this);
    m_label_browType->setGeometry(WINDOW_WIDTH-150,170,200,30);
    m_label_browType->setText("未知类型");

    m_label_mouthType = new QLabel(this);
    m_label_mouthType->setGeometry(WINDOW_WIDTH-150,210,200,30);
    m_label_mouthType->setText("未知类型");

    initViews();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::handleNextButton(){
    ++m_procIndex;
    if(m_procIndex>=m_filelist.size())
        --m_procIndex;
    showImage(m_procIndex);
    m_cbFaceType->setCurrentIndex(3);
}

void Widget::handlePrevButton(){
    --m_procIndex;
    if(m_procIndex<=0)
        m_procIndex = 0;
    showImage(m_procIndex);
}

void Widget::handleConfirmButton(){
     int faceTypeID = m_cbFaceType->currentIndex();
     int eyelidType = m_cbEyelidType->currentIndex();
//     m_faceInfoList[m_procIndex].browType = m_cbBrowType->currentIndex();
//     m_faceInfoList[m_procIndex].mouthType = m_cbMouthType->currentIndex();

     QString backedFilePath = m_cachedFileList.at(m_procIndex);
     QString fileName = getFileName(backedFilePath);
     QStringList components = fileName.split("_");

     QStringList faceTypeNameArr = {"Oval", "Round", "square","Sharp", "pear"};
     QStringList eyelidTypeNameArr = {"E1", "E2", "E3","E4", "E5","E6"};

     QString faceTypeStr = faceTypeNameArr.at(faceTypeID);
     QString eyelidTypeStr = eyelidTypeNameArr.at(eyelidType);

     QString indexStr = QString("%1").arg(m_procIndex, 3, 10, QChar('0'));
     QString retFileName = indexStr + "_"+faceTypeStr+"_"+eyelidTypeStr+"_.tif";
     qDebug()<<retFileName;

     m_currentDir.rename(fileName,retFileName);
     m_cachedFileList[m_procIndex] = m_rootPath +"/"+ retFileName;
     showImage(m_procIndex);
}

QString Widget::getFileName(QString fullpath){
    QStringList fileComponents = fullpath.split("/");
    QString fileName = fileComponents.at(fileComponents.size()-1);
    return fileName;
}

QFileInfoList Widget::getAllFiles(){
    QDir dir(m_rootPath);
     dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
     dir.setSorting(QDir::Size | QDir::Reversed);

     m_filelist = dir.entryInfoList();
     m_filelist.removeAt(0);

     for (auto item:m_filelist) {
         m_cachedFileList.append(item.filePath());
     }
     m_cachedFileList.sort();

     return m_filelist;
}

void Widget::showImage(int idx){
    m_label_curIndex->setText("第"+QString::number(idx+1)+"张");

    QString filePath = m_cachedFileList.at(idx);
    QImage image;
    image.load(filePath);
    m_label->clear();
    QPixmap pic = QPixmap::fromImage(image);
    m_label->setPixmap(pic.scaled(QSize(PREVIEW_WIDTH,PREVIEW_HEIGHT),Qt::KeepAspectRatio));

    QString fileName = getFileName(filePath);
    QStringList components = fileName.split("_");
    if(components.at(1) == "Oval")
        m_label_faecType->setText("椭圆脸");
    else if(components.at(1) == "Round")
        m_label_faecType->setText("圆型脸");
    else if(components.at(1) == "square")
        m_label_faecType->setText("方型脸");
    else if(components.at(1) == "Sharp")
        m_label_faecType->setText("坠子脸");
    else if(components.at(1) == "pear")
        m_label_faecType->setText("梨型脸");
    else
        m_label_faecType->setText("未知脸");

    if(components.at(2) == "E1")
        m_label_eyelidType->setText("杏眼");
    else if(components.at(2) == "E2")
        m_label_eyelidType->setText("丹凤眼");
    else if(components.at(2) == "E3")
        m_label_eyelidType->setText("上斜眼");
    else if(components.at(2) == "E4")
        m_label_eyelidType->setText("下斜眼");
    else if(components.at(2) == "E5")
        m_label_eyelidType->setText("细长眼");
    else if(components.at(2) == "E6")
        m_label_eyelidType->setText("三角眼");
    else
        m_label_eyelidType->setText("未知眼");

//    QLabel *m_label_faecType;
//    QLabel *m_label_eyelidType;
//    QLabel *m_label_browType;
//    QLabel *m_label_mouthType;

}

void Widget::handleJumpPicButton(){
    m_procIndex = m_editJumpPage->text().toInt();
    if(m_procIndex>=0 && m_procIndex<m_cachedFileList.size())
        showImage(m_procIndex);
    else {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, tr("QMessageBox::information()"), "跳转图片索引不在范围内，请重新输入");
        return;
    }
}

void Widget::handleInitButton(){
    m_rootPath = m_editRootPath->text();
    if(m_rootPath.isEmpty()){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, tr("QMessageBox::information()"), "人脸根路径地址不能为空");
        return;
    }
    getAllFiles();
    if(m_filelist.size()<=1){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::information(this, tr("QMessageBox::information()"), "指定路径没有文件");
        return;
    }
    m_currentDir.setCurrent(m_rootPath);

    qDebug()<<m_rootPath;
    m_editRootPath->hide();
    m_btnInit->hide();

    m_label->show();
    m_btnPrev->show();
    m_btnNext->show();
    m_cbFaceType->show();
    m_cbEyelidType->show();
    m_cbBrowType->show();
    m_cbMouthType->show();
    m_btnConfirm->show();

//    m_label_isTaged->show();
    m_label_faecType->show();
    m_label_eyelidType->show();
    m_label_browType->show();
    m_label_mouthType->show();
    m_label_curIndex->show();

    m_btnGo->show();
    m_editJumpPage->show();

    m_procIndex = 0;
    showImage(0);
}

void Widget::initViews(){
    m_label->hide();
    m_btnPrev->hide();
    m_btnNext->hide();
    m_cbFaceType->hide();
    m_cbEyelidType->hide();
    m_cbBrowType->hide();
    m_cbMouthType->hide();
    m_btnConfirm->hide();

//    m_label_isTaged->hide();
    m_label_faecType->hide();
    m_label_eyelidType->hide();
    m_label_browType->hide();
    m_label_mouthType->hide();
    m_label_curIndex->hide();

    m_btnGo->hide();
    m_editJumpPage->hide();
}


