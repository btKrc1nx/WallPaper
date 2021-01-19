#include "WallPaper.h"

WallPaper::WallPaper(QWidget *parent)
    : QWidget(parent), isDraging(false),settingWidget(nullptr) {
    ui.setupUi(this);
    iniUi();
    iniStyleSheet();
    iniSystemTray();
    iniConnect();
}

WallPaper::~WallPaper(){
}

void WallPaper::iniUi() {
    settingWidget = nullptr;
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this); 
    videoWidget = new QVideoWidget(this);
    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_LayoutOnEntireRect,true);
    setWindowIcon(QIcon(":/WallPaper/Resources/favicon.ico"));
}

void WallPaper::iniStyleSheet() {
    this->myStyleSheet = QString("\
        #widget{\
            border-image:url(%1);\
            background-repeat:no-repeat;\
                }\
        QPushButton{\
            background-color:#222a30;\
            border-radius:10px;\
            border:none;\
            width:40px;\
            height:40px;\
            color:#ffffff;\
    }\
        QPushButton:hover{\
            background-color:pink;\
}\
        #groupBox{border:none;}");
    this->ui.widget->setStyleSheet(myStyleSheet.arg(":/WallPaper/Resources/wz.png"));
}

void WallPaper::iniConnect() {
    QObject::connect(ui.btnMinimise, SIGNAL(clicked()), this, SLOT(onBtnMinimiseClicked()));
    QObject::connect(this->systemTray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
        this, SLOT(onSystemTrayActivated(QSystemTrayIcon::ActivationReason)));
    QObject::connect(ui.btnSetting, SIGNAL(clicked()), this, SLOT(onBtnSettingClicked()));
}

void WallPaper::iniSystemTray() {
    /*ϵͳ����*/
    systemTray = new QSystemTrayIcon(this);
    systemTray->setIcon(QIcon(":/WallPaper/Resources/favicon.ico"));
    systemTray->setVisible(true);
    systemTray->setToolTip(QString(QStringLiteral("by btKrc1nx")));
    QMenu *menu = new QMenu(this);
    menu->addAction(ui.actionExit);
    systemTray->setContextMenu(menu);
}


void WallPaper::onSystemTrayActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        this->show();
        systemTray->hide();
    }
}

void WallPaper::onBtnSettingClicked(){
    if (!settingWidget) {
        /*�Ӹ���̳�����*/
        settingWidget = new SettingWidget(this);
        /*����ʵ�������Ӳۺ���*/
        QObject::connect(settingWidget, SIGNAL(selectImage(QString *)), this, SLOT(onSelectImage(QString *)));
        QObject::connect(settingWidget, SIGNAL(selectWallPaper(QString *)), this, SLOT(onSelectWallPaper(QString *)));

        settingWidget->show();
        settingWidget->move(QPoint(30, 30));
    }
    else {
        settingWidget->close();
        settingWidget = nullptr;
    }
}

/*���෢����źţ�����һ���ַ���ָ��*/
void WallPaper::onSelectImage(QString *fileName){
    if (fileName)
        this->ui.widget->setStyleSheet(myStyleSheet.arg(*fileName));
}

void WallPaper::onSelectWallPaper(QString *fileName){
    if (fileName) {
        /*û�и��࣬��Ҫ��������*/
        videoWidget->setWindowFlag(Qt::FramelessWindowHint);
        playlist->clear();
        playlist->addMedia(QUrl::fromLocalFile(*fileName));

        /*��䴰��*/
        videoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
        player->setVideoOutput(videoWidget);
        player->setPlaylist(playlist);
        player->setMuted(true);
        videoWidget->setFullScreen(true);//ȫ��
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

        player->play();

        videoWidget->show();
 
        SetParent((HWND)(videoWidget->winId()), FindWindowEx(NULL, NULL, L"progman", NULL));

    }
}


void WallPaper::mousePressEvent(QMouseEvent *event) {
    startCoor = event->pos();//���������Դ��ڵ�����
    /*������������ᴥ�����¼�,����Ҫ�����ж��Ƿ��������������*/
    isDraging = true;
    QWidget::mousePressEvent(event);
}

void WallPaper::mouseMoveEvent(QMouseEvent *event) {
    /*������ֵ��������ö��ֵ����λ������*/
    if (event->buttons() & Qt::LeftButton&&isDraging) {
        this->move(event->globalPos() - startCoor);
    }
    QWidget::mouseMoveEvent(event);
}

void WallPaper::mouseReleaseEvent(QMouseEvent *event) {
    isDraging = false;
    QWidget::mouseReleaseEvent(event);
}

void WallPaper::enterEvent(QEvent *event){
    ui.groupBox->show();
}

void WallPaper::leaveEvent(QEvent *event){
    ui.groupBox->hide();
}

/*slots*/

void WallPaper::onBtnMinimiseClicked() {
    this->hide();
    systemTray->show();
}

/*slots end*/