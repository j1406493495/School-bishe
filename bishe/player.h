#ifndef __PLAYER_H_
#define __PLAYER_H_

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include <QTime>
#include <QTimer>
#include <QSlider>
#include <QString>
#include <QProcess>
#include <QFileDialog>
#include <QListWidget>
#include <QComboBox>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QPixmap>
#include <QScrollArea>

class Player: public QWidget
{
    Q_OBJECT
   
public:
    Player(QWidget *parent = 0);
    ~Player();

protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
private:
/******** whole ********/
    QPushButton *m_musicBtn;
    QPushButton *m_videoBtn;
    QPushButton *m_pictureBtn;
    QPushButton *m_gameBtn;
    QPushButton *m_closeBtn;
    QWidget *m_menuWidget;
    QWidget *m_musicWidget;
    QWidget *m_videoWidget;
    QWidget *m_pictureWidget;
    QWidget *m_gameWidget;
    bool m_showMusicWidget;
    bool m_showVideoWidget;
    bool m_showPictureWidget;
    bool m_showGameWidget;
    QVBoxLayout *m_vLayout; 

    void init();
    void connect_all_signal_slot();
    void create_music_widget();
    void create_video_widget();
    void create_picture_widget();
    void create_game_widget();

/********* play *********/
    QProcess *m_process;
    QTimer *m_timing;
    int m_playMusicFlag;

/********* music *********/
    QStringList m_musicPathList;
    QWidget *m_musicViewWidget;
    QWidget *m_musicControlWidget;
    QPushButton *m_musicOpenFileBtn;
    QPushButton *m_musicPlayBtn;
    QSlider *m_musicVoiceSlider;
    QSlider *m_musicTimeSlider;
    QComboBox *m_musicRoundBox;
    QLabel *m_musicVoiceLabel;
    QLabel *m_musicTimeLabel;
    QFileDialog *m_musicOpenFileDialog; 
    QListWidget *m_musicListWidget;
    int m_musicOpenfileFlag;
    double m_musicLong;

/********** video ***********/
    QWidget *m_videoViewWidget;
    QWidget *m_videoControlWidget;
    QFileDialog *m_videoOpenfileDialog;
    QPushButton *m_videoStartBtn;
    QPushButton *m_videoOpenfileBtn;
    QLabel *m_videoTimeLabel;
    QSlider *m_videoTimeSlider;
    QString m_videoPath;
    bool m_videoOpenfileFlag;
    double m_videoLong;

/********** picture ***********/
    QWidget *m_photoViewWidget;
    QWidget *m_photoControlWidget;
    QPushButton *m_photoAutoBtn;
    QPushButton *m_photoPrevBtn;
    QPushButton *m_photoNextBtn;
    QPushButton *m_photoOpenfileBtn;
    QFileDialog *m_photoOpenfileDialog;
    QLabel *m_photoShowLabel;
    QStringList m_photoPathList;
    QScrollArea *m_photoShowArea; 
    QPixmap m_photoPix;
    QTimer *m_photoTiming;
    int m_photoCurrent;
    qreal m_photoWidth;
    qreal m_photoHeight;

/********** game ***********/
    QPushButton *m_gameRandomBtn;
    QPushButton *m_gameAutoBtn;
    QPushButton *m_gameNextBtn;
    QPushButton *m_gameClearBtn;
    QWidget *m_gameViewWidget;
    QWidget *m_gameControlWidget;
    QTimer *m_gameTiming;
    int m_aLive[32][24];

    int cell_live_count(int, int);

private slots:
/********* whole slots **********/
    void on_show_music_btn();
    void on_show_video_btn();
    void on_show_picture_btn();
    void on_show_game_btn();
    void on_show_close_btn();

/****** play slots *******/
    void on_play_start(QString);
    void on_play_pause();
    void on_play_voice(int);
    void on_play_time();
    void on_play_send_time();
    void on_play_exit(int, QProcess::ExitStatus);

/******** music slots ********/
    void on_music_start_btn();
    void on_music_openfile_btn();
    void on_music_roundbox(QString);
    void on_music_voice_slider(int);
    void on_music_time_slider(int);

/********* video slots *********/
    void on_video_start_btn();
    void on_video_openfile_btn();
    void on_video_time_slider(int);

/********* picture slots *********/
    void on_photo_auto_btn();
    void on_photo_prev_btn();
    void on_photo_next_btn();
    void on_photo_openfile_btn();

/********* game slots *********/
    void on_game_random_btn();
    void on_game_auto_btn();
    void on_game_next_btn();
    void on_game_clear_btn();
};

#endif
