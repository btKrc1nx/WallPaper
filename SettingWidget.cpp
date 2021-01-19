#include "SettingWidget.h"
SettingWidget::SettingWidget(QWidget *parent)
    :QWidget(parent) {
    ui.setupUi(this);
    iniUi();
    iniConnect();
}

void SettingWidget::iniUi() {
    /*���ô����ԣ���ָ��ָ����ڴ��ڹر�ʱ���Զ��ͷ�*/
    setAttribute(Qt::WA_DeleteOnClose);
    setStyleSheet("#widget{background-color:rgba(0,0,0,0.5);border-radius:10px;}\
QPushButton{color:orange;background-color:black;border-radius:10px;}");
}

void SettingWidget::iniConnect() {
    QObject::connect(ui.btnSelectImage, SIGNAL(clicked()), this, SLOT(onBtnSelectImage()));
    QObject::connect(ui.btnSelectWallPaper,SIGNAL(clicked()), this, SLOT(onBtnSelectWallPaper()));
}

void SettingWidget::onBtnSelectWallPaper(){
    QString fileName("");//ֱ�ӹ��죬��ʹ�ÿ�������
    fileName = QFileDialog::getOpenFileName(this, QString(QStringLiteral("ѡ��һ����Ƶ")), 
        "../" + QApplication::applicationDirPath(),
        "image (*.mp4;*.avi;*.wmv)");
    if (fileName != "")
        emit selectWallPaper(&fileName);
    else
        emit selectWallPaper(nullptr);

}

void SettingWidget::onBtnSelectImage() {
    /*����û�ȡ��������һ����ָ��*/
    QString fileName("");
    fileName = QFileDialog::getOpenFileName(this, QString(QStringLiteral("ֻ֧��png��ʽ")), "", "image (*.png)");
    if (fileName != "")
        emit selectImage(&fileName);
    else
        emit selectImage(nullptr);
}
