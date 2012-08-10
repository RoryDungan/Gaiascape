#ifndef ADDOBJECTDIALOG_H
#define ADDOBJECTDIALOG_H

#include <QDialog>

namespace Ui {
class AddObjectDialog;
}

class AddObjectDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddObjectDialog(QWidget *parent = 0);
    ~AddObjectDialog();
    
private:
    Ui::AddObjectDialog *ui;
};

#endif // ADDOBJECTDIALOG_H
