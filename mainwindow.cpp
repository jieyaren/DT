#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QtGlobal>
#include <QMessageBox>
#include <QDate>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QVariant>

#include "udp.h"
#include "log.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),applicationDirPath(QCoreApplication::applicationDirPath())
{
	ui->setupUi(this);

	qInstallMessageHandler(myMessageOutput);
	connect(logger::instance(),SIGNAL(log_signnal(QString)),this,SLOT(on_log(QString)));
	qInfo() << DEBUG_TOOL_VERSION;
	loadCommandJson();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_enterIPButton_clicked()
{
	QString ipinfo = ui->IPLineEdit->text();

	if ((!ipinfo.isEmpty()) && (Udp::instance()->setIP(ipinfo)))
	{
		qDebug() << "Valid IP Address"<<Udp::instance()->ip();
	}
	else
	{
		QMessageBox::warning(this, "warnning", "IP Invalid", QMessageBox::Yes);
		qWarning("Invalid IP address");
	}		   
}


void MainWindow::on_log(const QString & msg)
{
	ui->LogBrowser->append(msg);
}



struct RAII_FILE_HANDLE_WRITE
{
	QFile& i_;
	explicit RAII_FILE_HANDLE_WRITE(QFile& in) : i_(in)
	{
		bool f = i_.open(QIODevice::WriteOnly | QIODevice::Append);
		if (!f)
			qWarning() << "[LOAD] can't write!";
	}

	~RAII_FILE_HANDLE_WRITE()
	{
		i_.close();
	}
};

struct RAII_FILE_HANDLE_READ
{
	QFile& i_;
	explicit RAII_FILE_HANDLE_READ(QFile& in): i_(in)
	{
		bool f =i_.open(QFile::ReadOnly);
		if(!f)
			qWarning() << "[LOAD] can't open or json file missing! if continue, you must write your msg with json format in left editer";
	}

	~RAII_FILE_HANDLE_READ()
	{
		i_.close();
	}
};

void MainWindow::loadCommandJson()
{
	qDebug() << "loading json...";
	QFile inputFile(applicationDirPath + "/cmd.json");

	RAII_FILE_HANDLE_READ inputFileHandleGuard(inputFile);

	//qDebug() << inputFile.readAll();
	//这个操作会把inputfile内容清空导致后面构造失败
	//我因为这个折腾了一上午
		

	QByteArray data = inputFile.readAll();
	
	json = QJsonDocument::fromJson(data);
	//qDebug() << data;
	if (json.isEmpty())
	{
		qWarning() << "[LOAD] Json File Empty or Format Error! you need write it in left text edit or add item in cmd.json!";
	}

	// JO->"cmd" : JA
	//
	QStringList ModuleItem;
	QStringList CommandComboxItem;
	QJsonObject jo = json.object();
	if (jo.contains("cmd"))
	{
		QJsonArray c = jo.take("cmd").toArray();
		for (QJsonArray::iterator i = c.begin(); i != c.end(); ++i)
		{
			//qInfo() << i->toObject().take("-pid").toString().toInt();
			//qInfo() << i->toObject().take("-pid").toInt();真他妈服了
			
			QJsonArray ja = i->toObject().take("cmdinfo").toArray();
			switch (i->toObject().take("pid").toString().toInt())
			{
			case LLC:
				ModuleItem << "LLC";
				for (QJsonArray::iterator j = ja.begin(); j != ja.end(); ++j)
				{
					CommandComboxItem << j->toObject().take("name").toString();
				}
				break;

			case CC:
				ModuleItem << "CC";
				for (QJsonArray::iterator j = ja.begin(); j != ja.end(); ++j)
				{
					CommandComboxItem << j->toObject().take("name").toString();
				}
				break;

			case MM:
				ModuleItem << "MM";
				for (QJsonArray::iterator j = ja.begin(); j != ja.end(); ++j)
				{
					CommandComboxItem << j->toObject().take("name").toString();
				}
				break;

			case RT:
				ModuleItem << "RT";
				for (QJsonArray::iterator j = ja.begin(); j != ja.end(); ++j)
				{
					CommandComboxItem << j->toObject().take("name").toString();
				}
				break;

			case RM:
				ModuleItem << "RM";
				for (QJsonArray::iterator j = ja.begin(); j != ja.end(); ++j)
				{

					CommandComboxItem << j->toObject().take("name").toString();
				}
				break;
			case VSW:
				ModuleItem << "VSW";
				for (QJsonArray::iterator j = ja.begin(); j != ja.end(); ++j)
				{
					CommandComboxItem << j->toObject().take("name").toString();
				}
				break;

			case BDBS:
				ModuleItem << "BDBS";
				for (QJsonArray::iterator j = ja.begin(); j != ja.end(); ++j)
				{
					CommandComboxItem << j->toObject().take("name").toString();
				}
				break;
			default:
				ModuleItem << "ALL";
				break;
			}
		}
	}

	ui->ModulecomboBox->addItems(ModuleItem);
	ui->CommandcomboBox->addItems(CommandComboxItem);

	qDebug() << "loading json end";
}


void MainWindow::loadCommandItemJson(QString PidString,QString CommandString)
{
	QJsonArray c = json.object().take("cmd").toArray();

	for (QJsonArray::iterator i = c.begin(); i != c.end(); ++i)
	{
		if (i->toObject().take("pid_name").toString() == PidString)
		{
			QJsonArray ja = i->toObject().take("cmdinfo").toArray();
			for (QJsonArray::iterator j = ja.begin(); j != ja.end(); ++j)
			{
				if (CommandString == j->toObject().take("name").toString())
				{
					QJsonArray display = j->toObject().take("items").toArray();
					//display.toVariantList()
					//ui->textEdit->setPlainText();
				}
			}
		}
	}
}


void MainWindow::loadCommandItemJson(QString PidString)
{
	//update ComandComboboxItem
	qDebug() << "update pid command " << PidString;
	QStringList CommandComboxItem;
	QJsonObject jo = json.object();
	QJsonArray c = jo.take("cmd").toArray();
	
	for (QJsonArray::iterator i = c.begin(); i != c.end(); ++i)
	{
		if (i->toObject().take("pid_name").toString() == PidString)
		{
			QJsonArray ja = i->toObject().take("cmdinfo").toArray();
			for (QJsonArray::iterator j = ja.begin(); j != ja.end(); ++j)
			{
				CommandComboxItem << j->toObject().take("name").toString();
				qDebug() << j->toObject().take("name").toString();
			}
		}
	}

	ui->CommandcomboBox->clear();
	ui->CommandcomboBox->addItems(CommandComboxItem);
}

void MainWindow::on_calcButton_clicked()
{
	qDebug() << "[EVENT] call calc...";
	system("calc");
}

void MainWindow::on_LogBrowser_cursorPositionChanged()
{
	QTextCursor c = ui->LogBrowser->textCursor();
	c.movePosition(QTextCursor::End);
	ui->LogBrowser->setTextCursor(c);
}

void MainWindow::on_clearLogButton_clicked()
{
	ui->LogBrowser->clear();
}

void MainWindow::on_saveLogButton_clicked()
{

	QString filename = applicationDirPath + "/DT-log-"+QDateTime::currentDateTime().toString(QLatin1String("MM-dd-hh-mm-ss-zzz"))+".log";        
	QString str = ui->LogBrowser->toPlainText();
	QFile file(filename);
	RAII_FILE_HANDLE_WRITE logfile(file);
	QTextStream in(&file);
	in << str << "\n";
	qDebug() << "[EVENT] Save Log "<<filename;
}

void MainWindow::on_execButton_clicked()
{
	if (ui->ModulecomboBox->currentText().isNull()||ui->CommandcomboBox->currentText().isNull())
	{
		qWarning() << "[Window] Please Make Sure The Message Item";
		return;
	}

	QJsonObject jo = json.object();
	QJsonArray c = jo.take("cmd").toArray();
	QJsonArray commandsave;
	int pid = VOS;
	unsigned int msg_code = static_cast<unsigned int>(-1);

	for (QJsonArray::iterator i = c.begin(); i != c.end(); ++i)
	{
		if (i->toObject().take("pid_name").toString() != ui->ModulecomboBox->currentText())
		{
			continue;
		}
		pid = i->toObject().take("pid").toString().toInt();
		QJsonArray ja = i->toObject().take("cmdinfo").toArray();
		for (QJsonArray::iterator j = ja.begin(); j != ja.end(); ++j)
		{
			if (j->toObject().take("name").toString() != ui->CommandcomboBox->currentText())
			{
				continue;
			}
			commandsave = j->toObject().take("items").toArray();
			msg_code = j->toObject().take("code").toString().toInt(nullptr,16);
		}
	}
	
	if (pid != VOS && msg_code != static_cast<unsigned int>(-1))
	{
		CommandExecSession::instance()->Exec((tsc_pid)pid, msg_code,commandsave);
	}
}

void MainWindow::on_ModulecomboBox_currentIndexChanged(const QString &arg1)
{
	loadCommandItemJson(arg1);
	loadCommandItemJson(arg1, ui->CommandcomboBox->currentText());
}

void MainWindow::on_CommandcomboBox_currentIndexChanged(const QString &arg1)
{
	loadCommandItemJson(ui->ModulecomboBox->currentText(),arg1);
}

void MainWindow::on_textEdit_textChanged()
{
	
}
