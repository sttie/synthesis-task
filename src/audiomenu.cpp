#include "../include/audiomenu.h"
#include "../ui/ui_audiomenu.h"
#include "../include/synthesizer.hpp"

#include <QString>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>


AudioMenu::AudioMenu(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::AudioMenu)
{
    ui->setupUi(this);
    setFixedSize(geometry().width(), geometry().height());
    setWindowTitle("Синтезатор частот");

    // Коннектим событие нажатия на кнопку к функции записи файла
    connect(ui->write_button, &QPushButton::clicked, this, &AudioMenu::WriteSoundFile);
}

AudioMenu::~AudioMenu() noexcept {
    delete ui;
}

void AudioMenu::WriteSoundFile() {
    if (ui->samplerate_edit->text().isEmpty()
            || ui->duration_edit->text().isEmpty()
            || ui->decay_edit->text().isEmpty()
            || ui->freqs_edit->text().isEmpty()) {
        ThrowErrorDialog("Некоторые поля пусты!");
        return;
    }

    int32_t sample_rate, duration;
    double energy_decay;
    // При ошибке перевода из строки в int/double в ok запишется false
    bool ok = true;

    sample_rate = ui->samplerate_edit->text().toInt(&ok);
    duration = ui->duration_edit->text().toInt(&ok);
    energy_decay = ui->decay_edit->text().toDouble(&ok);
    if (!ok) {
        ThrowErrorDialog("Все поля должны содержать только числа!");
        return;
    }

    // Извлекаем из LineEdit числа, разделенные пробелами и переводим их во double
    QStringList str_freqs_list = ui->freqs_edit->text().split(QRegExp("\\s+"), QString::SkipEmptyParts);
    std::vector<double> freqs(str_freqs_list.size());
    for (int i = 0; i < str_freqs_list.size(); ++i) {
        freqs[i] = str_freqs_list[i].toDouble(&ok);
        if (!ok) {
            ThrowErrorDialog("Все поля должны содержать только числа!");
            return;
        }
    }

    // Синтезируем звук и получаем соответствующий заголовок .wav файла
    auto wav_header = SynthesizeWavHeader(sample_rate, duration, energy_decay, 1, freqs);
    auto filename = QFileDialog::getSaveFileName(this, "Сохранение .wav файла", QDir::currentPath());
    WriteWav(wav_header, filename);
}


void AudioMenu::ThrowErrorDialog(const std::string& message) {
    QMessageBox::warning(this, "Ошибка", QString::fromStdString(message));
}
