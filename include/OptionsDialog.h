#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OptionsDialog(QWidget *parent = 0, QSettings* options = 0);
    ~OptionsDialog();
    
private:
    Ui::OptionsDialog *ui;

    QSettings* mOptions;

signals:
    void settingsUpdated();

public slots:
    void accept();
};

#endif // OPTIONSDIALOG_H
