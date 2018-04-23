#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>

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
#include <QAxObject>
#include <QtEndian>

#include "udp.h"
#include "log.h"
#include "tools.h"
#include "user_define_msg.h"
#include "excel_parser.h"


MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	applicationDirPath(QCoreApplication::applicationDirPath())

{
	ui->setupUi(this);

	qInstallMessageHandler(myMessageOutput);
	connect(logger::instance(),SIGNAL(log_signnal(QString)),this,SLOT(on_log(QString)));
	qInfo() << DEBUG_TOOL_VERSION;
	initComboBoxList();

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initComboBoxList()
{
	QStringList ModuleItem;
	ModuleItem <<"LLC"<<"CC"<<"MM"<<"RM"<<"PD"<<"VSW"<<"SD"<<"BDBS";
	ui->ModulecomboBox->addItems(ModuleItem);
}
void MainWindow::on_enterIPButton_clicked()
{
	QString ipinfo = ui->IPLineEdit->text();

	if ((!ipinfo.isEmpty()) && (Udp::instance()->setIP(ipinfo)))
	{
		qDebug() << "[Window] Valid IP Address"<<Udp::instance()->ip();
	}
	else
	{
		QMessageBox::warning(this, "warnning", "IP Invalid", QMessageBox::Yes);
		qWarning("[Window] Invalid IP address");
	}		   
}


void MainWindow::on_log(const QString & msg)
{
	ui->LogBrowser->append(msg);
}


void MainWindow::on_calcButton_clicked()
{
	qDebug() << "[Window] call calc...";
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
	qDebug() << "[Window] Save Log "<<filename;
}

void MainWindow::on_execButton_clicked()
{
	
	if (ui->ModulecomboBox->currentText().isNull()||ui->CommandcomboBox->currentText().isNull()||ui->IPLineEdit->text().isEmpty())
	{
		qWarning() << "[Window] Please Check ip,Or Combobox Items";
		return;
	}
	
	unsigned int msg_code = getMsgCode(ui->ModulecomboBox->currentText().toStdString(),
									ui->CommandcomboBox->currentText().toStdString());

	int datalen = 100;

	if (ui->CommandcomboBox->currentText() == "GPS_START")
	{
		QAxObject excel("Excel.Application");
		excel.setProperty("Visible", false);
		excel.setProperty("Caption", "Qt Excel");
		QAxObject * workbooks = excel.querySubObject("WorkBooks");

		workbooks->dynamicCall("Open (const QString&)", QString(applicationDirPath+"/gps.xlsx"));
		QAxObject * workbook = excel.querySubObject("ActiveWorkBook");
		QAxObject * worksheets = workbook->querySubObject("WorkSheets");
		QAxObject * workbasesheet = workbook->querySubObject("Worksheets(int)", 1);

		int intCount = worksheets->property("Count").toInt();
		for (int i = 2; i <= intCount; i++)
		{
			msg_code = 1;
			int intVal;
			QAxObject * worksheet = workbook->querySubObject("Worksheets(int)", i);
			qDebug() << i << worksheet->property("Name").toString();
			QList<QList<QVariant>> v=  excel_parser::instance().readSheet(worksheet);
			
			
			auto desptr = CommandExecSession::instance()->command_msg_ptr();
			//LLC_GPSPOLL_CycleTime_Stru* msgptr = (LLC_GPSPOLL_CycleTime_Stru*) CommandExecSession::instance()->command_msg_ptr(handle);
			LLC_GPSPOLL_CycleTime_Stru msg;
			char buffer[sizeof(LLC_GPSPOLL_CycleTime_Stru)] = { 0 };
			LLC_GPSPOLL_CycleTime_Stru* msgptr = &msg;

			auto times = sizeof(LLC_GPSPOLL_CycleTime_Stru) / MSG_BODY_LENGTH + 1;
			const int AllCount = times;

			QAxObject * cell2 = workbasesheet->querySubObject("Cells(int,int)", i, 2);
			QAxObject * cell3 = workbasesheet->querySubObject("Cells(int,int)", i, 3);
			QAxObject * cell4 = workbasesheet->querySubObject("Cells(int,int)", i, 4);
			QAxObject * cell5 = workbasesheet->querySubObject("Cells(int,int)",  i, 5);
			msgptr->PollTime_Cycle = qToBigEndian<quint32>( cell2->dynamicCall("Value2()").toString().toInt());
			msgptr->SavedCount = qToBigEndian<quint32>(cell3->dynamicCall("Value2()").toString().toInt());
			msgptr->MSMAX_Count = qToBigEndian<quint32>(cell4->dynamicCall("Value2()").toString().toInt());
			msgptr->MSPOLLed_Index = qToBigEndian<quint32>( cell5->dynamicCall("Value2()").toString().toInt());
			delete cell2;
			delete cell3;
			delete cell4;
			delete cell5;
			qDebug()<< qToBigEndian<quint32>(msgptr->PollTime_Cycle)<<" "
					<< qToBigEndian<quint32>(msgptr->SavedCount )<<" "
					<< qToBigEndian<quint32> (msgptr->MSMAX_Count)<<" "
					<< qToBigEndian<quint32>(msgptr->MSPOLLed_Index)<<""
					<<"len"<<sizeof(*msgptr);
			
			auto ind = 0;
			for (QList<QVariant> lv: v)
			{
				auto i = 0;
				//qDebug() << lv;
				if (lv == *v.begin())
				{
					continue;
				}
				lv.pop_front();

				msgptr->MS_GPSPOLL[ind].MSAddress = qToBigEndian<quint32> (lv.first().toString().toInt());
				lv.pop_front();
				msgptr->MS_GPSPOLL[ind].MsgHandle = qToBigEndian<quint32>(lv.first().toString().toInt());
				lv.pop_front();
				msgptr->MS_GPSPOLL[ind].AB = lv.first().toString().toInt();
				lv.pop_front();
				msgptr->MS_GPSPOLL[ind].GPS_Type = lv.first().toString().toInt();
				lv.pop_front();
				msgptr->MS_GPSPOLL[ind].Ms_Priority = lv.first().toString().toInt();
				lv.pop_front();
				msgptr->MS_GPSPOLL[ind].Begin_Time = qToBigEndian<quint32>(lv.first().toString().toInt());
				lv.pop_front();
				msgptr->MS_GPSPOLL[ind].PolledTime_Last = qToBigEndian<quint32>(lv.first().toString().toInt());
				lv.pop_front();
				msgptr->MS_GPSPOLL[ind].PolledTime_Curr = qToBigEndian<quint32>(lv.first().toString().toInt());
				lv.pop_front();
				msgptr->MS_GPSPOLL[ind].Subscribe_Cycle = qToBigEndian<quint32>(lv.first().toString().toInt());
				lv.pop_front();
				msgptr->MS_GPSPOLL[ind].Speed_Status = lv.first().toString().toInt();
				lv.pop_front();
				msgptr->MS_GPSPOLL[ind].Status_Count = qToBigEndian<quint32>(lv.first().toString().toInt());
				lv.pop_front();

				ind++;
			}
			memcpy(buffer, msgptr, sizeof(LLC_GPSPOLL_CycleTime_Stru));


			auto all_length = sizeof(LLC_GPSPOLL_CycleTime_Stru);
			auto index = 0;
			while (times--)
			{

				all_length = all_length - MSG_BODY_LENGTH;
				auto copy_lenth = all_length > MSG_BODY_LENGTH ? MSG_BODY_LENGTH : all_length;
				for (auto i=0;i<copy_lenth;i++)
				{
					desptr[i] = buffer[i+index];
				}
				index += MSG_BODY_LENGTH;

// 				char * p = (char*)msgptr;
// 				memcpy(p, desptr, copy_lenth);
// 				//auto iter = std::copy_n(p, MSG_BODY_LENGTH,desptr);
				CommandExecSession::instance()->Exec(
									TO_PID(ui->ModulecomboBox->currentText().toStdString()), 
									msg_code, 
									copy_lenth, AllCount -times, AllCount);
// 
// 				msgptr =(LLC_GPSPOLL_CycleTime_Stru*) &((char *)msgptr)[MSG_BODY_LENGTH*(AllCount - times-1) + copy_lenth];
				index++;
				
			}

		}

	}
	else
	{
		CommandExecSession::instance()->Exec(TO_PID(ui->ModulecomboBox->currentText().toStdString()),msg_code);
	}

}



void MainWindow::on_ModulecomboBox_currentIndexChanged(const QString &arg1)
{
	qDebug()<< "[Window] Module Change to "<<arg1;
	QStringList CommandcomBoxList = getMsgList(arg1.toStdString());

//临时为LLC写的
// 	if (arg1 == "LLC")
// 		//CommandcomBoxList << "GPS_STOP"<< "GPS_START";

	ui->CommandcomboBox->clear();
	ui->CommandcomboBox->addItems(CommandcomBoxList);
}

void MainWindow::on_CommandcomboBox_currentIndexChanged(const QString &arg1)
{
	qDebug()<< "[Window] Command Change to " << arg1;
}
