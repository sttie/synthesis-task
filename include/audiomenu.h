#pragma once

#include <QMainWindow>
#include <QDebug>
#include <QDir>
#include <string>
#include <fstream>

#include "wav.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class AudioMenu; }
QT_END_NAMESPACE


class AudioMenu : public QMainWindow
{
    Q_OBJECT

public:
    AudioMenu(QWidget* parent = nullptr);
    ~AudioMenu();

public slots:
    void WriteSoundFile();

private:
    template <typename T>
    void WriteWav(const wav::WavHeader<T>& header, const QString& filename) {
        std::ofstream file(filename.toStdString());
        file << header;
        file.close();

        qDebug() << "Created" << filename;
    }

    void ThrowErrorDialog(const std::string& message);

    Ui::AudioMenu* ui;
};
