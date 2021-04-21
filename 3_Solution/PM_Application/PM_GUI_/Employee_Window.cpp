#include "Employee_Window.h"

Employee_Window::Employee_Window(QWidget *parent) : QWidget(parent)
{
	ui = new Ui::Employee_Window();
	ui->setupUi(this);
}

Employee_Window::~Employee_Window()
{
	delete ui;
}
