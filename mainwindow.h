#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QJsonDocument>
#include <QAxObject>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
public slots:
void on_log(const QString & msg);

private slots:
	void on_enterIPButton_clicked();

	void on_calcButton_clicked();

	void on_LogBrowser_cursorPositionChanged();

	void on_clearLogButton_clicked();

	void on_saveLogButton_clicked();

	void on_execButton_clicked();

	void on_ModulecomboBox_currentIndexChanged(const QString &arg1);

	void on_CommandcomboBox_currentIndexChanged(const QString &arg1);



private:
	Ui::MainWindow *ui;
	const QString applicationDirPath;
	void initComboBoxList();

};

#endif // MAINWINDOW_H
