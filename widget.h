#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPicture>
#include <QComboBox>
#include <QMessageBox>
#include <QLineEdit>
#include "globalcfg.h"
#include <QStringList>
#include<QFileInfoList>
#include<QDir>
#include <map>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();
private slots:
    void handleNextButton();
    void handlePrevButton();
    void handleConfirmButton();
    void handleInitButton();
    void handleJumpPicButton();

private:
    QFileInfoList getAllFiles();
    void showImage(int idx);

    void initViews();
    QString getFileName(QString fullpath);
private:
    Ui::Widget *ui;

    QLineEdit *m_editRootPath;
    QPushButton *m_btnInit;

    QPushButton *m_btnNext;
    QPushButton *m_btnPrev;
    QLabel *m_label;

    QPushButton *m_btnGo;
    QLineEdit *m_editJumpPage;

    QComboBox *m_cbFaceType;
    QComboBox *m_cbEyelidType;
    QComboBox *m_cbBrowType;
    QComboBox *m_cbMouthType;

    QPushButton *m_btnConfirm;
    QFileInfoList m_filelist;

    int m_procIndex;
    QString m_rootPath;

    QLabel *m_label_curIndex;
    QLabel *m_label_isTaged;
    QLabel *m_label_faecType;
    QLabel *m_label_eyelidType;
    QLabel *m_label_browType;
    QLabel *m_label_mouthType;

    QStringList m_strFaceTypeList;
    QStringList m_strEyelidTypeList;
    QStringList m_strBrowTypeList;
    QStringList m_strMouthTypeList;

    std::map<int,FaceInfo> m_faceInfoList;
    QDir m_currentDir;
    QStringList m_cachedFileList;
};

#endif // WIDGET_H
