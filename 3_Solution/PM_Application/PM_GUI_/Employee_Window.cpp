#include "Employee_Window.h"
#include "PM_GUI_.h"

Employee_Window::Employee_Window(QWidget *parent) : QWidget(parent)
{
	ui = new Ui::Employee_Window();
	ui->setupUi(this);
}

Employee_Window::~Employee_Window()
{
	delete ui;
}

void Employee_Window::on_Logout_Button_clicked()
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
