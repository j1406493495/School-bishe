#include "player.h"

Player::Player(QWidget *parent): QWidget(parent)
{
    init();
    connect_all_signal_slot();
}

Player::~Player()
{
}

void Player::init()
{
    m_process = new QProcess;
    m_process->setProcessChannelMode(QProcess::MergedChannels);
    m_timing = new QTimer;
    m_playMusicFlag = 0;

    m_menuWidget = new QWidget(this);
    m_musicWidget = new QWidget(this);
    m_videoWidget = new QWidget(this);
    m_pictureWidget = new QWidget(this);
    m_gameWidget = new QWidget(this);

    m_menuWidget->setGeometry(340, 20, 90, 260);
    m_musicWidget->setGeometry(0, 0, 320, 280);
    m_videoWidget->setGeometry(0, 0, 320, 280);
    m_pictureWidget->setGeometry(0, 0, 320, 280);
    m_gameWidget->setGeometry(0, 0, 320, 280);
    m_videoWidget->hide();
    m_pictureWidget->hide();
    m_gameWidget->hide();
    m_showMusicWidget = true;
    m_showVideoWidget = false;
    m_showPictureWidget = false;
    m_showGameWidget = false;

    m_musicBtn = new QPushButton("Music", m_menuWidget);
    m_videoBtn = new QPushButton("Video", m_menuWidget);
    m_pictureBtn = new QPushButton("Picture", m_menuWidget);
    m_gameBtn = new QPushButton("Game", m_menuWidget);
    m_closeBtn = new QPushButton("Close", m_menuWidget);

    m_vLayout = new QVBoxLayout;

    m_vLayout->addWidget(m_musicBtn);
    m_vLayout->addWidget(m_videoBtn);
    m_vLayout->addWidget(m_pictureBtn);
    m_vLayout->addWidget(m_gameBtn);
    m_vLayout->addWidget(m_closeBtn);
    m_menuWidget->setLayout(m_vLayout);

    create_music_widget();
    create_video_widget();
    create_picture_widget();
    create_game_widget();
}

void Player::connect_all_signal_slot()
{
    connect(m_musicBtn, SIGNAL(clicked()), this, SLOT(on_show_music_btn()));
    connect(m_videoBtn, SIGNAL(clicked()), this, SLOT(on_show_video_btn()));
    connect(m_pictureBtn, SIGNAL(clicked()), this, SLOT(on_show_picture_btn()));
    connect(m_gameBtn, SIGNAL(clicked()), this, SLOT(on_show_game_btn()));
    connect(m_closeBtn, SIGNAL(clicked()), this, SLOT(on_show_close_btn()));
    connect(m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(on_play_time()));
    connect(m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(on_play_exit(int, QProcess::ExitStatus)));
    connect(m_timing, SIGNAL(timeout()), this, SLOT(on_play_send_time()));
}

/********* play slots ********/
void Player::on_play_start(QString playFile)
{
    QString temp = "/usr/bin/mplayer -slave -quiet " + playFile;
    m_process->start(temp);
}

void Player::on_play_pause()
{
    m_process->write("pause\n");
}

void Player::on_play_voice(int voiceValue)
{
    m_process->write(QString("volume "+QString::number(voiceValue)+" 1\n").toAscii());
}

void Player::on_play_time()
{
    while (m_process->canReadLine())
    {
        QString message(m_process->readLine()); 
        QStringList messageList = message.split("=");

        if (messageList[0] == "ANS_TIME_POSITION")
        {
            if (m_playMusicFlag == 1)
            {
                double currentTime = messageList[1].toDouble(); 
                m_musicTimeSlider->setValue(currentTime); 
            }
            else if (m_playMusicFlag == 2)
            {
                double currentTime = messageList[1].toDouble(); 
                m_videoTimeSlider->setValue(currentTime); 
            }
 
        }
        else if (messageList[0] == "ANS_LENGTH")
        {
            if (m_playMusicFlag == 1)
            {
                m_musicLong = messageList[1].toDouble(); 
                m_musicTimeSlider->setRange(0, m_musicLong); 
            }
            else if (m_playMusicFlag == 2)
            {
                m_videoLong = messageList[1].toDouble(); 
                m_videoTimeSlider->setRange(0, m_videoLong); 
            }
              
        }
    }
}

void Player::on_play_send_time()
{
    m_process->write("get_time_pos\n");
}

void Player::on_play_exit(int exitCode, QProcess::ExitStatus)
{
    m_playMusicFlag = 0;
    m_musicTimeSlider->setValue(0);
    m_videoTimeSlider->setValue(0);

    if (m_musicPlayBtn->text() == "Pause")
    {
        m_musicPlayBtn->setText("Start"); 
    }

    if (m_videoStartBtn->text() == "Pause")
    {
        m_videoStartBtn->setText("Start"); 
    }

}

/********** create son widget **********/
void Player::create_music_widget()
{
    m_musicViewWidget = new QWidget(m_musicWidget);
    m_musicControlWidget = new QWidget(m_musicWidget);
    m_musicViewWidget->setGeometry(0, 0, 320, 200);
    m_musicControlWidget->setGeometry(0, 200, 320, 80);

    m_musicListWidget = new QListWidget(m_musicViewWidget);
    m_musicListWidget->setGeometry(0, 0, 320, 200);

    m_musicOpenFileBtn = new QPushButton("Open", m_musicControlWidget);
    m_musicPlayBtn = new QPushButton("Start", m_musicControlWidget);
    m_musicVoiceSlider = new QSlider(Qt::Horizontal, m_musicControlWidget);
    m_musicTimeSlider = new QSlider(Qt::Horizontal, m_musicControlWidget);
    m_musicRoundBox = new QComboBox;
    m_musicVoiceLabel = new QLabel("Voice", m_musicControlWidget);
    m_musicTimeLabel = new QLabel("Time", m_musicControlWidget);
    m_musicVoiceSlider->setRange(0, 100);

    QStringList roundList;
    roundList << tr("Single") << tr("Random") << tr("List") << tr("List round");
    m_musicRoundBox->addItems(roundList);

    QGridLayout *gLayout = new QGridLayout;
    gLayout->addWidget(m_musicOpenFileBtn, 0, 0, 1, 1);
    gLayout->addWidget(m_musicPlayBtn, 1, 0, 1, 1);
    gLayout->addWidget(m_musicVoiceLabel, 0, 1, 1, 1);
    gLayout->addWidget(m_musicTimeLabel, 1, 1, 1, 1);
    gLayout->addWidget(m_musicRoundBox, 0, 4, 1, 2);
    gLayout->addWidget(m_musicVoiceSlider, 0, 2, 1, 2);
    gLayout->addWidget(m_musicTimeSlider, 1, 2, 1, 4);
    m_musicControlWidget->setLayout(gLayout);

    connect(m_musicPlayBtn, SIGNAL(clicked()), this, SLOT(on_music_start_btn()));
    connect(m_musicOpenFileBtn, SIGNAL(clicked()), this, SLOT(on_music_openfile_btn()));
    connect(m_musicRoundBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(on_music_roundbox(QString)));
    connect(m_musicVoiceSlider, SIGNAL(valueChanged(int)), this, SLOT(on_music_voice_slider(int)));
    connect(m_musicTimeSlider, SIGNAL(sliderMoved(int)), this, SLOT(on_music_time_slider(int)));
}

void Player::create_video_widget()
{
    m_videoViewWidget = new QWidget(m_videoWidget);
    m_videoControlWidget = new QWidget(m_videoWidget);
    m_videoViewWidget->setGeometry(0, 0, 320, 240);
    m_videoControlWidget->setGeometry(0, 240, 320, 40);

    m_videoStartBtn = new QPushButton(tr("Start"), m_videoControlWidget);
    m_videoOpenfileBtn = new QPushButton(tr("OpenFile"), m_videoControlWidget);
    m_videoTimeLabel = new QLabel(tr("Time"), m_videoControlWidget);
    m_videoTimeSlider = new QSlider(Qt::Horizontal, m_videoControlWidget);

    QGridLayout *gLayout = new QGridLayout;
    gLayout->addWidget(m_videoOpenfileBtn, 0, 0, 1, 1);
    gLayout->addWidget(m_videoStartBtn, 0, 1, 1, 1);
    gLayout->addWidget(m_videoTimeLabel, 0, 2, 1, 1);
    gLayout->addWidget(m_videoTimeSlider, 0, 3, 1, 1);
    m_videoControlWidget->setLayout(gLayout);

    connect(m_videoStartBtn, SIGNAL(clicked()), this, SLOT(on_video_start_btn()));
    connect(m_videoOpenfileBtn, SIGNAL(clicked()), this, SLOT(on_video_openfile_btn()));
    connect(m_videoTimeSlider, SIGNAL(sliderMoved(int)), this, SLOT(on_video_time_slider(int)));
}

void Player::create_picture_widget()
{
    m_photoTiming = new QTimer;
    m_photoCurrent = 0;

    m_photoViewWidget = new QWidget(m_pictureWidget);
    m_photoControlWidget = new QWidget(m_pictureWidget);
    m_photoViewWidget->setGeometry(0, 0, 320, 240);
    m_photoControlWidget->setGeometry(0, 240, 320, 40);

    m_photoOpenfileBtn = new QPushButton(tr("Openfile"), m_photoControlWidget);
    m_photoPrevBtn = new QPushButton(tr("Prev"), m_photoControlWidget);
    m_photoNextBtn = new QPushButton(tr("Next"), m_photoControlWidget);
    m_photoAutoBtn = new QPushButton(tr("Auto"), m_photoControlWidget);
    m_photoShowLabel = new QLabel;
    m_photoShowArea = new QScrollArea(m_photoViewWidget);

    m_photoShowLabel->setGeometry(0, 0, 320, 240);
    m_photoShowArea->setGeometry(0, 0, 320, 240);
    m_photoShowArea->setWidget(m_photoShowLabel);
    m_photoShowArea->setAlignment(Qt::AlignCenter);

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(m_photoOpenfileBtn);
    hLayout->addWidget(m_photoPrevBtn);
    hLayout->addWidget(m_photoAutoBtn);
    hLayout->addWidget(m_photoNextBtn);
    m_photoControlWidget->setLayout(hLayout);

    connect(m_photoOpenfileBtn, SIGNAL(clicked()), this, SLOT(on_photo_openfile_btn()));
    connect(m_photoAutoBtn, SIGNAL(clicked()), this, SLOT(on_photo_auto_btn()));
    connect(m_photoPrevBtn, SIGNAL(clicked()), this, SLOT(on_photo_prev_btn()));
    connect(m_photoNextBtn, SIGNAL(clicked()), this, SLOT(on_photo_next_btn()));
    connect(m_photoTiming, SIGNAL(timeout()), this, SLOT(on_photo_next_btn()));
}

void Player::create_game_widget()
{
    for (int i=0; i<32; i++)
    {
        for (int j=0; j<24; j++) 
        {
            m_aLive[i][j] = 0; 
        }
    }

    m_gameTiming = new QTimer;

    m_gameViewWidget = new QWidget(m_gameWidget); 
    m_gameControlWidget = new QWidget(m_gameWidget);
    m_gameViewWidget->setGeometry(0, 0, 320, 240);
    m_gameControlWidget->setGeometry(0, 240, 320, 40);

    m_gameRandomBtn = new QPushButton(tr("Random"), m_gameControlWidget);
    m_gameAutoBtn = new QPushButton(tr("Auto Start"), m_gameControlWidget);
    m_gameNextBtn = new QPushButton(tr("Next"), m_gameControlWidget);
    m_gameClearBtn = new QPushButton(tr("Clear"), m_gameControlWidget);

    QHBoxLayout *hLayout = new QHBoxLayout; 
    hLayout->addWidget(m_gameRandomBtn);
    hLayout->addWidget(m_gameAutoBtn);
    hLayout->addWidget(m_gameNextBtn);
    hLayout->addWidget(m_gameClearBtn);
    m_gameControlWidget->setLayout(hLayout);

    connect(m_gameRandomBtn, SIGNAL(clicked()), this, SLOT(on_game_random_btn()));
    connect(m_gameAutoBtn, SIGNAL(clicked()), this, SLOT(on_game_auto_btn()));
    connect(m_gameNextBtn, SIGNAL(clicked()), this, SLOT(on_game_next_btn()));
    connect(m_gameClearBtn, SIGNAL(clicked()), this, SLOT(on_game_clear_btn()));
    connect(m_gameTiming, SIGNAL(timeout()), this, SLOT(on_game_next_btn()));
}

/********* music slots **********/
void Player::on_music_start_btn()
{
    if (m_playMusicFlag == 2)
    {
        m_process->kill();
    }
    
    m_playMusicFlag = 1;

    if (m_musicPlayBtn->text() == "Start")
    {
        m_musicPlayBtn->setText("Pause");

        if (true == m_musicOpenfileFlag)
        {
            on_play_start("/home/jwh/wsi/music/qt.mp4");
            m_timing->start(1000);
            m_process->write("get_time_length\n");
            m_musicOpenfileFlag = false;
        }
        else
        {
            on_play_pause(); 
            m_timing->start(1000);
        }
    }
    else if (m_musicPlayBtn->text() == "Pause")
    {
        m_musicPlayBtn->setText("Start");

        on_play_pause();  
        m_timing->stop();
    }
}

void Player::on_music_openfile_btn()
{
    QStringList paths;

//    paths = QFileDialog::getOpenFileNames(this, tr("Open Music"), ".", tr("Music Type(*.mp3 *.wma *.o)"));

    m_musicOpenFileDialog = new QFileDialog(m_musicWidget);
    m_musicOpenFileDialog->setWindowTitle(tr("Open Music"));
    m_musicOpenFileDialog->setDirectory(".");
    m_musicOpenFileDialog->setFilter(tr("Music Type(*.mp3 *.wma *.o)"));
    m_musicOpenFileDialog->setFileMode(QFileDialog::ExistingFiles);

    if (m_musicOpenFileDialog->exec() == QFileDialog::Accepted)
    {
        paths = m_musicOpenFileDialog->selectedFiles(); 
    }

    for (int i=0; i<paths.size(); i++)
    {
        m_musicPathList << paths.at(i);
        m_musicListWidget->addItem(new QListWidgetItem(paths.at(i).section('/', -1)));
    }
}

void Player::on_music_roundbox(QString roundValue)
{

}

void Player::on_music_voice_slider(int voiceValue)
{
    on_play_voice(voiceValue);
}

void Player::on_music_time_slider(int timeValue)
{

}

/*********** video slots *************/
void Player::on_video_start_btn()
{
    if (m_playMusicFlag == 1)
    {
        m_process->kill();
    }

    m_playMusicFlag = 2;

    if (m_videoStartBtn->text() == "Start")
    {
        m_videoStartBtn->setText("Pause");

        if (m_videoOpenfileFlag == true)
        {
            on_play_start(m_videoPath);
            m_timing->start(1000);
            m_process->write("get_time_length");
            m_videoOpenfileFlag = false;
        }
        else
        {
            on_play_pause(); 
            m_timing->start(1000);
        }
    }
    else if (m_videoStartBtn->text() == "Pause")
    {
        m_videoStartBtn->setText("Start");

        on_play_pause(); 
        m_timing->stop();
    }
}
    
void Player::on_video_openfile_btn()
{
    m_videoOpenfileDialog = new QFileDialog(m_videoWidget);
    m_videoOpenfileDialog->setWindowTitle(tr("Open Video"));
    m_videoOpenfileDialog->setDirectory(".");
    m_videoOpenfileDialog->setFilter(tr("Video Type(*.mp4 *.avi *rm *rmvb *flv)"));
    m_videoOpenfileDialog->setFileMode(QFileDialog::ExistingFiles);

    if (m_videoOpenfileDialog->exec() == QFileDialog::Accepted)
    {
        m_videoPath = m_videoOpenfileDialog->selectedFiles()[0]; 
    }

    m_videoOpenfileFlag = true;
}

void Player::on_video_time_slider(int timeValue)
{

}

/********** photo slots ***********/
void Player::on_photo_auto_btn()
{
    if (m_photoAutoBtn->text() == "Auto") 
    {
        m_photoAutoBtn->setText("Quit");        
        m_photoTiming->start(1000);
    }
    else if (m_photoAutoBtn->text() == "Quit")
    {
        m_photoAutoBtn->setText("Auto"); 
        m_photoTiming->stop();
    }
}

void Player::on_photo_prev_btn()
{
    if (0 == m_photoCurrent)
    {
        m_photoCurrent = m_photoPathList.size() - 1; 
    }
    else
    {
        m_photoCurrent -= 1; 
    }

    m_photoPix.load(m_photoPathList[m_photoCurrent]);
    m_photoPix = m_photoPix.scaled(m_photoWidth, m_photoHeight, Qt::IgnoreAspectRatio);
    m_photoShowLabel->setPixmap(m_photoPix);
}

void Player::on_photo_next_btn()
{
    if (m_photoPathList.size()-1 == m_photoCurrent)
    {
        m_photoCurrent = 0; 
    }
    else
    {
        m_photoCurrent += 1; 
    }

    m_photoPix.load(m_photoPathList[m_photoCurrent]);
    m_photoPix = m_photoPix.scaled(m_photoWidth, m_photoHeight, Qt::IgnoreAspectRatio);
    m_photoShowLabel->setPixmap(m_photoPix);
}

void Player::on_photo_openfile_btn()
{
    QStringList paths;

    m_photoOpenfileDialog = new QFileDialog(m_pictureWidget);
    m_photoOpenfileDialog->setWindowTitle(tr("Open Photo"));
    m_photoOpenfileDialog->setDirectory(".");
    m_photoOpenfileDialog->setFilter(tr("Phote Type(*.jpg *.png)"));
    m_photoOpenfileDialog->setFileMode(QFileDialog::ExistingFiles);

    if (m_photoOpenfileDialog->exec() == QFileDialog::Accepted)
    {
        paths = m_photoOpenfileDialog->selectedFiles(); 
    }

    if (false == m_photoPathList.isEmpty())
    {
        m_photoCurrent = m_photoPathList.size(); 
    }

    for (int i=0; i<paths.size(); i++)
    {
        if (false == m_photoPathList.contains(paths.at(i)))
        {
            m_photoPathList << paths.at(i);
        }
    }

    m_photoPix.load(m_photoPathList[m_photoCurrent]);
    m_photoWidth = m_photoShowLabel->width();
    m_photoHeight = m_photoShowLabel->height();
    m_photoPix = m_photoPix.scaled(m_photoWidth, m_photoHeight, Qt::IgnoreAspectRatio);
    m_photoShowLabel->setPixmap(m_photoPix);
}

/********* game slots *********/
void Player::on_game_random_btn()
{
    qsrand(time(NULL));

    for (int i=0; i<16; i++)
    {
        for (int j=0; j<12; j++) 
        {
            int xValue = qrand() % 32; 
            int yValue = qrand() % 24;

            m_aLive[xValue][yValue] = 1;
        }
    }

    update();
}

void Player::on_game_auto_btn()
{
    if ("Auto Start" == m_gameAutoBtn->text())
    {
        m_gameAutoBtn->setText(tr("Auto Quit")); 
        m_gameTiming->start(500);
    }
    else if ("Auto Quit" == m_gameAutoBtn->text())
    {
        m_gameAutoBtn->setText(tr("Auto Start")); 
        m_gameTiming->stop();
    }
}

void Player::on_game_next_btn()
{
    int aTemp[32][24];

    for (int i=0; i<32; i++)
    {
        for (int j=0; j<24; j++) 
        {
            aTemp[i][j] = m_aLive[i][j]; 
        }
    }

    for (int i=0; i<32; i++)
    {
        for (int j=0; j<24; j++) 
        {
            int count = cell_live_count(i, j);     

            if (2 == count)
            { 
            }
            else if (3 == count)
            {
                aTemp[i][j] = 1; 
            }
            else
            {
                aTemp[i][j] = 0; 
            }
        }
    }

    for (int i=0; i<32; i++)
    {
        for (int j=0; j<24; j++) 
        {
            m_aLive[i][j] = aTemp[i][j]; 
        }
    }

    update();
}

int Player::cell_live_count(int i, int j)
{
    int count = 0;

    for (int row=i-1; row<i+2; row++)
    {
        for (int col=j-1; col<j+2; col++) 
        {
            if (row>=0 && row<32 && col>=0 && col<24 
                    && !(row==i&&col==j) && (m_aLive[row][col]==1)) 
            {
                count++; 
            }
        }
    }

    return count;
}

void Player::on_game_clear_btn()
{
    for (int i=0; i<32; i++)
    {
        for (int j=0; j<24; j++) 
        {
            m_aLive[i][j] = 0; 
        }
    }

    update();
}

void Player::paintEvent(QPaintEvent *)
{
    if (m_showGameWidget == true)
    {
        QPainter paint(this);
        paint.setPen(Qt::black);
        paint.setBrush(Qt::white);

        paint.drawRect(0, 0, 320, 240);

        for (int i=10; i<320; i+=10)
        {
            paint.drawLine(i, 0, i, 240); 
        }

        for (int i=10; i<240; i+=10)
        {
            paint.drawLine(0, i, 320, i); 
        }


        for (int i=0; i<32; i++)
        {
            for (int j=0; j<24; j++) 
            {
                if ( 1 == m_aLive[i][j]) 
                {
                    paint.setBrush(Qt::blue);
                    paint.drawEllipse(10*i, 10*j, 10, 10);
                }
                else if (0 == m_aLive[i][j])
                {
                    paint.setBrush(Qt::white);
                    paint.drawRect(10*i, 10*j, 10, 10);
                }
            }
        }
    }
}

void Player::mousePressEvent(QMouseEvent *event)
{
    int xValue = event->pos().x() / 10;
    int yValue = event->pos().y() / 10;

    if (xValue<32 && yValue<24)
    {
        if (0 == m_aLive[xValue][yValue])
        {
            m_aLive[xValue][yValue] = 1;  
        }
        else if (1 == m_aLive[xValue][yValue])
        {
            m_aLive[xValue][yValue] = 0;  
        }
    }

    update();
}

/******** which widget show ********/
void Player::on_show_music_btn()
{
    if (true == m_showVideoWidget)
    {
        m_showVideoWidget = false;
        m_videoWidget->hide();
    }

    if (true == m_showPictureWidget)
    {
        m_showPictureWidget = false;
        m_pictureWidget->hide();
    }

    if (true == m_showGameWidget)
    {
        m_showGameWidget = false;
        m_gameWidget->hide();
    }

    if (false == m_showMusicWidget)
    {
        m_showMusicWidget = true;
        m_musicWidget->show();
    }
}

void Player::on_show_video_btn()
{
    if (false == m_showVideoWidget)
    {
        m_showVideoWidget = true;
        m_videoWidget->show();
    }

    if (true == m_showPictureWidget)
    {
        m_showPictureWidget = false;
        m_pictureWidget->hide();
    }

    if (true == m_showGameWidget)
    {
        m_showGameWidget = false;
        m_gameWidget->hide();
    }

    if (true == m_showMusicWidget)
    {
        m_showMusicWidget = false;
        m_musicWidget->hide();
    }
}

void Player::on_show_picture_btn()
{
    if (true == m_showVideoWidget)
    {
        m_showVideoWidget = false;
        m_videoWidget->hide();
    }
 
    if (false == m_showPictureWidget)
    {
        m_showPictureWidget = true;
        m_pictureWidget->show();
    }

    if (true == m_showGameWidget)
    {
        m_showGameWidget = false;
        m_gameWidget->hide();
    }

    if (true == m_showMusicWidget)
    {
        m_showMusicWidget = false;
        m_musicWidget->hide();
    }
}

void Player::on_show_game_btn()
{
    if (true == m_showVideoWidget)
    {
        m_showVideoWidget = false;
        m_videoWidget->hide();
    }
 
    if (true == m_showPictureWidget)
    {
        m_showPictureWidget = false;
        m_pictureWidget->hide();
    }

    if (false == m_showGameWidget)
    {
        m_showGameWidget = true;
        m_gameWidget->show();
    }

    if (true == m_showMusicWidget)
    {
        m_showMusicWidget = false;
        m_musicWidget->hide();
    }
}

void Player::on_show_close_btn()
{
    m_process->kill();
    m_playMusicFlag = 0;
    m_musicTimeSlider->setValue(0);
    m_videoTimeSlider->setValue(0);

    if (m_musicPlayBtn->text() == "Pause")
    {
        m_musicPlayBtn->setText("Start"); 
    }

    if (m_videoStartBtn->text() == "Pause")
    {
        m_videoStartBtn->setText("Start"); 
    }

    close();
}
