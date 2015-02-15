#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ff7DataInstaller.h"
#include <QProcess>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //set default paths.
#ifdef Q_OS_WIN
    //on windows
    ui->lineConfigDir->setText(QString("%1/q-gears").arg(QDir::homePath()));
    ui->lineQGearsExe->setText(QDir::currentPath() + "/q-gears.exe");
    ui->lineOutput->setText(QString("%1/q-gears/data").arg(QDir::homePath()));
#else
    //on linux
    ui->lineConfigDir->setText(QString("%1/.q-gears").arg(QDir::homePath()));
    ui->lineQGearsExe->setText(QString("/usr/games/q-gears"));
    ui->lineOutput->setText(QString("%1/.q-gears/data").arg(QDir::homePath()));

#endif

#ifdef _DEBUG
    // Hard coded prebaked paths for debugging to save time
    ui->lineInput->setText("C:\\Games\\FF7\\data");
    ui->lineOutput->setText("C:\\Users\\paul\\Desktop\\q-gears\\output\\_data");
#endif

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_btnConfigDir_clicked()
{
	QString temp = QFileDialog::getExistingDirectory(this,tr("Select Location of QGears Configuration Data,"),QDir::homePath());
	ui->lineConfigDir->setText(temp);
}

void MainWindow::on_btnLaunch_clicked()
{
	QString configDir(ui->lineConfigDir->text());
	QString exe(ui->lineQGearsExe->text());

	QStringList args;
	args.append(QString("--resources-file=%1/resources.cfg").arg(ui->lineConfigDir->text()));
	args.append(QString("--config-file=%1/q-gears.cfg").arg(ui->lineConfigDir->text()));
	args.append(QString("--plugins-file=/%1/plugins.cfg").arg(ui->lineConfigDir->text()));

	// Check that the config dir is set up correctly

	QProcess qGears;
	qGears.startDetached(exe,args,configDir);

}
void MainWindow::on_btnQGearsExe_clicked()
{
	QString temp = QFileDialog::getOpenFileName(this,tr("Location of QGears Executable,"),QDir::rootPath());
	ui->lineQGearsExe->setText(temp);
}

void MainWindow::on_btnInput_clicked()
{
	QString temp = QFileDialog::getExistingDirectory(this,tr("Location of Game Data),"),QDir::homePath());
	ui->lineInput->setText(temp);
}


void MainWindow::on_btnOutput_clicked()
{
	QString temp = QFileDialog::getExistingDirectory(this,tr("Location of QGears Data),"),QDir::homePath());
	ui->lineOutput->setText(temp);
}

void MainWindow::on_btnGO_clicked()
{
	if (ui->lineInput->text().isEmpty())
	{
		QMessageBox::critical(this,tr("Input Error"),tr("No Input Provided"));
	}
	else if(ui->lineOutput->text().isEmpty())
	{
		QMessageBox::critical(this,tr("Output Error"),tr("No Output Path Provided"));
	}
	else
	{
		QMessageBox::information(this,tr("Converting Data"),tr("Attempt to convert with \n Input: %1 \n Output: %2").arg(ui->lineInput->text(),ui->lineOutput->text()));

		QString input = QDir::fromNativeSeparators(ui->lineInput->text());
		if (!input.endsWith("/"))
		{
			input += "/";
		}

		QString output = QDir::fromNativeSeparators(ui->lineOutput->text());
		if (!output.endsWith("/"))
		{
			output += "/";
		}

		// TODO: Enumerate files or find some better way to do this :)
		std::vector<std::string> vec;
		vec.push_back(QDir::toNativeSeparators("field/char.lgp").toStdString());
		vec.push_back(QDir::toNativeSeparators("field/flevel.lgp").toStdString());

		FF7DataInstaller conversion;
		conversion.Convert(QDir::toNativeSeparators(input).toStdString(), QDir::toNativeSeparators(output).toStdString(), vec);
	}
}
