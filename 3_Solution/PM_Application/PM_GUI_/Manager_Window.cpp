#include "Manager_Window.h"
#include "PM_GUI_.h"

Manager_Window::Manager_Window(QWidget *parent) : QWidget(parent)
{
	ui = new Ui::Manager_Window();
	ui->setupUi(this);

	database = QSqlDatabase::addDatabase("QODBC");
	database.setHostName("localhost");
	database.setDatabaseName("DRIVER={SQL Server};SERVER=localhost, 1434;DATABASE=Project_Manager;Trusted=true;");

	if (database.open())
	{
		querymodel = new QSqlQueryModel();
		querymodel->setQuery("SELECT * FROM Projects");
		ui->tableView->setModel(querymodel);
	}
	else
	{
		QMessageBox::information(this, "Failed", "Connection failed");
	}
}

Manager_Window::~Manager_Window()
{
	database.close();
	delete ui;
}

void Manager_Window::on_Logout_Button_clicked()
{
	QPixmap pixmap("Resources/icon.png");

	QMessageBox msg;
	msg.setWindowTitle("LOGOUT");
	msg.setText("Are you sure you want to Logout?");
	msg.setIcon(QMessageBox::Question);
	msg.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
	msg.setWindowIcon(QIcon(pixmap));

	int ret = msg.exec();
	switch (ret) {
	case QMessageBox::Yes:
		this->close();
		PM_GUI_* newmain = new PM_GUI_();
		newmain->show();
		break;
	}
}